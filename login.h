#ifndef LOGIN_H
#define LOGIN_H

int authenticate_user(const char *username, const char *password);
void get_user_role(char *role, size_t size);

#endif // LOGIN_H
