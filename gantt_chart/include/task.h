#pragma once

#include "stdbool.h"
#include "main.h"

#define MAX_NAME_WIDTH 24

/* The width of a task name heading, calculated at runtime. */
extern int task_width;

typedef struct Row {
    char name[MAX_NAME_WIDTH];
    Month start_month;
    Month end_month;
    unsigned int dependencies;
} Task;

/*
    Creates a new empty task with the given name.
*/
Task new_task(const char *name);
/*
    Searches the global tasks array for a task with the same name and returns it if found.
    Otherwise NULL.
*/
Task *get_task(Task *tasks, int task_count, const char *name);

/*
    Updates a task's start and end months.
*/
void set_task_months(Task *task, Month from, Month to);

/*
    Adds a dependency to the given task.
    If it already has the dependency, nothing happens.
*/
void add_dependency(Task *task, int dependency);

/*
    Remove a dependency from the given task.
    If it doesnt have the dependency already, nothing happens.
*/
void remove_dependency(Task *task, int dependency);
/*
    Removes all dependencies from the given task.
*/
void clear_dependencies(Task *task);

/*
    Generates the exact same example as seen in the assignment brief.
*/
Task *example_chart(int *task_count);

/*
    Recursively check a task and its dependencies.
    Record all visited tasks. If a task is visitied twice, it is circular.
    Return true in this case, otherwise false.
*/
bool is_circular(int visited[MAX_DEPENDENCIES], Task *tasks, int task_count, int current_task);


/*
    A menu that lets you edit any aspect of any task.
    Also allows new tasks to be made.
*/
void edit_tasks(Task *tasks, int task_count);
