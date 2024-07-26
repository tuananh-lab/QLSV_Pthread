#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "teacher.h"
#include "thread.h"

extern pthread_mutex_t lock;
extern pthread_cond_t cond1;
extern pthread_cond_t cond2;
extern int data_ready;
extern int file_written;
extern Student student;

void* thread1_func(void* arg) {
    char* role = (char*)arg;

    while (1) {
        pthread_mutex_lock(&lock);

        // Wait for thread 2 to signal that data is ready
        while (data_ready) {
            pthread_cond_wait(&cond2, &lock);
        }

        char choice;
        char filename[MAX_LEN] = "thongtinsinhvien.txt";
        if (strcmp(role, "student") == 0) {
            printf("Student actions:\n");
            printf("Choose action (s: SEARCH, l: LOG): ");
            scanf(" %c", &choice);
            getchar(); // Remove newline character left in buffer

            switch (choice) {
                case 's': {
                    char search_key[MAX_LEN];
                    char search_value[MAX_LEN];
                    printf("Enter search key (MSSV, NAME, DOB, HOMETOWN, PHONE, MAJOR, CLASS): ");
                    fgets(search_key, MAX_LEN, stdin);
                    search_key[strcspn(search_key, "\n")] = '\0'; // Remove newline
                    printf("Enter search value: ");
                    fgets(search_value, MAX_LEN, stdin);
                    search_value[strcspn(search_value, "\n")] = '\0'; // Remove newline
                    search_student_data(filename, search_key, search_value);
                    break;
                }
                case 'l': {
                    display_log();
                    break;
                }
                default:
                    printf("Invalid action.\n");
                    break;
            }
        } else if (strcmp(role, "teacher") == 0) {
            printf("Teacher actions:\n");
            printf("Choose action (a: ADD, d: DELETE, u: UPDATE, s: SEARCH, l: LOG, c: CLEAR): ");
            scanf(" %c", &choice);
            getchar(); // Remove newline character left in buffer

            switch (choice) {
                case 'a': {
                    input_student_data(&student);
                    data_ready = 1;
                    break;
                }
                case 'd': {
                    char id[9];
                    printf("Enter student ID (MSSV) to delete: ");
                    fgets(id, sizeof(id), stdin);
                    id[strcspn(id, "\n")] = '\0'; // Remove newline
                    delete_student_data(filename, id);
                    break;
                }
                case 'u': {
                    char id[9];
                    Student new_student;
                    printf("Enter student ID (MSSV) to update: ");
                    fgets(id, sizeof(id), stdin);
                    id[strcspn(id, "\n")] = '\0'; // Remove newline
                    input_student_data(&new_student);
                    update_student_data(filename, id, &new_student);
                    break;
                }
                case 's': {
                    char search_key[MAX_LEN];
                    char search_value[MAX_LEN];
                    printf("Enter search key (MSSV, NAME, DOB, HOMETOWN, PHONE, MAJOR, CLASS): ");
                    fgets(search_key, MAX_LEN, stdin);
                    search_key[strcspn(search_key, "\n")] = '\0'; // Remove newline
                    printf("Enter search value: ");
                    fgets(search_value, MAX_LEN, stdin);
                    search_value[strcspn(search_value, "\n")] = '\0'; // Remove newline
                    search_student_data(filename, search_key, search_value);
                    break;
                }
                case 'l': {
                    display_log();
                    break;
                }
                case 'c': {
                    clear_file(filename);
                    break;
                }
                default:
                    printf("Invalid action.\n");
                    break;
            }
        } else {
            printf("Invalid role.\n");
        }

        pthread_cond_signal(&cond1); // Signal thread 2 to write data to file
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    free(role); // Free the memory allocated for role
    return NULL;
}

void* thread2_func(void* arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&lock);
        while (!data_ready) {
            pthread_cond_wait(&cond1, &lock);
        }
        
        write_student_to_file(&student, "thongtinsinhvien.txt");
        
        file_written = 1;
        pthread_cond_signal(&cond2); // Signal thread 3 to read data from file
        data_ready = 0;
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    
    return NULL;
}

void* thread3_func(void* arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&lock);
        while (!file_written) {
            pthread_cond_wait(&cond2, &lock);
        }
        
        read_and_print_student_data("thongtinsinhvien.txt");
        
        file_written = 0;
        pthread_cond_signal(&cond1); // Signal thread 1 to input more data
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    
    return NULL;
}
