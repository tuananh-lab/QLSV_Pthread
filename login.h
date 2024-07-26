#ifndef LOGIN_H
#define LOGIN_H

int authenticate_user(const char *username, const char *password, const char *file);
void get_user_role(char *role, size_t size);
int authenticate_teacher(const char *username, const char *password);
int authenticate_student(const char *username, const char *password);
#endif // LOGIN_H
