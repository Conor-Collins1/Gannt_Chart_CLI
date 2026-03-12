#pragma once
#include "task.h"

/*
    Replaces the '\n' at the end of a user entered string with '\0'.
    Returns 1, if success and 0 if the '\n' is not found as it expects to find '\n'.
*/
int null_terminate(char *str, int max_len);

/*
    Reads a task from the user, asks for Name, Start & End months and Dependencies.
    Automatially adds the task to the global tasks array.
    If a user presses 'enter', it will exit and stop trying to add more tasks (by returning 0, instead of 1).
*/
int read_task(Task *tasks, int *task_count);
/*
    Reads a string of max length 'size', from the user, and automatically null terminates (adds replaces '\n' with '\0').
*/
int read_str(char *buf, int size);
/*
    Attempts to read a month from the user. Will keep trying until a valid month is given.
    Is case sensitive.
    Supports month by name or by number (1-12).
*/
Month read_month();
/*
    Read the start and end month from the user, and update the given task accordingly.
*/
void read_month_range(Task *task);
/*
    Reads the number of dependencies, and then the specific dependencies.
    Then adds them to the given task.
*/
void read_dependencies(Task *task);
/*
    Reads a string, and converts it to integer.
*/
int read_number();
