#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include "user_get.h"

#define RETURN_MSG "Prima ENTER para retornar ao menu."
#define SLEEP_TIME 0
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


void clear_Screen(){ system("clear");};

void return_To_Menu(){
	printf("%s \n", RETURN_MSG);
	getchar();
}

Menu_Option *add_menu_item(Menu_Option *menu, int *menuItens, char key, void (*function)(), const char *description);

void users_list( const char *args );
void user_select( const char *args );
void products_list( const char *args );
void cart_list( const char *args );
void cart_add_product( const char *args );
void cart_put( const char *args );
void add_external_lib( const char *args );
void exit_program( const char *args );

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
    menu = add_menu_item(menu, &menuItens, 'F', cart_put, "Finalizar compra");
    menu = add_menu_item(menu, &menuItens, 'M', add_external_lib, "Adicionar funcionalidade <Tecla Opcao> <nome da funcao/ficheiro> <descricao>");
    menu = add_menu_item(menu, &menuItens, 'T', exit_program, "Terminar Programa");

	do {
		clear_Screen();
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
			clear_Screen();
			printf("********************************************\n");
            printf("********* Escolha um opção válida. *********\n");
			printf("********************************************\n\n");
			return_To_Menu();
        }

	}while (selected_Option != 'T');
	
	return 0;
}

/******** END OF MAIN ****************************************/


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

void users_list( const char *args ) {
	clear_Screen();

	users = user_get();

    printf("Total de users: %ld\n\n", users->count);

    for (size_t i = 0; i < users->count; i++) {
		
		if (i % 15 == 0 && i > 0){
			printf("Prima uma tecla para continuar");
			getchar();
			clear_Screen();
		}
		
        printf("Id: %d - Nome: %s\n", users->users[i].id, users->users[i].name);
    }


	return_To_Menu();
}

void user_select( const char *args ) {
	clear_Screen();
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

void products_list( const char *args ) {
	clear_Screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *category = strtok(inner_args, " ");
	char *order = strtok(NULL, " ");
	
    printf("Você escolheu a opção P.\n Categoria: %s\n Ordenacao: %s\n", category, order);
	return_To_Menu();
}

void cart_list( const char *args ) {
	clear_Screen();
    printf("Você escolheu a opção C.\n");
	return_To_Menu();
}
void cart_add_product( const char *args ) {
	clear_Screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *product = strtok(inner_args, " ");
	char *quantity = strtok(NULL, " ");
	int qty = atoi(quantity); 
	
    printf("[O] Você escolheu acrescentar %d %s ao carrinho.\n", qty, product);
	return_To_Menu();
}
void cart_put( const char *args ) {
	clear_Screen();
    printf("Você escolheu a opção F.\n");
	return_To_Menu();
}


void add_external_lib( const char *args ) {
	clear_Screen();
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

	void (*new_function)(const char *) = dlsym(handle, new_file_name);
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

void exit_program( const char *args ) {
	clear_Screen();

    printf("Terminando o programa...\n");
	sleep(SLEEP_TIME);

	free_users(users);
	free_menu(menu, menuItens);
	free_libs();
}
