#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "teacher.h"

#define LOG_FILE "change_log.txt"
#define MAX_LEN 256

void display_student_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[MAX_LEN];
    int is_header = 1;

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        if (strstr(buffer, "+----------+------------------+------------+------------+---------------+------------+------------+") != NULL) {
            if (is_header) {
                printf("%s", buffer);
                is_header = 0;
            }
        } else {
            printf("%s", buffer);
        }
    }

    fclose(file);
}

void search_student_data(const char *filename, const char *search_key, const char *search_value) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    printf("Searching by: %s\n", search_key);

    char buffer[MAX_LEN];
    int record_found = 0;

    printf("+----------+------------------+------------+------------+---------------+------------+------------+\n");
    printf("| MSSV     | Ho ten           | Ngay sinh  | Que quan   | So dien thoai | Nganh hoc  | Lop        |\n");
    printf("+----------+------------------+------------+------------+---------------+------------+------------+\n");

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        if (strstr(buffer, search_value) != NULL) {
            Student student;
            sscanf(buffer, "| %8s | %16[^|] | %10[^|] | %10[^|] | %13[^|] | %10[^|] | %10[^|] |",
                   student.id, student.name, student.dob, student.hometown,
                   student.phone, student.major, student.class_t);
            printf("| %-8s | %-16s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                   student.id, student.name, student.dob, student.hometown,
                   student.phone, student.major, student.class_t);
            printf("+----------+------------------+------------+------------+---------------+------------+------------+\n");
            record_found = 1;
        }
    }

    if (!record_found) {
        printf("No records found for the given search criteria.\n");
    }

    fclose(file);
}
