#include "user_get.h"
#include "../exercicio1/http_get_json.h"
#include <jansson.h>
#include <stdio.h>
#include <string.h>

//TODO Remove limit= 2
User *extract_user(json_t * users_data);
char *get_full_name(char *firstName, char *lastName);

//#define USERS_URI "https://dummyjson.com/users?limit=0&select=id,username,firstName,lastName"
#define REQ_LIMIT				30
#define USERS_KEY				"users"
#define USERS_SIZE				"total"
#define USER_ID_KEY				"id"
#define USER_FIRST_NAME_KEY		"firstName"
#define USER_LAST_NAME_KEY		"lastName"


//Process get request 
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

		//printf("%s\n", json_dumps(get_reponse_json, JSON_INDENT(4)));

		int r = json_unpack(get_reponse_json, "{s:o}", 
			USERS_KEY, &users_data);

		if (r != 0){
			free(users);
			goto on_error;
		}

		User *user = extract_user(users_data);
		
		while(user != NULL){
//			printf("\tUser Name: %s\n", user->name); //Just testing, add to Users here??

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

	user->id = json_integer_value(
		json_object_get(json_user, USER_ID_KEY)
	);

	char * full_name = get_full_name(firstName, lastName);
	user->name = full_name;

//	free(full_name);

	return user;
}

char * get_full_name(char *firstName, char *lastName){
	
	size_t str_len = strlen(firstName) + strlen(lastName) + 2;
	char *full_name = (char *)malloc(str_len);	

	snprintf(full_name, str_len, "%s %s", firstName, lastName);

	return full_name;
}


void free_users(Users *users) {
    if (users) {
        if (users->users) {
            for (size_t i = 0; i < users->count; i++) {
                free(users->users[i].name);
            }
            free(users->users);        }
        free(users);
    }
}


//  printf("%s\n", json_dumps(get_reponse_json, JSON_INDENT(4)));
//	printf("%s\n", json_dumps(users_data, JSON_INDENT(4)));
