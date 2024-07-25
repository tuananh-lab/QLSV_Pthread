#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Student.h"

#define LOG_FILE "change_log.txt"

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

    // Write the log entry
    fprintf(log, "+---------------------+------------------------------+\n");
    fprintf(log, "| Timestamp           | %-28s |\n", time_buffer);
    fprintf(log, "+---------------------+------------------------------+\n");
    fprintf(log, "| Action              | %-28s |\n", action);
    fprintf(log, "| ID                  | %-28s |\n", student->id);
    fprintf(log, "| Name                | %-28s |\n", student->name);
    fprintf(log, "| Date of Birth       | %-28s |\n", student->dob);
    fprintf(log, "| Hometown            | %-28s |\n", student->hometown);
    fprintf(log, "| Phone               | %-28s |\n", student->phone);
    fprintf(log, "| Major               | %-28s |\n", student->major);
    fprintf(log, "| Class               | %-28s |\n", student->class_t);
    fprintf(log, "+---------------------+------------------------------+\n");
    fprintf(log, "\n");

    fclose(log);
}

void display_log() {
    FILE *log = fopen(LOG_FILE, "r");
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    char buffer[MAX_LEN];
    while (fgets(buffer, MAX_LEN, log) != NULL) {
        printf("%s", buffer);
    }

    fclose(log);
}

void read_string(char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void input_student_data(Student *student) {
    printf("Nhap thong tin sinh vien:\n");

    read_string("MSSV (8 chu so): ", student->id, 9);
    getchar();
    read_string("Ho ten: ", student->name, MAX_LEN);
    read_string("Ngay sinh: ", student->dob, MAX_LEN);
    read_string("Que quan: ", student->hometown, MAX_LEN);
    read_string("So dien thoai: ", student->phone, MAX_LEN);
    read_string("Nganh hoc: ", student->major, MAX_LEN);
    read_string("Lop: ", student->class_t, MAX_LEN);
        
    // Log the addition
    log_change("Added", student);
}

void write_student_to_file(Student *student, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Viết dữ liệu sinh viên theo định dạng bảng
    fprintf(file, "+-----------------+----------------------------------+\n");
    fprintf(file, "| Field           | Value                            |\n");
    fprintf(file, "+-----------------+----------------------------------+\n");
    fprintf(file, "| MSSV            | %-32s |\n", student->id);
    fprintf(file, "| Ho ten          | %-32s |\n", student->name);
    fprintf(file, "| Ngay sinh       | %-32s |\n", student->dob);
    fprintf(file, "| Que quan        | %-32s |\n", student->hometown);
    fprintf(file, "| So dien thoai   | %-32s |\n", student->phone);
    fprintf(file, "| Nganh hoc       | %-32s |\n", student->major);
    fprintf(file, "| Lop             | %-32s |\n", student->class_t);
    fprintf(file, "+-----------------+----------------------------------+\n\n");
    
    fclose(file);
}

void read_and_print_student_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[MAX_LEN];
    int is_new_student = 1; // Flag to indicate the start of a new student

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        // Check for start of a new student's data
        if (strstr(buffer, "+-----------------+----------------------------------+") != NULL) {
            if (!is_new_student) {
                // Print end of table and a separator between students
                printf("+-----------------+----------------------------------+\n\n");
            }
            printf("+-----------------+----------------------------------+\n");
            printf("| Field           | Value                            |\n");
            printf("+-----------------+----------------------------------+\n");
            is_new_student = 1;
        } else if (strstr(buffer, "MSSV:") != NULL) {
            printf("| MSSV            | %-32s |\n", buffer + 5);
        } else if (strstr(buffer, "Ho ten:") != NULL) {
            printf("| Ho ten          | %-32s |\n", buffer + 8);
        } else if (strstr(buffer, "Ngay sinh:") != NULL) {
            printf("| Ngay sinh       | %-32s |\n", buffer + 11);
        } else if (strstr(buffer, "Que quan:") != NULL) {
            printf("| Que quan        | %-32s |\n", buffer + 10);
        } else if (strstr(buffer, "So dien thoai:") != NULL) {
            printf("| So dien thoai   | %-32s |\n", buffer + 15);
        } else if (strstr(buffer, "Nganh hoc:") != NULL) {
            printf("| Nganh hoc       | %-32s |\n", buffer + 11);
        } else if (strstr(buffer, "Lop:") != NULL) {
            printf("| Lop             | %-32s |\n", buffer + 5);
        } else if (strstr(buffer, "+-----------------+----------------------------------+") != NULL) {
            printf("%s", buffer);
            is_new_student = 0;
        } else {
            // Print any other lines that are not part of the table
            printf("%s", buffer);
        }
    }

    // Print the last table's end if necessary
    if (!is_new_student) {
        printf("+-----------------+----------------------------------+\n");
    }

    fclose(file);
}


