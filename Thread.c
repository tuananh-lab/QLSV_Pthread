#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Student.h"
#include "Thread.h"

extern pthread_mutex_t lock;
extern pthread_cond_t cond1;
extern pthread_cond_t cond2;
extern int data_ready;
extern int file_written;
extern Student student;

void* thread1_func(void* arg) {
    (void) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        
        // Wait for thread 3 to signal that it has finished reading and printing
        while (data_ready) {
            pthread_cond_wait(&cond2, &lock);
        }
        
        // Input student data
        input_student_data(&student);
  
        data_ready = 1;
        pthread_cond_signal(&cond1); // Signal thread 2 to write data to file
  
        pthread_mutex_unlock(&lock);
  
        // Sleep for a short duration to simulate real-time data entry
        sleep(1);
    }
    return NULL;
}

void* thread2_func(void* arg) {
    (void) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        
        // Wait for thread 1 to signal that data is ready
        while (!data_ready) {
            pthread_cond_wait(&cond1, &lock);
        }
        
        // Write student data to file
        write_student_to_file(&student, "thongtinsinhvien.txt");
        
        file_written = 1;
        pthread_cond_signal(&cond2); // Signal thread 3 to read data from file
        
        data_ready = 0;
        pthread_mutex_unlock(&lock);
        
        // Sleep for a short duration to simulate real-time file writing
        sleep(1);
    }
    
    return NULL;
}

void* thread3_func(void* arg) {
    (void) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        
        // Wait for thread 2 to signal that file is written
        while (!file_written) {
            pthread_cond_wait(&cond2, &lock);
        }
        
        // Read and print student data from file
        read_and_print_student_data("thongtinsinhvien.txt");
        
        file_written = 0;
        pthread_cond_signal(&cond1); // Signal thread 1 to input more data
        
        pthread_mutex_unlock(&lock);
        
        // Sleep for a short duration to simulate real-time data reading
        sleep(1);
    }
    
    return NULL;
}
