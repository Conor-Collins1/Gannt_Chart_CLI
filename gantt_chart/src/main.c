#include "main.h"
#include "draw.h"
#include "read.h"
#include "task.h"

#include "stdio.h"
#include "stdlib.h"
#include <string.h>

const char *months[13] = {
    [JANUARY] = "January",   [FEBRUARY] = "February", [MARCH] = "March",   [APRIL] = "April",         [MAY] = "May",
    [JUNE] = "June",         [JULY] = "July",         [AUGUST] = "August", [SEPTEMBER] = "September", [OCTOBER] = "October",
    [NOVEMBER] = "November", [DECEMBER] = "December",
};

const char *headings[14] = {
    NULL,   "January", "February",  "March",   "April",    "May",      "June",
    "July", "August",  "September", "October", "November", "December", "Dependencies",
};
int column_widths[14] = {0};
int term_width = -1;

// Gantt chart's tasks array.
Task *tasks = NULL;
// User defined total tasks for the gantt chart.
int total_tasks = 0;
// Tracks the number of tasks read successfully.
int task_count = 0;

int main() {
    clear();
    update_widths();
    printf("Welcome to the Gantt Generator\n");
    printf("Would you like to use the test example or create your own Gantt from "
           "scratch? (yes=example, no=own)\n");

    int use_example = false;

    // Read the user's reply
    for (;;) {
        char resp[5];
        if (read_str(resp, 5)) {
            if (strcmp(resp, "yes") == 0) {
                use_example = true;
                break;
            } else if (strcmp(resp, "no") == 0) break;
        }
        printf("Please enter a valid response, either \"yes\" or \"no\".\n");
    }

    // Load example tasks
    if (use_example) tasks = example_chart(&task_count);
    else {
        // Read user defined tasks:
        printf("How many tasks would you like to add? (1-%d)\n", MAX_TASKS);
        for (;;) {
            total_tasks = read_number();
            if (total_tasks >= 1 && total_tasks <= MAX_TASKS) break;
            else printf("Please enter a valid number between 1 and %d inclusive.\n", MAX_TASKS);
        }

        // The user declared their intention to add n tasks, so we allocate enough
        // space for them here.
        tasks = malloc(sizeof(Task) * MAX_TASKS);
        if (!tasks) {
            printf("Failed to allocate for tasks with %d tasks\n", total_tasks);
            exit(1);
        }

        // Keep reading tasks until all n tasks have been read successfully.
        while (task_count < total_tasks) {
            // Will try to read a task, if it succeeds, it will loop.
            if (!read_task(tasks, &task_count)) break;
            printf("\nAdded task [%d/%d]\n\n", task_count, total_tasks);
        }
    }

    draw_gantt_chart(tasks, task_count);
    free(tasks);
    return 0;
}
