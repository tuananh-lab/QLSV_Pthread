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
    char role[10];
    int authenticated = 0;

    while (!authenticated) {
        // Prompt for username and password
        printf("Enter username: ");
        scanf("%49s", username);
        printf("Enter password: ");
        scanf("%49s", password);

        // Authenticate user based on role
        printf("Enter your role (student/teacher): ");
        scanf("%9s", role);

        if (strcmp(role, "student") == 0) {
            authenticated = authenticate_student(username, password);
        } else if (strcmp(role, "teacher") == 0) {
            authenticated = authenticate_teacher(username, password);
        } else {
            printf("Invalid role. Please enter 'student' or 'teacher'.\n");
            continue;
        }

        if (!authenticated) {
            printf("Invalid username or password. Please try again.\n");
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
