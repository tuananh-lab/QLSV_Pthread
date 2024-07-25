#include <stdio.h>
#include <string.h>
#include "Student.h"

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
}

void write_student_to_file(Student *student, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    
    fprintf(file, "MSSV: %s\nHo ten: %s\nNgay sinh: %s\nQue quan: %s\nSo dien thoai: %s\nNganh hoc: %s\nLop: %s\n\n",
            student->id, student->name, student->dob, student->hometown, student->phone, student->major, student->class_t);
    fclose(file);
}

void read_and_print_student_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    
    char buffer[MAX_LEN];
    printf("Thong tin sinh vien:\n");
    while (fgets(buffer, MAX_LEN, file) != NULL) {
        printf("%s", buffer);
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
    int found = 0;
    while (fgets(buffer, MAX_LEN, file) != NULL) {
        if (strstr(buffer, search_value) != NULL) {
            found = 1;
            do {
                printf("%s", buffer);
                fgets(buffer, MAX_LEN, file);
            } while (strlen(buffer) > 1); // Continue reading until an empty line or EOF
            printf("\n");
        }
    }

    if (!found) {
        printf("No student matching %s: %s found.\n", search_key, search_value);
    }

    fclose(file);
}