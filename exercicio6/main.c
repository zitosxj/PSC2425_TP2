/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <dlfcn.h>
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
#define PRODUCT_DESCRIPTION_DIM 		52
#define PRODUCT_COL						4
#define PRODUCT_ID_TITLE				"  ID  "
#define PRODUCT_PRICE_TITLE				"   PREÇO   "
#define PRODUCT_CATEGORY_TITLE			"    CATEGORIA     "
#define PRODUCT_DESCRIPTION_TITLE		"                     DESCRIÇÃO                      "

size_t product_col_dim[4] = {PRODUCT_ID_DIM, PRODUCT_PRICE_DIM, PRODUCT_CATEGORY_DIM, PRODUCT_DESCRIPTION_DIM};

// Cart
#define CART_DESCRIPTION_DIM 			73
#define CART_PRICE_DIM 					11
#define CART_QUANTITY_DIM 				6

#define CART_COL						3
#define CART_DESCRIPTION_TITLE			"                                DESCRIÇÃO                                "
#define CART_PRICE_TITLE				"   PREÇO   "
#define CART_QUANTITY_TITLE				"  QT  "

size_t cart_col_dim[3] = {CART_DESCRIPTION_DIM, CART_PRICE_DIM, CART_QUANTITY_DIM};

// variáveis Gerais
Users *users;
Products *products; 
Cart  *cart;


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
		printf("| %s ", text[i]);
	}
	printf("|\n");

}

// Limpa o stdout "linux"
void clear_screen(){
	int r = system("clear");
	(void)r;
}


/* ********************************************************************
 *                       Listar utilizadores  
 * ********************************************************************/

