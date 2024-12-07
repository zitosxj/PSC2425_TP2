/* *************************************************************************************
 * PSC 2425i - Grupo 3 - Exercício 3 (products_get_test)
 * Build: 20241207
 * Test: 20241207
 * *************************************************************************************/
 
#include <stdio.h>
#include "products_get.h"

#define PRODUCTS_FILENAME				"products.csv"


/* *************************************************************************************
 * Função: Tratamento dos erros detetados
 * Entrada:	
 *     err: código do erro 
 * Obs: erros não listados são identificaados como "Não especificado" 
 * *************************************************************************************/
void errors (int err) {
	switch (err) {
		case 1:
			printf("Erro: Na abertura do ficheiro\n");
			break;
		case 2:
			printf("Erro: Na escrita do ficheiro\n");
			break;
		case 3:
			printf("Erro: No fecho do ficheiro\n");
			break;
		default:
			printf("Erro: Não especificado\n");
			break;
	}
}

/* *************************************************************************************
 * Função: Exporta os produtos para um ficheiro no formato CSV
 * Entrada:	
 *     *products: Ponteiro para Products
 *     *filecsv: Ponteiro para o FILE	 
 * Retorna: 
 *     -1: se houve erros
 *      0: se concluiu com sucesso a escrita 
 * Obs: utiliza como caracter delimitador ';'
 * *************************************************************************************/
int products_to_csv(Products *products, FILE *filecsv) {
	for (int i = 0; i < products->size; i++) {
		if (fprintf(filecsv, "%d;%.2f;%s;%s\n", products->product[i].id, 
											products->product[i].price, 
											products->product[i].description, 
											products->product[i].category) < 0) return -1;
	}
		
	return 0;
}		
		
		
	
// Testar a exportação dos produtos para ficheiro CSV
int main() {
	Products *products = products_get();
	
	FILE *filecsv = fopen(PRODUCTS_FILENAME, "w");
	if (filecsv == NULL) {
		errors(1);
		return 1;
	}
	if (products_to_csv(products, filecsv) == -1) {
		errors(2);
		return 2;
	}
	if (fclose(filecsv) == -1) {
		errors(3);
		return 3;
	}
	
	products_free(products); 
}
