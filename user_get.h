/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#ifndef USER_GET_H
#define USER_GET_H

typedef struct {
	int id;
	const char *name;
} User;

typedef struct {
    size_t count;
    User *users;
} Users;

Users *user_get();
void user_free(Users *users);

#endif // USER_GET_H
