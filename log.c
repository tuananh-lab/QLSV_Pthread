#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "student.h"

#define LOG_FILE "change_log.txt"
#define MAX_LEN 256

void log_change(const char *action, const Student *student) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_buffer[26];
    strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    // Print table headers only if the file is empty
    fseek(log, 0, SEEK_END);
    if (ftell(log) == 0) {
        fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+\n");
        fprintf(log, "| Timestamp           | Action   | MSSV     | Ho ten          | Ngay sinh  | Que quan   | So dien thoai | Nganh hoc  | Lop        |\n");
        fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+\n");
    }

    // Write the log entry
    fprintf(log, "| %-19s | %-8s | %-8s | %-15s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
            time_buffer, action, student->id, student->name, student->dob,
            student->hometown, student->phone, student->major, student->class_t);
    fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+\n");

    fclose(log);
}


void display_log() {
    FILE *log = fopen(LOG_FILE, "r");
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    char buffer[MAX_LEN];
    int is_header = 1;
    int has_data = 0;

    while (fgets(buffer, MAX_LEN, log) != NULL) {
        // Check if there is any data line
        if (strstr(buffer, "Timestamp") == NULL &&
            strstr(buffer, "Action") == NULL &&
            strstr(buffer, "MSSV") == NULL &&
            strstr(buffer, "Ho ten") == NULL &&
            strstr(buffer, "Ngay sinh") == NULL &&
            strstr(buffer, "Que quan") == NULL &&
            strstr(buffer, "So dien thoai") == NULL &&
            strstr(buffer, "Nganh hoc") == NULL &&
            strstr(buffer, "Lop") == NULL) {
            has_data = 1;
        }

        // Print the log content
        if (strstr(buffer, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+") != NULL) {
            if (is_header) {
                printf("%s", buffer);
                is_header = 0;
            }
        } else {
            printf("%s", buffer);
        }
    }

    // Print footer only if there was any data
    if (has_data) {
        printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+\n");
    } else {
        // Print table footer if there is no data
        printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+------------+\n");
    }

    fclose(log);
}
void clear_log(void) {
    FILE *log = fopen(LOG_FILE, "w");
    if (log == NULL) {
        perror("Failed to clear log file");
        return;
    }
    fclose(log);
}