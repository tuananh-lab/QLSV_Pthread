
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 100
#define FILE_NAME "thongtinsinhvien.txt"

// Struct to hold student information
typedef struct{
	char name[MAX_LEN];
	char dob[MAX_LEN];
	char hometown[MAX_LEN];
} Student;

// Mutex and condition variables
pthread_mutex_t lock;
pthread_cond_t cond1;
pthread_cond_t cond2;

// Flags to control thread synchronization
int data_ready = 0;
int file_written = 0;

//Function prototypes
void* thread1_func(void* arg);
void* thread2_func(void* arg);
void* thread3_func(void* arg);

Student student;
int main(){
	pthread_t thread1, thread2, thread3;

	// Initialize mutex and condition variables
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond1, NULL);
	pthread_cond_init(&cond2, NULL);

	// Create threads
	pthread_create(&thread1, NULL, thread1_func, NULL);
	pthread_create(&thread2, NULL, thread2_func, NULL);
	pthread_create(&thread3, NULL, thread3_func, NULL);

	// Wait for threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	// Destroy mutex and condition variables
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);

	return 0;
}

void* thread1_func(void* arg) {
	
	while(1){
		pthread_mutex_lock(&lock);

		// Wait for thread 3 to signal that it has finished reading and printing
		while(data_ready){
			pthread_cond_wait(&cond2, &lock);
		}

		// Input student data
		printf("Nhap ho ten sinh vien:\n");
		printf("Ho ten: ");
		fgets(student.name, MAX_LEN, stdin);
		student.name[strcspn(student.name, "\n")] = '\0';

		printf("Ngay sinh:");
		fgets(student.dob, MAX_LEN, stdin);
		student.dob[strcspn(student.dob, "\n")] = '\0';

		printf("Que quan:");
		fgets(student.hometown, MAX_LEN, stdin);
		student.hometown[strcspn(student.hometown, "\n")] = '\0';

		data_ready = 1;
		pthread_cond_signal(&cond1);// Signal thread 2 to write data to file

		pthread_mutex_unlock(&lock);

		//Sleep for a short duration to simulate real-time data entry
		sleep(1);
	}
}

void* thread2_func(void* arg){
	FILE* file;
	while(1){
		pthread_mutex_lock(&lock);

		// Wait for thread 1 to signal that data is ready
		while(!data_ready){
			pthread_cond_wait(&cond1, &lock);
		}

		// Open file in append mode
		file = fopen(FILE_NAME, "a");
		if(file == NULL){
			perror("Failed to open file");
			exit(EXIT_FAILURE);
		}

		// Write student data to file
		fprintf(file, "%s	%s	%s\n", student.name, student.dob, student.hometown);
		fclose(file);

		file_written = 1;
		pthread_cond_signal(&cond2); // Signal thread 3 to read data from file

		data_ready = 0;
		pthread_mutex_unlock(&lock);

		// Sleep for a short duration to simulate real-time file writing
		sleep(1);
	}

	return NULL;
}

void* thread3_func(void* arg){
	FILE* file;
	char buffer[MAX_LEN];

	while(1){
		pthread_mutex_lock(&lock);

		// Wait for thread 2 to signal that file is written
		while (!file_written){
			pthread_cond_wait(&cond2, &lock);
		}

		// Open file in read mode 
		file = fopen(FILE_NAME, "r");
		if (file == NULL){
			perror("Failed to open file");
			exit(EXIT_FAILURE);
		}

		// Read and print student data from file
		printf("Thong tin sinh vien:\n");
		while (fgets(buffer, MAX_LEN, file) != NULL){
			printf("%s", buffer);
		}
		fclose(file);
		file_written = 0;
		pthread_cond_signal(&cond1);// Signal thread 1 to input more data

		pthread_mutex_unlock(&lock);

		// Sleep for a short duration to simulate real-time data reading
		sleep(1);
	}

	return NULL;
}

