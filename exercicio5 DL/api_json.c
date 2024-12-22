/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <jansson.h>
#include "http_json.h"
#include "api_json.h"


/* ********************************************************************
 *                            products_get  
 * ********************************************************************/
#define PRODUCTS_URI 					"https://dummyjson.com/products?limit=0&select=price,description,category"
#define PRODUCTS_TOTAL_KEY				"total"
#define PRODUCTS_KEY					"products"


// Obtém todos os produtos existentes no site
Products *products_get(const char *uri) {
	Products *products = (Products *)malloc(sizeof(Products));
	
	json_t *all_products = http_get_json(uri);
	products->size = json_integer_value(json_object_get(all_products, PRODUCTS_TOTAL_KEY));
	products->product = (Product *)malloc(products->size * sizeof(Product));
	
	json_t *list_products = json_object_get(all_products, PRODUCTS_KEY);
	if (!json_is_array(list_products)) {
        fprintf(stderr, "O elemento não é um array\n");
        json_decref(all_products);
        json_decref(list_products);
        free(products);
        return NULL;
    }

	size_t index;
    json_t *product;
    size_t size_str;
    
    json_array_foreach(list_products, index, product) {
		products->product[index].id = json_integer_value(json_object_get(product, "id"));
		products->product[index].price = (float)json_real_value(json_object_get(product, "price"));
		
		size_str = strlen(json_string_value(json_object_get(product, "description")));
		products->product[index].description = (char *)malloc((size_str + 1) * sizeof(char));
		strcpy((void *)products->product[index].description, json_string_value(json_object_get(product, "description")));
		
		size_str = strlen(json_string_value(json_object_get(product, "category")));
		products->product[index].category = (char *)malloc((size_str + 1) * sizeof(char));
		strcpy((void *)products->product[index].category, json_string_value(json_object_get(product, "category")));
    }
		
	json_decref(all_products);
	
	return products;
}


//  Libertar a memória alocada de "products"
void products_free(Products *products) {
	for (int i = 0; i < products->size; i++) {
		free((void *)products->product[i].description);
		free((void *)products->product[i].category);
	}
	free(products->product);
	free(products);
}


/* ********************************************************************
 *                            user_get  
 * ********************************************************************/
#define REQ_LIMIT				0
#define URI_STR					256
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

	if (r != 0) return NULL;

	user->id = json_integer_value(json_object_get(json_user, USER_ID_KEY));
	char *full_name = get_full_name(firstName, lastName);
	user->name = full_name;

	return user;
}

// Obtém todos os utilizadores existentes no site
Users *user_get(const char *uri){
	//char url[URI_STR];
	
	size_t processed_users = 0;

	Users *users = (Users *)malloc(sizeof(Users));
	users->count = 0;
	users->users = NULL;

	json_t *users_data;
	json_t *get_reponse_json;
	
	do {
		//snprintf(url, sizeof(url), "https://dummyjson.com/users?limit=%d&skip=%zu&select=id,username,firstName,lastName", REQ_LIMIT, processed_users);
		get_reponse_json = http_get_json(uri);
		
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


// Liberta o espaço de memória alocado pelo "users"
void user_free(Users *users) {
	for (int i = 0; i < users->count; i++) {
		free((void *)users->users[i].name);
	}
	free(users->users);
	free(users);
} 


/* ********************************************************************
 *                            cart_put  
 * ********************************************************************/
#define URI_CART_ADD 					"https://dummyjson.com/carts/add"
#define STR_LENGTH						100


// Envia o carrinho de compras para o site
bool cart_put(Cart *cart) {
	char str[STR_LENGTH]; 
	
	snprintf(str, STR_LENGTH, "{\"userId\":%d,\"products\":[]}", cart->user_id);
	
	json_error_t error;
	json_t *cart_json = json_loads(str, 0, &error);
	if (cart_json == NULL) {
		fprintf(stderr, "Erro na leitura JSON %d: %s\n", error.line, error.text);
		return 0;
	}
	
	json_t *products = json_object_get(cart_json, "products");
    if (!json_is_array(products)) {
        fprintf(stderr, "Erro porque JSON não é um array\n");
        json_decref(cart_json);
        return 0;
    }
		
	for (int i = 0; i < (int)cart->n_products; i++) {
		json_t *new_product = json_object();
		json_object_set_new(new_product, "id", json_integer(cart->products[i].id));
		json_object_set_new(new_product, "quantity", json_integer(cart->products[i].quantity));
		
		json_array_append_new(products, new_product);
	}
	
	//char *updated_cart = json_dumps(cart_json, JSON_INDENT(2));
    //printf("Conteúdo JSON:\n%s\n", updated_cart);
	
	json_t *new_cart_json = http_post_json(URI_CART_ADD, cart_json);
	if (new_cart_json == NULL ) {
		fprintf(stderr, "Erro na escrita do carrinho de compras\n");
		//free(updated_cart);
		json_decref(cart_json);
		return 0;
	}
	//printf("Conteúdo Escrito:\n%s\n", json_dumps(new_cart_json, JSON_INDENT(4)));
	
	//free(updated_cart);
	json_decref(new_cart_json);
	json_decref(cart_json);
	return 1;
}		
		

// Liberta o espaço de memória alocado por "cart"
void cart_free(Cart *cart) {
	free(cart);
} 

