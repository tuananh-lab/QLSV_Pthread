#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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

// Helper function to trim whitespace from the start and end of a string
char* trim_whitespace(char* str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate
    *(end + 1) = '\0';

    return str;
}

int is_student_exists(const char *filename, const Student *new_student) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    // Skip the header line
    char line[MAX_LEN];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Failed to read header line");
        fclose(file);
        return -1;
    }

    // Skip the separator line if it exists
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Failed to read separator line");
        fclose(file);
        return -1;
    }

    // Flag to indicate if we found a duplicate
    int found_duplicate = 0;

    // Read and check student data lines
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '|') { // Assuming student data lines start with '|'
            Student temp_student;
            int fields_read = sscanf(line,
                   "| %8s | %16[^|] | %10[^|] | %10[^|] | %13[^|] | %10[^|] | %10[^|] |",
                   temp_student.id, temp_student.name, temp_student.dob, temp_student.hometown,
                   temp_student.phone, temp_student.major, temp_student.class_t);

            if (fields_read == 7) {
                // Trim whitespace from student data
                trim_whitespace(temp_student.id);
                trim_whitespace(temp_student.name);
                trim_whitespace(temp_student.dob);
                trim_whitespace(temp_student.hometown);
                trim_whitespace(temp_student.phone);
                trim_whitespace(temp_student.major);
                trim_whitespace(temp_student.class_t);

                // Trim whitespace from new student data
                char id[MAX_LEN], name[MAX_LEN], dob[MAX_LEN], hometown[MAX_LEN], phone[MAX_LEN], major[MAX_LEN], class_t[MAX_LEN];
                strcpy(id, new_student->id);
                strcpy(name, new_student->name);
                strcpy(dob, new_student->dob);
                strcpy(hometown, new_student->hometown);
                strcpy(phone, new_student->phone);
                strcpy(major, new_student->major);
                strcpy(class_t, new_student->class_t);

                trim_whitespace(id);
                trim_whitespace(name);
                trim_whitespace(dob);
                trim_whitespace(hometown);
                trim_whitespace(phone);
                trim_whitespace(major);
                trim_whitespace(class_t);

                // Print debug info
                printf("Comparing: ID=%s, Name=%s, DOB=%s, Hometown=%s, Phone=%s, Major=%s, Class=%s\n",
                       id, name, dob, hometown, phone, major, class_t);

                if (strcmp(id, temp_student.id) == 0 &&
                    strcmp(name, temp_student.name) == 0 &&
                    strcmp(dob, temp_student.dob) == 0 &&
                    strcmp(hometown, temp_student.hometown) == 0 &&
                    strcmp(phone, temp_student.phone) == 0 &&
                    strcmp(major, temp_student.major) == 0 &&
                    strcmp(class_t, temp_student.class_t) == 0) {
                    found_duplicate = 1;
                    break; // Stop searching once a duplicate is found
                }
            } else {
                printf("Error reading fields from line: %s\n", line);
            }
        }
    }

    fclose(file);

    if (found_duplicate) {
        // Call the function to overwrite data if a duplicate is found
        overwrite_student_data(filename, new_student);
        return 1; // Student with the same data exists and has been overwritten
    }

    return 0; // Student with the same data does not exist
}

void overwrite_student_data(const char *filename, const Student *new_student) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Create a temporary file
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to create temporary file");
        fclose(file);
        return;
    }

    // Skip header and separator lines
    char line[MAX_LEN];
    if (fgets(line, sizeof(line), file) != NULL) {
        fprintf(temp_file, "%s", line); // Write header to temp file
    }
    if (fgets(line, sizeof(line), file) != NULL) {
        fprintf(temp_file, "%s", line); // Write separator to temp file
    }

    int updated = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '|') { // Assuming student data lines start with '|'
            Student temp_student;
            int fields_read = sscanf(line,
                   "| %8s | %16[^|] | %10[^|] | %10[^|] | %13[^|] | %10[^|] | %10[^|] |",
                   temp_student.id, temp_student.name, temp_student.dob, temp_student.hometown,
                   temp_student.phone, temp_student.major, temp_student.class_t);

            if (fields_read == 7) {
                // Trim whitespace from student data
                trim_whitespace(temp_student.id);
                trim_whitespace(temp_student.name);
                trim_whitespace(temp_student.dob);
                trim_whitespace(temp_student.hometown);
                trim_whitespace(temp_student.phone);
                trim_whitespace(temp_student.major);
                trim_whitespace(temp_student.class_t);

                // Trim whitespace from new student data
                char id[MAX_LEN], name[MAX_LEN], dob[MAX_LEN], hometown[MAX_LEN], phone[MAX_LEN], major[MAX_LEN], class_t[MAX_LEN];
                strcpy(id, new_student->id);
                strcpy(name, new_student->name);
                strcpy(dob, new_student->dob);
                strcpy(hometown, new_student->hometown);
                strcpy(phone, new_student->phone);
                strcpy(major, new_student->major);
                strcpy(class_t, new_student->class_t);

                trim_whitespace(id);
                trim_whitespace(name);
                trim_whitespace(dob);
                trim_whitespace(hometown);
                trim_whitespace(phone);
                trim_whitespace(major);
                trim_whitespace(class_t);

                if (strcmp(id, temp_student.id) == 0 &&
                    strcmp(name, temp_student.name) == 0 &&
                    strcmp(dob, temp_student.dob) == 0 &&
                    strcmp(hometown, temp_student.hometown) == 0 &&
                    strcmp(phone, temp_student.phone) == 0 &&
                    strcmp(major, temp_student.major) == 0 &&
                    strcmp(class_t, temp_student.class_t) == 0) {
                    // Write new student data to temp file
                    fprintf(temp_file, "| %-8s | %-16s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                            new_student->id, new_student->name, new_student->dob, new_student->hometown,
                            new_student->phone, new_student->major, new_student->class_t);
                    fprintf(temp_file, "+----------+------------------+------------+------------+---------------+------------+------------+\n");
                    updated = 1;
                } else {
                    // Write old student data to temp file
                    fprintf(temp_file, "%s", line);
                }
            }
        } else {
            fprintf(temp_file, "%s", line);
        }
    }

    if (!updated) {
        // If no update occurred, write the new student data at the end
        fprintf(temp_file, "| %-8s | %-16s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                new_student->id, new_student->name, new_student->dob, new_student->hometown,
                new_student->phone, new_student->major, new_student->class_t);
        fprintf(temp_file, "+----------+------------------+------------+------------+---------------+------------+------------+\n");
    }

    fclose(file);
    fclose(temp_file);

    // Replace original file with the temporary file
    if (remove(filename) != 0) {
        perror("Error deleting original file");
        return;
    }
    if (rename("temp.txt", filename) != 0) {
        perror("Error renaming temporary file");
        return;
    }
}
