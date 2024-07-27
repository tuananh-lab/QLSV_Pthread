#ifndef TEACHER_H
#define TEACHER_H

#include "student.h" // Include student.h if you need Student struct

// Function prototypes specific to teacher actions
void input_student_data(Student *student);
void check_and_create_file(const char *filename);
void write_student_to_file(Student *student, const char *filename);
void delete_student_data(const char *filename, const char *student_id);
void update_student_data(const char *filename, const char *student_id, Student *new_data);
void log_change(const char *action, const Student *student);
void display_log();
void clear_file(const char *filename);
void clear_log(void);

#endif // TEACHER_H
