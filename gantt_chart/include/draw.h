#pragma once

#include "task.h"

/*
    Retrieve the terminal dimensions in characters.
    Implemented for windows and linux hypothetically...
    Must be run before any draw calls.
*/
void set_char_width_height();
/*
    The column widths and task width must be computed after we retrieve the terminal dimensions.
*/
void update_widths();

/* Draw a ' ' character n times. */
void draw_whitespace_n(int n);
/* Draw a single whitespace. */
static inline void draw_whitespace() { draw_whitespace_n(1); }

/*
    Draw a '|' character.
*/
void draw_h_spacer();
/*
    Draw '-' for the width of the terminal.
*/
void draw_v_spacer();
/*
    Draw the given string up to max_len characters - 3, then adds '...'
*/
void draw_ellipses(const char *str, int max_len);
/*
    Draw the given string centered with spaces for padding left and right to fill max_len characters.
*/
void draw_centered(const char *str, int max_len);
/*
    Draw the given string and add max_len padding spaces to the right.
*/
void draw_left_align(const char *str, int max_len);

/*
    Draw the gantt chart's header.
*/
void draw_header();
/*
    Draw the gantt chart's footer.
*/
void draw_footer();
/*
    Clear the terminal.
*/
void clear();

/*
    Draw the task as a row in the gantt chart.
    Includes, task name, months and dependencies.
*/
void draw_task(Task *task);
/*
    Draw the header.
    Draw every task.
    Draw the footer.
*/
void draw_gantt_chart(Task *tasks, int task_count);
