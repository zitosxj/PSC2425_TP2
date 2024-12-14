/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <stdbool.h>
#include <jansson.h>
#include "http_post_json.h"
#include "cart_put.h"

#define CARTS_ADD_URI 					"https://dummyjson.com/carts/add"
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
	
	json_t *new_cart_json = http_post_json("https://dummyjson.com/carts/add", cart_json);
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
