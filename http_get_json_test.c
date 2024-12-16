/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#include <stdio.h>
#include <jansson.h>
#include "http_get_json.h"

#define PRODUCT1_URI 					"https://dummyjson.com/products/1"

// Testa a função http_get_json
int main() {
	char uri[] = PRODUCT1_URI;

	json_t *product_json = http_get_json(uri);
	if (product_json == NULL) printf("Erro na leitura do produto em Dummyjson.com\n");
	else printf("Leitura de produto com sucesso em Dummyjson.com\n");
	char *str_product = json_dumps(product_json, JSON_INDENT(2));
	printf("Produto:\n%s\n", str_product);
	
	free(str_product);
	json_decref(product_json);
	return 0;
}
