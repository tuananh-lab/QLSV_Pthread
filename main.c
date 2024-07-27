#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "teacher.h"
#include "thread.h"
#include "login.h"

#define STUDENT_FILE "students.txt"
#define TEACHER_FILE "teachers.txt"

// Mutex and condition variables
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
// Flags to control thread synchronization
int data_ready = 0;
int file_written = 0;

// Global student variable to share data between threads
Student student;

void login_and_run(pthread_t* thread) {
    char username[50];
    char password[50];
    char role[2];
    int authenticated = 0;

    while (!authenticated) {
        // Prompt for username
        printf("Enter username: ");
        if (scanf("%49s", username) != 1 || strlen(username) == 0) {
            fprintf(stderr, "Username cannot be empty. Please try again.\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Prompt for password
        printf("Enter password: ");
        if (scanf("%49s", password) != 1 || strlen(password) == 0) {
            fprintf(stderr, "Password cannot be empty. Please try again.\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Prompt for role
        printf("Enter your role (1 for student or 2 for teacher): ");
        if (scanf("%1s", role) != 1 || (strlen(role) == 0)) {
            fprintf(stderr, "Role cannot be empty. Please try again.\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Validate and authenticate based on role
        if (strcmp(role, "1") == 0) {
            authenticated = authenticate_student(username, password);
            if (!authenticated) {
                printf("Invalid username or password for student. Please try again.\n");
            }
        } else if (strcmp(role, "2") == 0) {
            authenticated = authenticate_teacher(username, password);
            if (!authenticated) {
                printf("Invalid username or password for teacher. Please try again.\n");
            }
        } else {
            printf("Invalid role entered. Please enter 1 for 'student' or 2 for 'teacher'.\n");
            // Clear role
            role[0] = '\0';
            continue;
        }
    }

    // Start the role-specific thread
    pthread_create(thread, NULL, thread1_func, (void*)strdup(role));
    pthread_join(*thread, NULL);
}

int main() {
    pthread_t thread1, thread2, thread3;
    
    // Initialize mutex and condition variables
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

    // Start login and role-specific thread
    login_and_run(&thread1);
    
    // Create remaining threads
    pthread_create(&thread2, NULL, thread2_func, NULL);
    pthread_create(&thread3, NULL, thread3_func, NULL);

    // Wait for all threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    // Destroy mutex and condition variables
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
    
    return 0;
}
