#include "task.h"
#include "draw.h"
#include "main.h"
#include "read.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Task new_task(const char *name) {
    Task row = (Task){.name = {0}, .start_month = JANUARY, .end_month = JANUARY, .dependencies = 0u};
    int len = strlen(name);
    if (len > MAX_NAME_WIDTH) {
        printf("Task name is too long\n");
        exit(1);
    }
    memcpy((char *)row.name, name, len);
    return row;
}

Task *get_task(Task *tasks, int task_count, const char *name) {
    // Loop over every task in the global tasks array.
    for (int i = 0; i < task_count; i++) {
        // If the names match, return a reference to it.
        if (strcmp(name, tasks[i].name) == 0) return &tasks[i];
    }
    // If no match was found,
    return NULL;
}

void set_task_months(Task *task, Month from, Month to) {
    task->start_month = from;
    task->end_month = to;
}

void add_dependency(Task *task, int dependency) {
    // bitset array is 32 bits, so the dependency must be 0-31
    if (dependency >= MAX_DEPENDENCIES) {
        clear();
        printf("CRASH: Dependency number is too large\n");
        exit(1);
    }
    task->dependencies |= (1u << dependency);
}
void remove_dependency(Task *task, int dependency) {
    // bitset array is 32 bits, so the dependency must be 0-31
    if (dependency >= MAX_DEPENDENCIES) {
        clear();
        printf("CRASH: Dependency number is too large\n");
        exit(1);
    }
    task->dependencies &= ~(1u << dependency);
}
void clear_dependencies(Task *task) { task->dependencies = 0u; }

Task *example_chart(int *task_count) {
    Task *example_tasks = malloc(sizeof(Task) * MAX_TASKS);
    if (!example_tasks) {
        printf("Failed to allocate for example\n");
        exit(1);
    }
    clear();
    draw_header();
    example_tasks[0] = new_task("Create_Github");
    set_task_months(&example_tasks[0], JANUARY, FEBRUARY);

    example_tasks[1] = new_task("Make_task_struct");
    set_task_months(&example_tasks[1], FEBRUARY, FEBRUARY);
    add_dependency(&example_tasks[1], 2);

    example_tasks[2] = new_task("Terminal_Sizing");
    set_task_months(&example_tasks[2], APRIL, JUNE);
    add_dependency(&example_tasks[2], 0);
    add_dependency(&example_tasks[2], 1);

    example_tasks[3] = new_task("Example_tasks");
    set_task_months(&example_tasks[3], APRIL, MAY);
    add_dependency(&example_tasks[3], 2);

    example_tasks[4] = new_task("Print_table");
    set_task_months(&example_tasks[4], MAY, JUNE);

    example_tasks[5] = new_task("Dependencies");
    set_task_months(&example_tasks[5], JUNE, JULY);
    add_dependency(&example_tasks[5], 3);
    add_dependency(&example_tasks[5], 4);

    example_tasks[6] = new_task("Critical_path");
    set_task_months(&example_tasks[6], JULY, JULY);
    add_dependency(&example_tasks[6], 5);

    example_tasks[7] = new_task("Edit_tasks");
    set_task_months(&example_tasks[7], AUGUST, SEPTEMBER);
    add_dependency(&example_tasks[7], 6);

    example_tasks[8] = new_task("Create_doc");
    set_task_months(&example_tasks[8], SEPTEMBER, NOVEMBER);
    add_dependency(&example_tasks[8], 4);
    *task_count = 9;
    return example_tasks;
}
/* !!!PAIR PROGRAMMED!!! */
bool is_circular(int visited[MAX_DEPENDENCIES], Task *tasks, int task_count, int current_task) {
    if (visited[current_task]) return true;

    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        if (tasks[current_task].dependencies & (1u << i)) {
            visited[current_task] = 1;
            if (is_circular(visited, tasks, task_count, i)) return true;
        }
    }
    return false;
}

void edit_tasks(Task *tasks, int task_count) {
    char task_buf[MAX_NAME_WIDTH];

    printf("\n\nIn edit mode\n");
    printf("Enter task name or index:\n");
    Task *task = NULL;

    // Checks if first lettter in task is a number(index).
    for (;;) {
        read_str(task_buf, MAX_NAME_WIDTH);
        if (task_buf[0] >= '0' && task_buf[0] <= '9') {
            int i = task_buf[0] - '0';
            if (i < task_count) task = &tasks[i];
        }
        // function which searches all tasks and returns first one with that name
        else
            task = get_task(tasks, task_count, task_buf);

        if (task) break;
        printf("\nError: Task not found try again:\n");
    }

    printf("\nTask Found (%s)\n", task->name);
    printf("What would you like to edit:\n\n  1. Name\n  2. Time Range\n  3. Add Dependencies\n  4. Remove Dependencies\n");

    for (;;) {
        printf("\nEnter an option(1-4):\n");
        int edit_buf = read_number();
        switch (edit_buf) {
        // Edit Name
        case 1:
            printf("\nEnter new name:\n");
            char name_buf[MAX_NAME_WIDTH];
            if (read_str(name_buf, MAX_NAME_WIDTH) == 1) {
                strcpy_s(task->name, MAX_NAME_WIDTH, name_buf);
                printf("Name changed successfully.\n");
                break;
            }
            continue;
        // Edit Time Range
        case 2:
            read_month_range(task);
            break;
        // Add dependencies
        case 3:
            printf("Enter how many dependencies you want to add:\n");
            read_dependencies(task);
            break;
        // Remove dependency
        case 4:
            if (task->dependencies == 0u) {
                printf("This task already has no dependencies, nothing to remove\n");
                continue;
            }
            int dependency;
            for (;;) {
                printf("Enter dependent task to remove:\n");
                dependency = read_number();
                if (dependency >= 0 && dependency <= 32) break;
                printf("Dependency count cannot be negative or more than %d\n", MAX_DEPENDENCIES - 1);
            }
            remove_dependency(task, dependency);
            break;
        default:
            printf("Invalid input (must be between 1-4)\n");
            continue;
        }
        break;
    }
}
