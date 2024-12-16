/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "api_json.h"

#define PRODUCTS_FILENAME				"products.csv"
#define USERS_FILENAME					"users.csv"
#define CART_FILENAME					"cart.csv"
#define MAX_LINE 						100
#define MAX_COLUMNS 					3
#define MAX_USERID 						6
#define MAX_PRODUCTID 					6
#define MAX_PRODUCTQT 					6


// Exporta os utilizadores para um ficheiro no formato CSV
void products_to_csv(Products *products, char *filename) {
	FILE *filecsv = fopen(filename, "w");
	
	for (int i = 0; i < products->size; i++) {
		if (fprintf(filecsv, "%d;%.2f;%s;%s\n", products->product[i].id, 
											products->product[i].price, 
											products->product[i].description, 
											products->product[i].category) < 0) fprintf(stderr, "Erro na escrita no ficheiro %s\n", filename);
	}
	printf("Produtos exportados com sucesso para %s\n\n", filename);
	
	fclose(filecsv);
}


// Exporta os produtos para um ficheiro no formato CSV
void users_to_csv(Users *users, char *filename) {
	FILE *filecsv = fopen(filename, "w");
	
	for (int i = 0; i < users->count; i++) {
		if (fprintf(filecsv, "%d;%s\n", users->users[i].id, 
										users->users[i].name) < 0) fprintf(stderr, "Erro na escrita no ficheiro %s\n", filename);
	}
	printf("Utilizadores exportados com sucesso para %s\n\n", filename);
	
	fclose(filecsv);
}		


// Obtém os dados a partir de uma linha texto com formato CSV
void get_columns_data(char *line, char *data[]) {
	int aux = 0, id = 0;
	
	// Remove o caracter mudança de linha
	line[strlen(line)-1] = '\0';
	for (int i = 0; *(line + i) != '\0'; i++) {
		if (*(line + i) == ';') {
				*(line + i) = '\0';
				strcpy(data[id++], (line + aux));
				aux = i + 1;
		}
	}
	strcpy(data[id], (line + aux));
}


// Cria um carrinho de compras a partir de um ficheiro CSV
Cart *csv_to_cart(char *filename) {
	Cart *cart = malloc(sizeof(Cart));
	char line[MAX_LINE], *data[MAX_COLUMNS];
	FILE *filecsv = fopen(filename, "r");
	
	if (filecsv == NULL) {
		fprintf(stderr, "Erro na abertura do ficheiro %s\n", filename);
		free(cart);
		return NULL;
	}
	else {
		cart->n_products = 0;
		data[0] = (char *)malloc(MAX_USERID * sizeof(char));  
		data[1] = (char *)malloc(MAX_PRODUCTID * sizeof(char));  
		data[2] = (char *)malloc(MAX_PRODUCTQT * sizeof(char)); 
		cart->user_id = 0;
    
		int i = 0;
		while (fgets(line, sizeof(line), filecsv)) {
			get_columns_data(line, data);
			if (cart->user_id == 0) cart->user_id = atoi(data[0]);
			// o realloc pode devolver NULL
			cart = realloc(cart, (sizeof(Cart) + ((i + 1) * sizeof(cart->products[0]))));
			cart->products[i].id = atoi(data[1]);
			cart->products[i].quantity = (size_t)atoi(data[2]);
			cart->n_products++;
			i++;
		}
	
		printf("Carrinho de Compras importado com sucesso de %s\n", filename);
	
		free(data[0]);
		free(data[1]);
		free(data[2]);
		fclose(filecsv);
	}
	
	return cart;
}
	
	
// Testar a exportação dos produtos e utilizador, importação carrinho de compras
int main() {
	Products *products = products_get();
	if (products != NULL) printf("Leitura de Produtos de dummyjson.com com sucesso\n");
	products_to_csv(products, PRODUCTS_FILENAME);
	products_free(products); 
	
	Users *users = user_get();
	if (users != NULL) printf("Leitura de Utilizadores de dummyjson.com com sucesso\n");
	users_to_csv(users, USERS_FILENAME);
	user_free(users); 
	
	Cart *cart = csv_to_cart(CART_FILENAME);
	if (cart != NULL) {
		if (cart_put(cart) != 0) printf("Escrita de Carrinho de Compras em dummyjson.com com sucesso\n");
		cart_free(cart);
	}
	return 0;
}
