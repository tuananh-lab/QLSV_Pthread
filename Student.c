#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Student.h"

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
        fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");
        fprintf(log, "| Timestamp           | Action   | MSSV     | Ho ten          | Ngay sinh  | Que quan   | So dien thoai | Nganh hoc  | Lop      |\n");
        fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");
    }

    // Write the log entry
    fprintf(log, "| %-19s | %-8s | %-8s | %-15s | %-10s | %-10s | %-13s | %-10s | %-8s |\n",
            time_buffer, action, student->id, student->name, student->dob,
            student->hometown, student->phone, student->major, student->class_t);
    fprintf(log, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");

    fclose(log);
}

void display_log() {
    FILE *log = fopen(LOG_FILE, "r");
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    char buffer[MAX_LEN];
    int is_header = 1;  // Flag to determine if the header has been printed
    int has_data = 0;
    
    // Print table header
    printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");
    printf("| Timestamp           | Action   | MSSV     | Ho ten          | Ngay sinh  | Que quan   | So dien thoai | Nganh hoc  | Lop      |\n");
    printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");

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
        if (strstr(buffer, "+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+") != NULL) {
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
        printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");
    } else {
        // Print table footer if there is no data
        printf("+---------------------+----------+----------+-----------------+------------+------------+---------------+------------+----------+\n");
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

void read_string(char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void input_student_data(Student *student) {
    printf("Nhap thong tin sinh vien:\n");

    read_string("MSSV (8 chu so): ", student->id, 9);
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

    // Print table headers only if the file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "+----------+-----------------+------------+------------+---------------+------------+----------+\n");
        fprintf(file, "|   MSSV   |     Ho ten      |  Ngay sinh |  Que quan  | So dien thoai | Nganh hoc  |    Lop   |\n");
        fprintf(file, "+----------+-----------------+------------+------------+---------------+------------+----------+\n");
    }

    // Write student data
    fprintf(file, "| %-8s | %-15s | %-10s | %-10s | %-13s | %-10s | %-10s |\n", 
            student->id, student->name, student->dob, student->hometown, 
            student->phone, student->major, student->class_t);
    fprintf(file, "+----------+-----------------+------------+------------+---------------+------------+----------+\n");

    fclose(file);
}

void read_and_print_student_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[MAX_LEN];
    int is_header = 1;  // Flag to determine if the header has been printed

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        // Print the header only once at the beginning of the file
        if (strstr(buffer, "+----------+-----------------+------------+------------+---------------+------------+----------+") != NULL) {
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
            fprintf(temp, "| %-8s | %-15s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                    new_data->id, new_data->name, new_data->dob, new_data->hometown, 
                    new_data->phone, new_data->major, new_data->class_t);
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

    // Display search options
    printf("Searching by: %s\n", search_key);

    char buffer[MAX_LEN];
    int record_found = 0;

    // Print table header
    printf("+----------+-----------------+------------+------------+---------------+------------+----------+\n");
    printf("| MSSV     | Ho ten          | Ngay sinh  | Que quan   | So dien thoai | Nganh hoc  | Lop      |\n");
    printf("+----------+-----------------+------------+------------+---------------+------------+----------+\n");

    while (fgets(buffer, MAX_LEN, file) != NULL) {
        // Skip header and separator lines
        if (strstr(buffer, "+----------+-----------------+------------+------------+---------------+------------+----------+") != NULL) {
            continue;
        }

        // Check if this line contains the search value
        if (strstr(buffer, search_value) != NULL) {
            // Read the student data from the file and display it in table format
            Student student;
            sscanf(buffer, "| %8s | %15[^|] | %10[^|] | %10[^|] | %13[^|] | %10[^|] | %10[^|] |",
                   student.id, student.name, student.dob, student.hometown,
                   student.phone, student.major, student.class_t);
            printf("| %-8s | %-15s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                   student.id, student.name, student.dob, student.hometown,
                   student.phone, student.major, student.class_t);
            printf("+----------+-----------------+------------+------------+---------------+------------+----------+\n");
            record_found = 1;
        }
    }

    if (!record_found) {
        printf("No records found for the given search criteria.\n");
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
