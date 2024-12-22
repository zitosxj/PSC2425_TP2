#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_json.c"

#define RETURN_MSG 				"Prima ENTER para retornar ao menu."
#define CATEGORIES_URI 			"https://dummyjson.com/products/category-list"


void get_cart_categories(const char *args){
	system("clear");


	json_t *categories_data;
/******************** GET DATA FROM API ******************************/
	json_t *get_response_json = http_get_json(CATEGORIES_URI);
	
	if (get_response_json == NULL)
			goto on_error;
/**********************************************************************/
	size_t index;
	json_t *value;

	printf("Lista de categorias:\n");
	
	json_array_foreach(get_response_json, index, value) {
		printf("Categoria [%zu]: %s\n", index, json_string_value(value));
	}

on_error:
	json_decref(get_response_json);
	
	printf("%s \n", RETURN_MSG);
	getchar();
}
