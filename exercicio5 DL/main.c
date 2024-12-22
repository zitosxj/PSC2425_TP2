/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "api_json.h"

// DummyJSON
#define PRODUCTS_ALL_URI 				"https://dummyjson.com/products?limit=0&select=price,description,category"
#define USERS_ALL_URI 					"https://dummyjson.com/users?limit=0&select=id,username,firstName,lastName"
#define URI_STR							256

// Terminal
#define DISPLAY_COL						100
#define DISPLAY_LINE					24
#define ELEMENTS_PER_PAGE				((DISPLAY_LINE - 5)/2)

// Utilizadores
#define USER_ID_DIM 					6
#define USER_NAME_DIM 					40
#define USER_COL						2
#define USER_ID_TITLE					"  ID  "
#define USER_NAME_TITLE					"                  NOME                  "

size_t user_col_dim[2] = {USER_ID_DIM, USER_NAME_DIM};

// Produtos
#define PRODUCTS_ALL_URI 				"https://dummyjson.com/products?limit=0&select=price,description,category"
#define PRODUCT_ID_DIM 					6
#define PRODUCT_PRICE_DIM 				11
#define PRODUCT_CATEGORY_DIM 			18
#define PRODUCT_DESCRIPTION_DIM 		50
#define PRODUCT_COL						4
#define PRODUCT_ID_TITLE				"  ID  "
#define PRODUCT_PRICE_TITLE				"   PREÇO   "
#define PRODUCT_CATEGORY_TITLE			"    CATEGORIA     "
#define PRODUCT_DESCRIPTION_TITLE		"                    DESCRIÇÃO                     "

size_t product_col_dim[4] = {PRODUCT_ID_DIM, PRODUCT_PRICE_DIM, PRODUCT_CATEGORY_DIM, PRODUCT_DESCRIPTION_DIM};

// variáveis Gerais
Users *users;
Products *products; 


//#define PRODUCTS_FILENAME				"products.csv"
//#define USERS_FILENAME					"users.csv"
//#define CART_FILENAME					"cart.csv"
//#define MAX_LINE 						100
//#define MAX_COLUMNS 					3
//#define MAX_USERID 						6
//#define MAX_PRODUCTID 					6
//#define MAX_PRODUCTQT 					6


/* ********************************************************************
 *                      Funções auxiliares listagens  
 * ********************************************************************/
// Altera o stdin para modo non canonical
void set_noncanonical_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Altera o stdin para modo canonical
void reset_canonical_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Identa o texto
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


// Escreve Separador
void write_separator(int col, size_t col_dim[]){
	for (int i = 0; i < col; i++) {
		printf("+");
		for (int j = 1; j <= (col_dim[i] + 2); j++) printf("-");
	}
	printf("+\n");	
}


// Escreve o Texto 
void write_text(int col, char *text[]){
	for (int i = 0; i < col; i++) {
		//alignment_text(text[i], col_dim[i], a[i]);
		printf("| %s ", text[i]);
	}
	printf("|\n");

}

// Limpa o stdout "linux"
void clear_screen(){
	system("clear");
}


/* ********************************************************************
 *                       Listar utilizadores  
 * ********************************************************************/

// Envia para stdout a lista os utilizadores
int users_view(Users *users) {
	char *text[2];
	char op;
	int aux = ELEMENTS_PER_PAGE, idx = 0;
	
	text[0] = malloc(sizeof(char) * (USER_ID_DIM + 1));
	text[1] = malloc(sizeof(char) * (USER_NAME_DIM + 1));
	do {
		system("clear");
		set_noncanonical_mode();
		strcpy(text[0], USER_ID_TITLE);
		strcpy(text[1], USER_NAME_TITLE);
		write_separator(USER_COL, user_col_dim);
		write_text(USER_COL, text);
		if ((idx + aux) > users->count) aux = users->count-idx;
		for (size_t i = 0; i < aux; i++) {
			write_separator(USER_COL, user_col_dim);
			sprintf(text[0], "%d", users->users[i+idx].id);
			if (strlen(users->users[i+idx].name) > USER_NAME_DIM) {
				strncpy(text[1], users->users[i+idx].name, USER_NAME_DIM -2);
				text[1][USER_NAME_DIM - 1] = '>';
				text[1][USER_NAME_DIM] = '\0';
				strcat(text[1], "...");
			} else strcpy(text[1], users->users[i+idx].name);
			alignment_text (text[0], USER_ID_DIM + 1, 'r');
			alignment_text (text[1], USER_NAME_DIM + 1, 'l');
			write_text(USER_COL, text);
		}
		write_separator(USER_COL, user_col_dim);
		printf("\nPrima qualquer tecla para continuar, <Esc> para terminar");
		op = (char)getchar();
		idx += aux;
	} while ((op != 27) && (users->count > idx));
	reset_canonical_mode();
	if (op == 27) return 0;
	else return 1;
}

