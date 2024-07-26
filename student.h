#ifndef STUDENT_H
#define STUDENT_H

#define MAX_LEN 256
#define FILENAME "thongtinsinhvien.txt"

// Struct to hold student information
typedef struct {
    char id[9];  
    char name[MAX_LEN];
    char dob[MAX_LEN];
    char hometown[MAX_LEN];
    char phone[MAX_LEN];
    char major[MAX_LEN];
    char class_t[MAX_LEN];
} Student;

// Function prototypes
void read_and_print_student_data(const char *filename);
void search_student_data(const char *filename, const char *search_key, const char *search_value);
int is_student_exists(const char *filename, const Student *new_student);
char* trim_whitespace(char* str);
void overwrite_student_data(const char *filename, const Student *new_student);
#endif // STUDENT_H
