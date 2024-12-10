#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user_get.h"

#define RETURN_MSG "Prima qualquer tecla para retornar ao menu."
#define USER_INPUT 255

User app_user;
Users *users;

typedef struct {
	char tecla;
	void (*funcao)();
	const char *descricao;
} Menu_Option;

void clear_Screen(){ system("clear");};

void return_To_Menu(){
	printf("%s \n", RETURN_MSG);
	getchar();
}

void opcaoU( const char *args ) {
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

void opcaoZ( const char *args ) {
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

void opcaoP( const char *args ) {
	clear_Screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *category = strtok(inner_args, " ");
	char *order = strtok(NULL, " ");
	
    printf("Você escolheu a opção P.\n Categoria: %s\n Ordenacao: %s\n", category, order);
	return_To_Menu();
}

void opcaoC( const char *args ) {
	clear_Screen();
    printf("Você escolheu a opção C.\n");
	return_To_Menu();
}
void opcaoO( const char *args ) {
	clear_Screen();
	
	char inner_args[USER_INPUT];
	strcpy(inner_args, args);
	char *product = strtok(inner_args, " ");
	char *quantity = strtok(NULL, " ");
	int qty = atoi(quantity); 
	
    printf("[O] Você escolheu acrescentar %d %s ao carrinho.\n", qty, product);
	return_To_Menu();
}
void opcaoF( const char *args ) {
	clear_Screen();
    printf("Você escolheu a opção F.\n");
	return_To_Menu();
}
void opcaoM( const char *args ) {
	clear_Screen();
    printf("Adicionar funcionalidade\n");
}
void opcaoT( const char *args ) {
	clear_Screen();
	free_users(users);
    printf("Terminando o programa...\n");
}

int main(){


    Menu_Option menu[] = {
        {'U', opcaoU, "Listar todos os utilizadores"},
        {'Z', opcaoZ, "Seleccionar utilizador \'Z <identificador>\'"},
        {'P', opcaoP, "Listar os produtos da categoria indicada segundo o critério de \n"
						"\t ordenação indicado. Se a categoria não for reconhecida lista \n"
						"\t todos os produtos. Os critérios possíveis são “preço \n"
						"\t crescente” ou “preço decrescente”.\n"
						"\t Utilize os sinais < e > para indicar."},
        {'C', opcaoC, "Listar os produtos que estão no carrinho de compras"},
        {'O', opcaoO, "Acrescentar um produto ao carrinho de compras \'O <produto> <quantidade>\'"},        
		{'F', opcaoF, "Finalizar compra"},
		{'M', opcaoM, "Adicionar funcionalidade <Tecla Opcao> <nome funcao> <descricao>"},
        {'T', opcaoT, "Terminar Programa"},
    };

	int menuItens = sizeof(menu) / sizeof(menu[0]);

	char input[USER_INPUT];
	char arguments[USER_INPUT];
	char selected_Option;

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
