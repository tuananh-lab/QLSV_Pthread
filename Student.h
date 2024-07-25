#ifndef STUDENT_H
#define STUDENT_H

#define MAX_LEN 100

// Struct to hold student information
typedef struct {
    char id[9];          // MSSV (8 digits)
    char name[MAX_LEN];
    char dob[MAX_LEN];
    char hometown[MAX_LEN];
    char phone[MAX_LEN];
    char major[MAX_LEN];
    char class_t[MAX_LEN];
} Student;

// Function prototypes
void input_student_data(Student *student);
void write_student_to_file(Student *student, const char *filename);
void read_and_print_student_data(const char *filename);

#endif // STUDENT_H
