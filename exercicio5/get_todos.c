#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_get_json.c"

#define RETURN_MSG 				"Prima ENTER para retornar ao menu."
#define TODOS_URI 				"https://dummyjson.com/todos?limit=0"
#define TODOS_KEY				"todos"
#define TODOS_SIZE				"total"
#define TODO_ID					"id"
#define TODO_DESCRIPTION		"todo"
#define TODO_COMPLETION			"completed"
#define TODO_USERID				"userId"


typedef struct {
int id;
const char *description;
int completed;
int user_id;
} Todo;
 
Todo *extract_todo(json_t *todos_data);

void get_todos(const char *args){
	system("clear");

	char inner_args[255];
	strcpy(inner_args, args);
	char *criteria = strtok(inner_args, " ");

	if (criteria == NULL || (strcmp(criteria, "0") != 0 && strcmp(criteria, "1") != 0)) {
		printf("Utilizacao: <tecla opcao> <criterio> (0 - incompletos, 1 - completos)\n");
		printf("%s \n", RETURN_MSG);
		getchar();
	return;
	}

	int completion_criteria = (strcmp(criteria, "1") == 0) ? 1 : 0;
	
	printf("Criterio: %s (Imprimir tarefas %s)\n", criteria, completion_criteria ? "completas" : "incompletas");


	size_t processed_todos = 0;
	json_t *todos_data;
/******************** GET DATA FROM API ******************************/
	json_t *get_response_json = http_get_json(TODOS_URI);
	
	if (get_response_json == NULL)
			goto on_error;
/**********************************************************************/
	
	int r = json_unpack(get_response_json, "{s:o}", 
			TODOS_KEY, &todos_data);

	if (r != 0){
		goto on_error;
	}

	Todo *todo = extract_todo(todos_data);
		
	while(todo != NULL){
		if ( completion_criteria == todo->completed )
			printf("Todo: %d\tUser: %d\n\tDesc: %s\n", todo->id, todo->user_id, todo->description);

		free(todo); 
		todo = extract_todo(NULL);
	}

on_error:
	json_decref(get_response_json);
	
	printf("%s \n", RETURN_MSG);
	getchar();
}

Todo *extract_todo(json_t *todos_data) {

	char *todo_description = NULL;
	char *is_completed = NULL;
	
	static json_t *st_todos_array = NULL;
	static int idx = 0;
	if (todos_data != NULL) {
		st_todos_array = todos_data;
		idx = 0;
	}

	json_t *json_todo = json_array_get(st_todos_array, idx);
	if (json_todo == NULL) 
		return NULL;

	idx += 1;

	Todo *todo = (Todo*)malloc(sizeof(Todo));

	int test;
	int r = json_unpack(json_todo, "{s:i, s:s, s:b, s:i}", 
		TODO_ID, &todo->id,
		TODO_DESCRIPTION, &todo->description,
		TODO_COMPLETION, &todo->completed,
		TODO_USERID, &todo->user_id
		);

	if (r != 0)
		return NULL;

	return todo;
}
