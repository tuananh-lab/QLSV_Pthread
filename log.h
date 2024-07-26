#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "student.h"

#define LOG_FILE "change_log.txt"
#define MAX_LEN 256

// Function prototypes
void log_change(const char *action, const Student *student);
void display_log();
void clear_log();

#endif // LOG_H
