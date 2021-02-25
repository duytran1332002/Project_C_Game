#ifndef USER_MODEL_H
#define USER_MODEL_H
#include "CalendarModel.h"

typedef struct strUser {
    int id;
    char *username;
    char *password;
    char *name;
    // Date dateOfBirth;
} User;

typedef struct strAdmin {
    int id;
    char *username;
    char *password;
    char *name;
} Admin;

#endif