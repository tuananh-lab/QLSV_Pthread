#include <stdio.h>
#include <string.h>
#include "Student.h"

void input_student_data(Student *student) {
    printf("Nhap thong tin sinh vien:\n");
  
    printf("MSSV (8 chu so): ");
    fgets(student->id, 9, stdin);
    student->id[strcspn(student->id, "\n")] = '\0'; // Remove newline character

    getchar(); // Remove newline character from input buffer

    printf("Ho ten: ");
    fgets(student->name, MAX_LEN, stdin);
    student->name[strcspn(student->name, "\n")] = '\0'; // Remove newline character

    getchar(); // Remove newline character from input buffer

    printf("Ngay sinh: ");
    fgets(student->dob, MAX_LEN, stdin);
    student->dob[strcspn(student->dob, "\n")] = '\0'; // Remove newline character

    getchar(); // Remove newline character from input buffer

    printf("Que quan: ");
    fgets(student->hometown, MAX_LEN, stdin);
    student->hometown[strcspn(student->hometown, "\n")] = '\0'; // Remove newline character

    getchar(); // Remove newline character from input buffer

    printf("So dien thoai: ");
    fgets(student->phone, MAX_LEN, stdin);
    student->phone[strcspn(student->phone, "\n")] = '\0'; // Remove newline character

    getchar(); // Remove newline character from input buffer

    printf("Nganh hoc: ");
    fgets(student->major, MAX_LEN, stdin);
    student->major[strcspn(student->major, "\n")] = '\0'; // Remove newline character
    
    getchar(); // Remove newline character from input buffer

    printf("Lop: ");
    fgets(student->class_t, MAX_LEN, stdin);
    student->class_t[strcspn(student->class_t, "\n")] = '\0'; // Remove newline character
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