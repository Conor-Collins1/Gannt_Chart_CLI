#include "read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int null_terminate(char *str, int max_len) {
    for (int i = 0; i < max_len; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            return 1;
        }
    }
    return 0;
}
Month read_month() {
    // Longest name in characters of a month of the year + 2 for '\n' + '\0'
    char month[11];
    for (;;) {
        if (read_str(month, 11)) {
            if (month[0] >= '0' && month[0] <= '9') {
                int index_m = atoi(month);
                if (index_m > 0 && index_m <= 12) return (Month)index_m;
            } else {
                for (int i = 1; i <= 12; i++) {
                    if (strcmp(month, months[i]) == 0) return (Month)i;
                }
            }
        }
        // If we fail to find the \n character, the given input was too long.
        printf("Invalid month, please try again\n");
    }
}

void read_month_range(Task *task) {
    Month start, end;
    for (;;) {
        printf("Enter new starting month by name or number (1-12)\n");
        start = read_month();

        printf("\nEnter new ending month by name or number (1-12)\n");
        end = read_month();

        if (start <= end) break;
        printf("Start month must be before the end month, please try again.\n");
    }

    set_task_months(task, start, end);
}

void read_dependencies(Task *task) {
    int dependency_count;
    for (;;) {
        dependency_count = read_number();
        if (dependency_count >= 0 && dependency_count <= 32) break;
        printf("Cannot have more than %d dependencies.\n", MAX_DEPENDENCIES);
    }
    for (int i = 0; i < dependency_count; i++) {
        printf("Enter dependent task (0-%d):\n", MAX_TASKS);
        int dependency;
        for (;;) {
            dependency = read_number();
            if (dependency >= 0 && dependency < 32) {
                add_dependency(task, dependency);
                break;
            }
            printf("Dependency must be within the range (0-%d)\n", MAX_DEPENDENCIES - 1);
        }
    }
}

int read_number() {
    char tmp[5];
    for (;;) {
        if (read_str(tmp, 5)) {
            // Retry if user presses enter
            if (tmp[0] != '\n') return atoi(tmp);
            else continue;
        }
        printf("Invalid number. Likely too long.\n");
    }
}

int read_task(Task *tasks, int *task_count) {
    printf("Please enter the task name\n");
    char name[MAX_NAME_WIDTH];
    for (;;) {
        if (read_str(name, MAX_NAME_WIDTH)) {
            if (name[0] >= '0' && name[0] <= '9') {
                printf("A task name cannot start with a number, please try again.\n");
                continue;
            }
            // User pressed 'enter', exit early and stop reading any more tasks.
            if (name[0] == '\0') {
                printf("Finished reading tasks.\n");
                return 0;
            }
            break;
        }

        // If we fail to find the \n character, the given input was too long.
        printf("Task name is too long, please try again.\n");
    }

    // Add the task to the global task array.
    tasks[*task_count] = new_task((char *)name);
    Task *task = &tasks[*task_count];

    read_month_range(task);
    printf("Enter how many dependencies this task has\n");
    read_dependencies(task);

    // At this point, the task was created successfully
    (*task_count)++;
    return 1;
}

int read_str(char *buf, int size) {
    if (!fgets(buf, size, stdin)) {
        // Happens when user presses 'ctrl+c' or exit the program.
        printf("Gantt Chart Interrupted.\n");
        exit(1);
    }
    // If fgets completes, try null terminate the string.
    return null_terminate(buf, size);
}
