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
            return 1; // Authentication successful
        }
    }

    fclose(file_ptr);
    return 0; // Authentication failed
}

// Function to get user role after authentication
void get_user_role(char *role, size_t size) {
    (void)size;
    printf("Enter your role (student/teacher): ");
    scanf("%9s", role);
}

// Function to authenticate student
int authenticate_student(const char *username, const char *password) {
    return authenticate_user(username, password, STUDENT_FILE);
}

// Function to authenticate teacher
int authenticate_teacher(const char *username, const char *password) {
    return authenticate_user(username, password, TEACHER_FILE);
}