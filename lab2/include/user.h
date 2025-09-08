#ifndef __USER_H__
#define __USER_H__

#define MAX_LENGTH 100

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char first_name[MAX_LENGTH];
    char last_name[MAX_LENGTH];
    char middle_name[MAX_LENGTH];
    char birth_date[MAX_LENGTH];
    char birth_place[MAX_LENGTH];
    char phone_number[MAX_LENGTH];
} User;

#endif // __USER_H__