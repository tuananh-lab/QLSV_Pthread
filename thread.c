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
    Student student;
    char filename[MAX_LEN] = "thongtinsinhvien.txt";

    while (1) {
        pthread_mutex_lock(&lock);

        while (data_ready) {
            pthread_cond_wait(&cond2, &lock);
        }

        char choice;
        int file_has_data = 0; // Biến để kiểm tra xem file có dữ liệu hay không

        // Kiểm tra xem file có dữ liệu hay không
        FILE *file_check = fopen(filename, "r");
        if (file_check != NULL) {
            fseek(file_check, 0, SEEK_END);
            file_has_data = ftell(file_check) > 0; // Kiểm tra kích thước file
            fclose(file_check);
        }

        if (strcmp(role, "1") == 0) { // Student role
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
                    search_key[strcspn(search_key, "\n")] = '\0';
                    printf("Enter search value: ");
                    fgets(search_value, MAX_LEN, stdin);
                    search_value[strcspn(search_value, "\n")] = '\0';
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
        } else if (strcmp(role, "2") == 0) { // Teacher role
            printf("Teacher actions:\n");
            printf("Choose action (a: ADD, d: DELETE, u: UPDATE, s: SEARCH, l: LOG, c: CLEAR): ");
            scanf(" %c", &choice);
            getchar(); // Remove newline character left in buffer

            switch (choice) {
                case 'a': {
                    input_student_data(&student);

                    if (file_has_data) {
                        if (is_student_exists(filename, &student)) {
                            printf("Student with ID %s already exists. Do you want to overwrite it? (y/n): ", student.id);
                            char overwrite;
                            scanf(" %c", &overwrite);
                            getchar(); // Remove newline character left in buffer

                            if (overwrite == 'y' || overwrite == 'Y') {
                                pthread_mutex_unlock(&lock);
                                // Call overwrite function to handle the update
                                overwrite_student_data(filename, &student);
                                pthread_mutex_lock(&lock);
                                data_ready = 1;
                            } else {
                                printf("Student data not added.\n");
                                data_ready = 0; // Allow further actions
                            }
                        } else {
                            // If student does not exist, directly add new student data
                            pthread_mutex_unlock(&lock);
                            FILE *file = fopen(filename, "a");
                            if (file != NULL) {
                                fprintf(file, "| %-8s | %-16s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                                        student.id, student.name, student.dob, student.hometown,
                                        student.phone, student.major, student.class_t);
                                fprintf(file, "+----------+------------------+------------+------------+---------------+------------+------------+\n");
                                fclose(file);
                            }
                            pthread_mutex_lock(&lock);
                            data_ready = 1;
                        }
                    } else {
                        // If file is empty, directly add new student data
                        pthread_mutex_unlock(&lock);
                        FILE *file = fopen(filename, "a");
                        if (file != NULL) {
                            fprintf(file, "| %-8s | %-16s | %-10s | %-10s | %-13s | %-10s | %-10s |\n",
                                    student.id, student.name, student.dob, student.hometown,
                                    student.phone, student.major, student.class_t);
                            fprintf(file, "+----------+------------------+------------+------------+---------------+------------+------------+\n");
                            fclose(file);
                        }
                        pthread_mutex_lock(&lock);
                        data_ready = 1;
                    }
                    break;
                }
                case 'd': {
                    char id[9];
                    printf("Enter student ID (MSSV) to delete: ");
                    fgets(id, sizeof(id), stdin);
                    id[strcspn(id, "\n")] = '\0'; // Remove newline character
                    delete_student_data(filename, id);
                    break;
                }
                case 'u': {
                    char id[9];
                    Student new_student;
                    printf("Enter student ID (MSSV) to update: ");
                    fgets(id, sizeof(id), stdin);
                    id[strcspn(id, "\n")] = '\0'; // Remove newline character
                    input_student_data(&new_student);
                    update_student_data(filename, id, &new_student);
                    break;
                }
                case 's': {
                    char search_key[MAX_LEN];
                    char search_value[MAX_LEN];
                    printf("Enter search key (MSSV, NAME, DOB, HOMETOWN, PHONE, MAJOR, CLASS): ");
                    fgets(search_key, MAX_LEN, stdin);
                    search_key[strcspn(search_key, "\n")] = '\0'; // Remove newline character
                    printf("Enter search value: ");
                    fgets(search_value, MAX_LEN, stdin);
                    search_value[strcspn(search_value, "\n")] = '\0'; // Remove newline character
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

        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
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
