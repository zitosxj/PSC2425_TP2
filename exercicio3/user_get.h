#ifndef USER_GET_H
#define USER_GET_H

#include <jansson.h>


typedef struct {
int id;
const char *name;
} User;

typedef struct {
    size_t count;
    User *users;
} Users;

Users *user_get();

#endif
