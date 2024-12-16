/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "user_get.h"
#include "http_get_json.h"

#define REQ_LIMIT				0
#define USERS_KEY				"users"
#define USERS_SIZE				"total"
#define USER_ID_KEY				"id"
#define USER_FIRST_NAME_KEY		"firstName"
#define USER_LAST_NAME_KEY		"lastName"


// Obtém o nome completo a partir nome + apelido
char *get_full_name(char *firstName, char *lastName){
	
	size_t str_len = strlen(firstName) + strlen(lastName) + 2;
	char *full_name = (char *)malloc(str_len);	

	snprintf(full_name, str_len, "%s %s", firstName, lastName);

	return full_name;
}


// Obtém um utilizador da lista
User *extract_user(json_t *users_data) {

	char *firstName = NULL;
	char *lastName = NULL;
	
	static json_t *st_users_array = NULL;
	static int idx = 0;
	if (users_data != NULL) {
		st_users_array = users_data;
		idx = 0;
	}

	json_t *json_user = json_array_get(st_users_array, idx);
	if (json_user == NULL) 
		return NULL;

	idx += 1;

	User *user = (User*)malloc(
		sizeof(User));

	int r = json_unpack(json_user, "{s:i, s:s, s:s}", 
		USER_ID_KEY, &user->id,
		USER_FIRST_NAME_KEY, &firstName,
		USER_LAST_NAME_KEY, &lastName
		 );

	if (r != 0)
		return NULL;

	user->id = json_integer_value(json_object_get(json_user, USER_ID_KEY));

	char *full_name = get_full_name(firstName, lastName);
	user->name = full_name;

	return user;
}


// Obtém a lista de Users
Users *user_get(){
	char url[256];
	
	size_t processed_users = 0;

	Users *users = (Users *)malloc(sizeof(Users));
	users->count = 0;
	users->users = NULL;

	json_t *users_data;
	json_t *get_reponse_json;
	
	do {
		snprintf(url, sizeof(url), "https://dummyjson.com/users?limit=%d&skip=%zu&select=id,username,firstName,lastName", REQ_LIMIT, processed_users);
		get_reponse_json = http_get_json(url);
		
		if (get_reponse_json == NULL)
			goto on_error;
		
		if (users->count == 0 && json_object_get(get_reponse_json, USERS_SIZE) != 0){
			users->count = json_integer_value(
				json_object_get(get_reponse_json, USERS_SIZE));
			users->users = (User *)malloc(users->count * sizeof(User));
		}

		int r = json_unpack(get_reponse_json, "{s:o}", 
			USERS_KEY, &users_data);

		if (r != 0){
			free(users);
			goto on_error;
		}

		User *user = extract_user(users_data);
		
		while(user != NULL){
			users->users[processed_users] = *user;

			processed_users ++;
			free(user); 
			user = extract_user(NULL);
		}
		
	} while (processed_users < users->count);

on_error:
	json_decref(get_reponse_json);
	return users;
}


// Liberta o espaço de memória alocado user_get()
void user_free(Users *users) {
	for (int i = 0; i < users->count; i++) {
		free((void *)users->users[i].name);
	}
	free(users->users);
	free(users);
}
