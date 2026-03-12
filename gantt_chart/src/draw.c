#include "draw.h"

#include "main.h"
#include "read.h"
#include "task.h"
#include <stdio.h>
#include <vadefs.h>

int task_width;
int heading_width;

#ifdef _WIN32
#include "windows.h"
void set_char_width_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // Try get window dimensions, using 1920x1080 defaults as fallback.
    term_width = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) ? csbi.dwSize.X : 209;
}
#else
#include "sys/ioctl.h"
// Linux version, completely untested...
void set_char_width_height() {
    struct winsize size;
    // Try get window dimensions, using 1920x1080 defaults as fallback.
    term_width = !ioctl(0, TIOCGWINSZ, &size) ? max.ws_row : 209;
}
#endif

void update_widths() {
    set_char_width_height();
    // Shrink Month Column Widths if terminal is smaller than 180 characters.
    heading_width = term_width < 180 ? 9 : 12;
    // Total width minus the 12 months. minus the twelve '|' vertical spacers.
    // Halved.
    task_width = (term_width - 12 * heading_width - 12) / 2;
    for (int i = 0; i < HEADINGS; i++) {
        column_widths[i] = heading_width;
    }
    int total_width = 12 * heading_width + 2 * task_width + 12;
    // Update task name column width
    column_widths[0] = task_width;
    // Update Dependencies column width
    column_widths[13] = task_width;

    // Needed to properly size odd width terminals
    if (total_width == term_width) column_widths[0]--;
}

void draw_whitespace_n(int n) {
    for (int i = 0; i < n; i++) {
        putchar(' ');
    }
}

void draw_h_spacer() {
    printf("\n");
    for (int i = 0; i < term_width; i++) {
        putchar('-');
    }
    printf("\n");
}
void draw_v_spacer() { putchar('|'); }
void draw_ellipses(const char *str, int max_len) {
    printf("%.*s", max_len - 4, str);
    printf("... ");
}
void draw_centered(const char *str, int max_len) {
    int text_width = strlen(str);
    // If the given text is too long, no need to align at all just crop.
    if (text_width > max_len) return draw_ellipses(str, max_len);
    int delta = max_len - text_width;
    int pad = delta / 2;
    draw_whitespace_n(pad);
    printf("%s", str);
    // Integer division may skip a whitespace if the whitespace length is odd, place the extra whitespace here.
    draw_whitespace_n(pad + delta % 2);
}
void draw_left_align(const char *str, int max_len) {
    int text_width = strlen(str);
    if (text_width > max_len) return draw_ellipses(str, max_len);
    int delta = max_len - text_width;
    printf("%s", str);
    draw_whitespace_n(delta);
}

void draw_header() {
    for (int i = 0; i < HEADINGS; i++) {
        if (headings[i]) draw_centered(headings[i], column_widths[i]);
        // If the heading is empty, draw whitespace instead.
        else draw_whitespace_n(column_widths[i]);
        // For all headings except the last one, place a '|' spacer
        if (i < HEADINGS - 1) draw_v_spacer();
    }
    draw_h_spacer();
}
void draw_footer() {
    int width = term_width / 4 - 3;
    draw_centered("To edit the Gantt, type \'edit\'", width);
    draw_v_spacer();
    draw_centered("To run a test, type  \'test\'", width);
    draw_v_spacer();
    draw_centered("To make a new task, type  \'new\'", width);
    draw_v_spacer();
    draw_centered("To exit type \'quit\'", width);
    printf("\n");
}
void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void draw_task(Task *task) {
    // Minus 3 on the width to account for task numbering "x| "
    draw_left_align(task->name, column_widths[0] - 3);
    draw_v_spacer();
    // Draw the calendar section.
    for (int i = 1; i <= 12; i++) {
        bool within_range = i >= task->start_month && i <= task->end_month;
        draw_centered(within_range ? "xxx" : "", heading_width);
        if (i < 12) draw_v_spacer();
    }
    draw_v_spacer();
    draw_whitespace();
    int width = 1;
    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        // Stop printing dependencies and crop, leaving ~5 characters
        if (width >= task_width - 5) {
            printf("...");
            width += 3;
            break;
        }
        if (task->dependencies & (1u << i)) {
            printf("%d ", i);
            width += 2;
            // Track extra width for double digit characters
            if (i > 9) width++;
        }
    }
    draw_whitespace_n(task_width - width - 1);
    draw_h_spacer();
}

void draw_gantt_chart(Task *tasks, int task_count) {
    clear();
    draw_header();
    for (int i = 0; i < task_count; i++) {
        // Number the tasks
        printf("%d", i);
        draw_v_spacer();
        draw_whitespace();
        draw_task(&tasks[i]);
    }
    draw_footer();

    // Handle gantt chart interactive commands.
    char resp_buf[6];
    for (;;) {
        read_str(resp_buf, 6);
        if (strcmp(resp_buf, "test") == 0) {
            printf("\n");
            bool found_circular = false;
            // Loop over every task, and check if it has a cirular dependency tree
            for (int i = 0; i < task_count; i++) {
                int visited[MAX_DEPENDENCIES] = {0};
                if (is_circular(visited, tasks, task_count, i)) {
                    printf("- Task \'%s\' is circular.\n", tasks[i].name);
                    found_circular = true;
                }
            }
            if (!found_circular) printf("The Gantt Chart passed the test.\n");

            printf("\nPress enter to clear.\n");
            read_str(resp_buf, 6);
            break;
        }
        if (strcmp(resp_buf, "edit") == 0) {
            edit_tasks(tasks, task_count);
            break;
        }
        if (strcmp(resp_buf, "quit") == 0) return;
        if (strcmp(resp_buf, "new") == 0) {
            if (task_count >= 10) {
                printf("Error: Maximum tasks reached, please edit one instead.\n");
                printf("\nPress enter to clear.\n");
                read_str(resp_buf, 6);
            } else read_task(tasks, &task_count);
            break;
        }
        printf("Invalid command, try test, edit, quit, new.\n");
    }
    draw_gantt_chart(tasks, task_count);
}
