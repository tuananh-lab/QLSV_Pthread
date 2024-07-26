#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"

#define USERNAME_FILE "usernames.txt"

// Function to authenticate user
int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(USERNAME_FILE, "r");
    if (file == NULL) {
        perror("Failed to open username file");
        return 0;
    }

    char file_username[50];
    char file_password[50];

    while (fscanf(file, "%49s %49s", file_username, file_password) == 2) {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            fclose(file);
            return 1; // Authentication successful
        }
    }

    fclose(file);
    return 0; // Authentication failed
}

// Function to get user role after authentication
void get_user_role(char *role, size_t size) {
    (void)size;
    printf("Enter your role (student/teacher): ");
    scanf("%9s", role);
}