void delete_student_data(const char *filename, const char *student_id) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        perror("Failed to open temporary file");
        fclose(file);
        return;
    }

    char buffer[MAX_LEN];
    int found = 0;
    while (fgets(buffer, MAX_LEN, file) != NULL) {
        if (strstr(buffer, student_id) != NULL) {
            // Skip writing this student's data to the temp file
            found = 1;
            for (int i = 0; i < 8; i++) {
                fgets(buffer, MAX_LEN, file);
            }
            continue;
        }
        fprintf(temp, "%s", buffer);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
        printf("Student with ID %s has been deleted.\n", student_id);

        // Log the deletion
        Student dummy_student;
        strcpy(dummy_student.id, student_id);
        log_change("Deleted", &dummy_student);

    } else {
        remove("temp.txt");
        printf("Student with ID %s not found.\n", student_id);
    }
}

void update_student_data(const char *filename, const char *student_id, Student *new_data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        perror("Failed to open temporary file");
        fclose(file);
        return;
    }

    char buffer[MAX_LEN];
    int found = 0;
    while (fgets(buffer, MAX_LEN, file) != NULL) {
        if (strstr(buffer, student_id) != NULL) {
            // Write new student data to the temp file
            found = 1;
            fprintf(temp, "MSSV: %s\nHo ten: %s\nNgay sinh: %s\nQue quan: %s\nSo dien thoai: %s\nNganh hoc: %s\nTuoi: %s\nLop: %s\n\n",
                    new_data->id, new_data->name, new_data->dob, new_data->hometown, new_data->phone, new_data->major, new_data->class_t);
            // Skip old student data in the original file
            for (int i = 0; i < 8; i++) {
                fgets(buffer, MAX_LEN, file);
            }
            continue;
        }
        fprintf(temp, "%s", buffer);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
        printf("Student with ID %s has been updated.\n", student_id);

        // Log the update
        log_change("Updated", new_data);
    } else {
        remove("temp.txt");
        printf("Student with ID %s not found.\n", student_id);
    }
}

void search_student_data(const char *filename, const char *search_key, const char *search_value) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[MAX_LEN];
    int in_record = 0;
    int record_found = 0;

    printf("+-----------------+----------------------------------+\n");
    printf("| Field           | Value                            |\n");
    printf("+-----------------+----------------------------------+\n");

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        // Check if this line contains the search value
        if (strstr(buffer, search_value) != NULL) {
            in_record = 1;  // Start printing the record
            record_found = 1;
        }

        if (in_record) {
            // Print the line of the record
            if (strstr(buffer, "MSSV:") || strstr(buffer, "Ho ten:") ||
                strstr(buffer, "Ngay sinh:") || strstr(buffer, "Que quan:") ||
                strstr(buffer, "So dien thoai:") || strstr(buffer, "Nganh hoc:") ||
                strstr(buffer, "Lop:")) {
                // Print the field and value in table format
                char *field = strtok(buffer, ":");
                char *value = strtok(NULL, "\n");

                if (field && value) {
                    printf("| %-17s | %-32s |\n", field, value);
                }
            }

            // End of a record (checking for end of record delimiter)
            if (strstr(buffer, "+-----------------+----------------------------------+") != NULL) {
                printf("%s", buffer);
                in_record = 0;
                continue;
            }
        }
    }

    if (!record_found) {
        printf("No student matching %s: %s found.\n", search_key, search_value);
    } else {
        printf("+-----------------+----------------------------------+\n");
    }

    fclose(file);
}

void clear_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fclose(file);
    printf("The file %s has been cleared.\n", filename);
}



