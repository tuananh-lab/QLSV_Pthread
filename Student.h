#ifndef STUDENT_H
#define STUDENT_H

#define MAX_LEN 100
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
void input_student_data(Student *student);
void write_student_to_file(Student *student, const char *filename);
void read_and_print_student_data(const char *filename);
void delete_student_data(const char *filename, const char *student_id);
void update_student_data(const char *filename, const char *student_id, Student *new_data);
void search_student_data(const char *filename, const char *search_key, const char *search_value);
void log_change(const char *action, const Student *student);
void display_log();
void clear_file(const char *filename);

#endif // STUDENT_H
