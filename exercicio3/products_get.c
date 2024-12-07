/* *************************************************************************************
 * PSC 2425i - Grupo 3 - Exercício 3 (products_get)
 * Build: 20241206
 * Test: 20241206
 * *************************************************************************************/
 
#include <stdio.h>
#include <curl/curl.h>
#include <jansson.h>
#include <string.h>
#include "../exercicio1/http_get_json.h"

#define PRODUCTS_URI 					"https://dummyjson.com/products?limit=0&select=price,description,category"
#define PRODUCTS_TOTAL_KEY				"total"
#define PRODUCTS_KEY					"products"


typedef struct {
	int id;
	float price;
	const char *description;
	const char *category;
} Product;

typedef struct {
	Product *product;
	size_t size;
} Products;


/* *************************************************************************************
 * Função: Libertar a memória alocada de Products
 * Entrada:	
 *     *products: ponteiro para variável tipo Struct Products 
 * *************************************************************************************/
void products_free(Products *products) {
	for (int i = 0; i < products->size; i++) {
		free((void *)products->product[i].description);
		free((void *)products->product[i].category);
	}
	free(products->product);
	free(products);
}


/* *************************************************************************************
 * Função: Obter todos os produtos DummyJSON
 * Retorno:	
 *     *products: ponteiro para Struct Products criada em memória
 * Obs: retorna NULL em caso de erro 
 * *************************************************************************************/
Products *products_get() {
	Products *products = (Products *)malloc(sizeof(Products));
	
	json_t *all_products = http_get_json(PRODUCTS_URI);
	products->size = json_integer_value(json_object_get(all_products, PRODUCTS_TOTAL_KEY));
	products->product = (Product *)malloc(products->size * sizeof(Product));
	
	json_t *list_products = json_object_get(all_products, PRODUCTS_KEY);
	if (!json_is_array(list_products)) {
        fprintf(stderr, "Erro: 'list_products' não é um array\n");
        json_decref(all_products);
        json_decref(list_products);
        free(products);
        return NULL;
    }

	size_t index;
    json_t *product;
    size_t size_str;
    
    json_array_foreach(list_products, index, product) {
		products->product[index].id = json_integer_value(json_object_get(product, "id"));
		products->product[index].price = (float)json_real_value(json_object_get(product, "price"));
		
		size_str = strlen(json_string_value(json_object_get(product, "description")));
		products->product[index].description = (char *)malloc((size_str + 1) * sizeof(char));
		strcpy((void *)products->product[index].description, json_string_value(json_object_get(product, "description")));
		
		size_str = strlen(json_string_value(json_object_get(product, "category")));
		products->product[index].category = (char *)malloc((size_str + 1) * sizeof(char));
		strcpy((void *)products->product[index].category, json_string_value(json_object_get(product, "category")));
    }
		
	json_decref(all_products);
	
	return products;
}
			


/* *************************************************************************************
 * Função: Tratamento dos erros detetados
 * Entrada:	
 *     err: código do erro 
 * Obs: erros não listados são identificaados como "Não especificado" 
 * *************************************************************************************/
//void errors (int err) {
	//switch (err) {
		//case 1:
			//printf("Erro: Na abertura do ficheiro\n");
			//break;
		//case 2:
			//printf("Erro: Na escrita do ficheiro\n");
			//break;
		//case 3:
			//printf("Erro: No fecho do ficheiro\n");
			//break;
		//default:
			//printf("Erro: Não especificado\n");
			//break;
	//}
//}

/* *************************************************************************************
 * Função: Exporta os produtos para um ficheiro no formato CSV
 * Entrada:	
 *     *products: Ponteiro para struct a exporta
 * 		fileout: Ponteiro para..	 
 * Obs: erros não listados são identificaados como "Não especificado" 
 * *************************************************************************************/

//int products_to_file(Products *products, FILE *fileout) {
	//for (int i = 0; i < products->size; i++) {
		////if (fprintf(fileout, "%d;%.2f;%s;%s\n", products->product[i].id, 
												////products->product[i].price, 
												////products->product[i].description, 
												////products->product[i].category) < 0) return -1;
		//fprintf(fileout, "%d;%.2f\n", products->product[i].id, products->product[i].price);
	//}
	 //return 0;
//}		
		
		
		

//int main() {
   ////FILE *file_ptr;
   
   ////file_ptr = fopen("products.csv", "w");
   ////Products *products = products_get();
      
   ////if (file_ptr == NULL) {
       ////printf("Erro na abertura do ficheiro!");
       ////return 1;
   ////}
   ////for (int i = 0; i < 134; i++) {
		////fprintf(file_ptr, "Line 1\n");
   ////}
   ////products_free(products);
   
   ////fclose(file_ptr);
   
   
   ////return 0;
////}
	
	////FILE *fileout = fopen(PRODUCTS_FILENAME, "w");
	////if (fileout == NULL) {
		////errors(1);
		////return 1;
	////}
	////if (products_to_file(products, fileout) == -1) {
		////errors(2);
		////return 2;
	////}
	////if (fclose(fileout) == -1) {
		////errors(3);
		////return 3;
	////}
	
	//Products *products = products_get();
	
	//FILE *filecsv = fopen(PRODUCTS_FILENAME, "w");
	//for (int i = 0; i < products->size; i++) {
		////fprintf(filecsv, "%d", id);
		//fprintf(filecsv, "%d;%.2f;%s;%s\n", products->product[i].id, products->product[i].price, products->product[i].description, products->product[i].category);
	//}
	//fclose(filecsv);
	
	//products_free(products); 
//}			
	////return 0;
	
	
	
	////Products *p = products_get();
	
	////printf("Tamanho Total do array %zu\n", p->size);
	
	////for (int i = 0; i < p->size; i++) {
		////printf("ID: %d, Price: %.2f, Description: %s, Category: %s\n", p->product[i].id, p->product[i].price, p->product[i].description, p->product[i].category);
	////}
	
	////products_free(p); 
	////return 0;