// Lista todos os utilizadores
void users_list() {
	users = user_get(USERS_ALL_URI);
	users_view(users);
}



/* ********************************************************************
 *                       Listar produtos  
 * ********************************************************************/

// Lista os produtos
int products_view(Products *products) {
	char *text[4];
	char op;
	int aux = ELEMENTS_PER_PAGE, idx = 0;
	
	text[0] = malloc(sizeof(char) * (PRODUCT_ID_DIM + 1));
	text[1] = malloc(sizeof(char) * (PRODUCT_PRICE_DIM + 1));
	text[2] = malloc(sizeof(char) * (PRODUCT_CATEGORY_DIM + 1));
	text[3] = malloc(sizeof(char) * (PRODUCT_DESCRIPTION_DIM + 1));
	
	do {
		system("clear");
		set_noncanonical_mode();
		strcpy(text[0], PRODUCT_ID_TITLE);
		strcpy(text[1], PRODUCT_PRICE_TITLE);
		strcpy(text[2], PRODUCT_CATEGORY_TITLE);
		strcpy(text[3], PRODUCT_DESCRIPTION_TITLE);
		write_separator(PRODUCT_COL, product_col_dim);
		write_text(PRODUCT_COL, text);
		if ((idx + aux) > products->size) aux = products->size-idx;
		for (size_t i = 0; i < aux; i++) {
			write_separator(PRODUCT_COL, product_col_dim);
			sprintf(text[0], "%d", products->product[i+idx].id);
			sprintf(text[1], "%.2f", products->product[i+idx].price);
			if (strlen(products-> product[i+idx].category) > PRODUCT_CATEGORY_DIM) {
				strncpy(text[2], products->product[i+idx].category, PRODUCT_CATEGORY_DIM - 2);
				text[2][PRODUCT_CATEGORY_DIM - 1] = '>';
				text[2][PRODUCT_CATEGORY_DIM] = '\0';
			} else strcpy(text[2], products->product[i+idx].category);
			if (strlen(products-> product[i+idx].description) > PRODUCT_DESCRIPTION_DIM) {
				strncpy(text[3], products->product[i+idx].description, PRODUCT_DESCRIPTION_DIM - 2);
				text[3][PRODUCT_DESCRIPTION_DIM - 1] = '>';
				text[3][PRODUCT_DESCRIPTION_DIM] = '\0';
			} else strcpy(text[3], products->product[i+idx].description);
			alignment_text (text[0], PRODUCT_ID_DIM + 1, 'r');
			alignment_text (text[1], PRODUCT_PRICE_DIM + 1, 'r');
			alignment_text (text[2], PRODUCT_CATEGORY_DIM + 1, 'l');
			alignment_text (text[3], PRODUCT_DESCRIPTION_DIM + 1, 'l');
			write_text(PRODUCT_COL, text);
		}
		write_separator(PRODUCT_COL, product_col_dim);
		printf("\nPrima qualquer tecla para continuar, <Esc> para terminar");
		op = (char)getchar();
		idx += aux;
	} while ((op != 27) && (products->size > idx));
	reset_canonical_mode();
	if (op == 27) return 0;
	else return 1;
}


void products_list (const char *category, const char *sort) {
	//char uri[URI_STR];
	//snprintf(uri, sizeof(uri), "https://dummyjson.com/products/category/%s?limit=0?sortBy=price&order=%s&select=price,description,category", category, sort);
	//printf("\n%s\n", uri);
	//Products *products = products_get(PRODUCTS_ALL_URI);
	
	//products_view(products);
	//products_free(products); 
	//printf("Função\nCategory: %s\nSort:%s", *args[0], a*rgs[1]);
	
	//char inner_args[USER_INPUT];
	//strcpy(inner_args, args);
	//char *category = strtok(inner_args, " ");
	//char *order = strtok(NULL, " ");
	
    //printf("Você escolheu a opção P.\n Categoria: %s\n Ordenacao: %s\n", category, order);
	//return_To_Menu()
}
	

	
// Testar subfunções dos comandos
int main() {
	users_list();
	user_free(users); 
	//products_list("smartphones", "dsc");
	
	return 0;
}
