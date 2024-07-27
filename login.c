#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"

#define USERNAME_FILE "usernames.txt"
#define STUDENT_FILE "students.txt"
#define TEACHER_FILE "teachers.txt"

// Function to authenticate user
int authenticate_user(const char *username, const char *password, const char *file) {
    FILE *file_ptr = fopen(file, "r");
    if (file_ptr == NULL) {
        perror("Failed to open file");
        return 0;
    }

    char file_username[50];
    char file_password[50];

    while (fscanf(file_ptr, "%49s %49s", file_username, file_password) == 2) {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            fclose(file_ptr);
            return 1;
        }
    }

    fclose(file_ptr);
    return 0;
}

// Function to get user role after authentication
void get_user_role(char *role, size_t size) {
    if (size < 2) {
        fprintf(stderr, "Error: Provided buffer size is too small.\n");
        return;
    }

    printf("Enter your role (1 for student or 2 for teacher): ");
    if (scanf("%1s", role) != 1) {
        fprintf(stderr, "Error: Failed to read role.\n");
        return;
    }

    // Validate input to ensure it is either "1" or "2"
    if (strcmp(role, "1") != 0 && strcmp(role, "2") != 0) {
        fprintf(stderr, "Error: Invalid role entered. Please enter '1' or '2'.\n");
        role[0] = '\0';  // Clear the role to indicate invalid input
    }
}

// Function to authenticate student
int authenticate_student(const char *username, const char *password) {
    return authenticate_user(username, password, STUDENT_FILE);
}

// Function to authenticate teacher
int authenticate_teacher(const char *username, const char *password) {
    return authenticate_user(username, password, TEACHER_FILE);
}