// Envia para stdout a lista dos utilizadores
int users_view(Users *users) {
	char *text[2];
	char op;
	int aux = ELEMENTS_PER_PAGE, idx = 0;
	
	text[0] = malloc(sizeof(char) * (USER_ID_DIM + 1));
	text[1] = malloc(sizeof(char) * (USER_NAME_DIM + 1));
	do {
		clear_screen();
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



/* ********************************************************************
 *                       Listar produtos  
 * ********************************************************************/

// Envia para stdout a lista dos produtos
int products_view(Products *products) {
	char *text[4];
	char op;
	int aux = ELEMENTS_PER_PAGE, idx = 0;
	
	text[0] = malloc(sizeof(char) * (PRODUCT_ID_DIM + 1));
	text[1] = malloc(sizeof(char) * (PRODUCT_PRICE_DIM + 1));
	text[2] = malloc(sizeof(char) * (PRODUCT_CATEGORY_DIM + 1));
	text[3] = malloc(sizeof(char) * (PRODUCT_DESCRIPTION_DIM + 1));
	
	do {
		clear_screen();
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



/* ********************************************************************
 *                       Listar carrinho compras  
 * ********************************************************************/

// Obtém um ponteiro para o produto com o id
Product *product_data(int id) {
	Product *product = NULL;
	
	for (int i = 0; (i < products->size); i++) {
		if (products->product[i].id == id) {
			product = &(products->product[i]);
			return product;
		}
	}
	return product;
}


// Envia para stdout os produtos do carrinho de compras
int cart_view(Cart *cart) {
	char *text[3];
	char op;
	int aux = ELEMENTS_PER_PAGE, idx = 0;
	Product *product_aux;
	
	text[0] = malloc(sizeof(char) * (CART_DESCRIPTION_DIM + 1));
	text[1] = malloc(sizeof(char) * (CART_PRICE_DIM + 1));
	text[2] = malloc(sizeof(char) * (CART_QUANTITY_DIM + 1));
	
	do {
		clear_screen();
		set_noncanonical_mode();
		strcpy(text[0], CART_DESCRIPTION_TITLE);
		strcpy(text[1], CART_PRICE_TITLE);
		strcpy(text[2], CART_QUANTITY_TITLE);
		write_separator(CART_COL, cart_col_dim);
		write_text(CART_COL, text);
		if ((idx + aux) > cart->n_products) aux = cart->n_products-idx;
		if (aux !=0) {
			for (size_t i = 0; i < aux; i++) {
				write_separator(CART_COL, cart_col_dim);
				product_aux = product_data(cart->products[i+idx].id);
				if (strlen(product_aux->description) > CART_DESCRIPTION_DIM) {
					strncpy(text[0], product_aux->description, CART_DESCRIPTION_DIM - 2);
					text[0][CART_DESCRIPTION_DIM - 1] = '>';
					text[0][CART_DESCRIPTION_DIM] = '\0';
				} else strcpy(text[0], product_aux->description);
				sprintf(text[1], "%.2f", product_aux->price);
				sprintf(text[2], "%d", (int) cart->products[i].quantity);
				alignment_text (text[0], CART_DESCRIPTION_DIM + 1, 'l');
				alignment_text (text[1], CART_PRICE_DIM + 1, 'r');
				alignment_text (text[2], CART_QUANTITY_DIM + 1, 'r');
				write_text(CART_COL, text);
			}
		} else write_separator(CART_COL, cart_col_dim);
		write_separator(CART_COL, cart_col_dim);
		printf("\nPrima qualquer tecla para continuar, <Esc> para terminar");
		op = (char)getchar();
		idx += aux;
	} while ((op != 27) && (cart->n_products > idx));
	reset_canonical_mode();
	if (op == 27) return 0;
	else return 1;
}


// Lista todos os produtos do cariinho de compras
//void cart_list () {
	//cart_view(cart);
//}
	

/* ********************************************************************
 *                             Comandos  
 * ********************************************************************/


#define RETURN_MSG "Prima ENTER para retornar ao menu."
#define SLEEP_TIME 1
#define USER_INPUT 255


typedef struct {
	char tecla;
	void (*funcao)();
	const char *descricao;
} Menu_Option;

Menu_Option *menu = NULL;
int menuItens = 0;
void **imported_libs = NULL; 	//referencia para as bibliotecas importadas
int lib_count = 0;				// quantidade de bibliotecas importadas 
User app_user;
Users *users;


void return_To_Menu(){
	printf("%s \n", RETURN_MSG);
	getchar();
}


Menu_Option *add_menu_item(Menu_Option *menu, int *menuItens, char key, void (*function)(), const char *description) {

	(*menuItens)++; //adiciona contador de items de menu

	menu = realloc(menu, (*menuItens) * sizeof(Menu_Option)); //estender memoria para mais um item

	// Adicionar opção ao menu
	menu[(*menuItens) - 1].tecla = key;
	menu[(*menuItens) - 1].funcao = function;
	menu[(*menuItens) - 1].descricao = strdup(description);

	return menu;
}

void free_menu(Menu_Option *menu, int menuItens) {
    printf("A remover menu itens...\n");
	sleep(SLEEP_TIME);

	//Libertar memoria alocada para os items de menu
	for (int i = 0; i < menuItens; i++) {
		if (menu[i].descricao) {
			free((void *)menu[i].descricao);
		}
	}
	free(menu);
}

// Lista todos os utilizadores
void users_list(const char *args) {
	users = user_get(USERS_ALL_URI);
	users_view(users);
}

// Associa um utiliador ao carrinho de compras
void user_select( const char *args ) {
	clear_screen();
	char inner_args[USER_INPUT];

	if (users && users->count > 0){
		strcpy(inner_args, args);

		int id = atoi(strtok(inner_args, " "));
		app_user.id = id;
		app_user.name = users->users[id - 1].name;

		printf("User: %s, Id: %d\n", app_user.name, app_user.id);
	} else 
		printf("Atualize a lista de utilizadores antes de definir o user.\n");

	return_To_Menu();
}


void products_list(const char *args) {
	clear_screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *category = strtok(inner_args, " ");
	char *order = strtok(NULL, " ");
	
    printf("Você escolheu a opção P.\n Categoria: %s\n Ordenacao: %s\n", category, order);
	return_To_Menu();
}

// Lista todos os produtos
//void products_list (const char *category, const char *order) {
	//char uri[URI_STR];
	//snprintf(uri, sizeof(uri), "https://dummyjson.com/products/category/%s?sortBy=price&order=%s&select=price,description,category", category, order);
	//products = products_get(uri);
	//if (products->size == 0) {
		//products_free(products);
		//products = products_get(PRODUCTS_ALL_URI);
	//}
	//products_view(products);	
//}


// Lista todos os produtos do carrinho de compras
void cart_list(const char *args) {
	cart_view(cart);
}


void cart_add_product( const char *args ) {
	clear_screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *product = strtok(inner_args, " ");
	char *quantity = strtok(NULL, " ");
	int qty = atoi(quantity); 
	
    printf("[O] Você escolheu acrescentar %d %s ao carrinho.\n", qty, product);
	return_To_Menu();
}

void cart_send(const char *args) {
	clear_screen();
    printf("Você escolheu a opção F.\n");
	return_To_Menu();
}


/* ********************************************************************
 *                            Plug-in  
 * ********************************************************************/


void add_external_lib( const char *args ) {
	clear_screen();
	char inner_args[USER_INPUT];
	
	
	if (args == NULL){
		printf("Parâmetros inválidos. Exemplo de uso: M <Tecla> <Nome da Funcao/Ficheiro> <Descricao>\n");
		return_To_Menu();
		return;
	}
	
	strcpy(inner_args, args);

	char * new_key = strtok(inner_args, " ");
	char * new_file_name = strtok(NULL, " ");
	char * new_description = strtok(NULL, "\0");

	if( new_key[0] >= 'a' && new_key[0] <= 'z' )
		new_key[0] -= 'a' - 'A';


	if (!new_key || strlen(new_key) != 1 || !new_file_name || !new_description) {
        printf("Parâmetros inválidos. Exemplo de uso: M <Tecla> <Nome da Funcao/Ficheiro> <Descricao>\n");
        return_To_Menu();
        return;
    }


	for (int i = 0; i < menuItens; i++){
		if(menu[i].tecla == new_key[0]){
			printf("Tecla %c já utilizada noutra opção de menu!\n", new_key[0]);
			return_To_Menu();
			return;
		}
	}

	char din_lib[USER_INPUT];
	snprintf(din_lib, sizeof(din_lib), "./lib%s.so", new_file_name);

	void *handle = dlopen(din_lib, RTLD_NOW);
		if (!handle) {
			printf("Falha ao carregar o arquivo: %s\n", dlerror());
			return_To_Menu();
			return;
		}


	lib_count++;
    imported_libs = realloc(imported_libs, lib_count * sizeof(void *));
    imported_libs[lib_count - 1] = handle;

	void (*new_function)(const char *); // = dlsym(handle, new_file_name);
	*(void **)(&new_function) = dlsym(handle, new_file_name);

	if (!new_function) {
		printf("Função não encontrada: %s\n", dlerror());

		return_To_Menu();
		return;
	}
	
	menu = add_menu_item(menu, &menuItens, new_key[0], new_function, new_description);

	printf("Nova opção de menu adicionada\n \tKey: %s, File_name: %s, Desc: %s\n", new_key, new_file_name, new_description);
		
	return_To_Menu();
}

void free_libs(){
    printf("A libertar bibliotecas importadas...\n");
    sleep(SLEEP_TIME);

	for (int i = 0; i < lib_count; i++) {
		dlclose(imported_libs[i]);
	}
	
	free(imported_libs);	
}



/* ********************************************************************
 *                          Programa Principal  
 * ********************************************************************/
 

void exit_program( const char *args ) {
	clear_screen();

    printf("Terminando o programa...\n");
	sleep(SLEEP_TIME);
	free_libs();
	sleep(SLEEP_TIME);

	user_free(users);
	free_menu(menu, menuItens);

}
 
 
int main(){
	char input[USER_INPUT];
	char arguments[USER_INPUT];
	char selected_Option;

    menu = add_menu_item(menu, &menuItens, 'U', users_list, "Listar todos os utilizadores");
    menu = add_menu_item(menu, &menuItens, 'Z', user_select, "Seleccionar utilizador 'Z <identificador>'");
    menu = add_menu_item(menu, &menuItens, 'P', products_list, 
        "Listar os produtos da categoria indicada segundo o critério de \n"
        "\t ordenação indicado. Se a categoria não for reconhecida lista \n"
        "\t todos os produtos. Os critérios possíveis são “preço \n"
        "\t crescente” ou “preço decrescente”.\n"
        "\t Utilize os sinais < e > para indicar.");
    menu = add_menu_item(menu, &menuItens, 'C', cart_list, "Listar os produtos que estão no carrinho de compras");
    menu = add_menu_item(menu, &menuItens, 'O', cart_add_product, "Acrescentar um produto ao carrinho de compras 'O <produto> <quantidade>'");
    menu = add_menu_item(menu, &menuItens, 'F', cart_send, "Finalizar compra");
	menu = add_menu_item(menu, &menuItens, 'M', add_external_lib, "Adicionar funcionalidade <Tecla Opcao> <nome da funcao/ficheiro> <descricao>");
    menu = add_menu_item(menu, &menuItens, 'T', exit_program, "Terminar Programa");

	do {
		clear_screen();
		/* Lista opções do menu */
        for (int i = 0; i < menuItens; i++) {
            printf("[%c] - %s\n", menu[i].tecla, menu[i].descricao);
        }
        printf("Escolha uma opção (ex: Z 123): ");
        //scanf(" %c", &selected_Option);

		if (!fgets(input, sizeof(input), stdin)) {
            continue; 									//le user input
        }
        
        input[strcspn(input, "\n")] = '\0'; 			//substitui \n por \0

		//Separa opcao de argumentos
		sscanf(input, " %c %[^\n]", &selected_Option, arguments);
        
        
        if( selected_Option >= 'a' && selected_Option <= 'z' )
			selected_Option -= 'a' - 'A';

		/* Pesquisa e executa as opçoes */
        int is_valid_option = 0;
        for (int i = 0; i < menuItens; i++) {
            if (menu[i].tecla == selected_Option) {
                menu[i].funcao(arguments); // Chama a função correspondente
                is_valid_option = 1;
                break;
            }
        }

        if (!is_valid_option) {
			clear_screen();
			printf("********************************************\n");
            printf("********* Escolha um opção válida. *********\n");
			printf("********************************************\n\n");
			return_To_Menu();
        }

	}while (selected_Option != 'T');
	
	return 0;
}
