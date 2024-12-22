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

int truncate_text(char *text, size_t text_size) {
	char *ptext = text;
	
	//printf("strlen = %d, Text_size = %d\n", (int)strlen(ptext), (int)text_size);
	
	if (strlen(ptext) > text_size) {
        ptext[text_size] = '\0';
        printf("strlen = %d, Text_size = %d, Text = %s\n", (int)strlen(ptext), (int)text_size, ptext); 
        return 1;
    }
    
    return 0;
}
	

int alignment_text (char *text, size_t text_size, char a) {
	int aux = (int) strlen(text), i = 0;
	
	// Verifica se a string de retorno tem capacidade
	if ((aux + 1) > text_size) return 1;
	else {
		// Se text_size = aux, não é necessário identar, a string ocupa a totalidade de caracteres
		if ((text_size-1) > (aux)) {
			switch (a) {
				case 'l':
					for (i = aux; i < (text_size -1); i++) 
						text[i] = ' ';
					text[i] = '\0';
					break;
				case 'r':
					text[text_size - 1] = '\0';
					for (i = (text_size - 2); i >= 0; i--)
						if (aux > 0) {
							text[i] = text[aux - 1];
							aux--;
						} else text[i] = ' ';
					break;
			}
		}
	}
	aux = (int)strlen(text);
	
	return 0;
}

void write_separator(int col, size_t col_dim[], FILE *fileout){
	for (int i = 0; i < col; i++) {
		fputc('+', fileout);
		for (int j = 1; j <= (col_dim[i] + 2); j++) fputc('-', fileout);
	}
	fputc('+', fileout);	
	fputc('\n', fileout);
}

void write_text(int col, size_t col_dim[], char *text[], char a[], FILE *fileout){
	for (int i = 0; i < col; i++) {
		truncate_text(text[i], col_dim[i]);
		//alignment_text(text[i], col_dim[i], a[i]);
		//fprintf(fileout, "| %s ", text[i]);
	}
	//fprintf(fileout, "|\n");

}
	
// Testar a exportação dos produtos e utilizador, importação carrinho de compras
int main() {
	int col=4;
	size_t col_dim[4] = {7, 11, 16, 128};
	char al[4] = {'r', 'r', 'l', 'l'};
	char *text[4] = {	"192",
					"10999,99",
					"womens-watches",
					"The Rolex Datejust Women's watch is an iconic timepiece designed for women. With a timeless design and a date complication, it offers both elegance and functionality."};
	
	Products *products = products_get();
	Users *users = user_get();
	
	write_separator(col, col_dim, stdout);
	write_text(col, col_dim, text, al, stdout);
	
	products_free(products); 
	user_free(users); 
	
	
	return 0;
}
