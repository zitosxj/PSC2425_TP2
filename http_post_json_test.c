/* *************************************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * *************************************************************************************/

#include <stdio.h>
#include <jansson.h>
#include "http_post_json.h"


// Testa a função http_post_json
int main() {
	json_error_t error;
	json_t *cart_json = json_loads("{\"userId\":1,\"products\":"
									"[{\"id\":144,\"quantity\":4}]}", 0, &error);
	if (cart_json == NULL) {
		fprintf(stderr, "Erro na Criação do carrinho de compras, código: %d (%s)\n", error.line, error.text);
		exit(EXIT_FAILURE);
	}
	json_t *new_cart_json = http_post_json("https://dummyjson.com/carts/add", cart_json);
	if (new_cart_json == NULL) printf("Erro na Gravação do Carrinho em Dummyjson.com\n");
	else printf("Gravação do carrinho de compras em Dummyjson.com com sucesso\n");
	char *str_cart = json_dumps(new_cart_json, JSON_INDENT(2));
	printf("Carrinho de compras:\n%s\n", str_cart);
	
	free(str_cart);
	json_decref(new_cart_json);
	json_decref(cart_json);
	return 0;
}
