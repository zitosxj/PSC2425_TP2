/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "http_get_json.h"
#include "products_get.h"

#define PRODUCTS_URI 					"https://dummyjson.com/products?limit=0&select=price,description,category"
#define PRODUCTS_TOTAL_KEY				"total"
#define PRODUCTS_KEY					"products"


// Obter todos os produtos do site
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


//  Libertar a memória alocada de Products
void products_free(Products *products) {
	for (int i = 0; i < products->size; i++) {
		free((void *)products->product[i].description);
		free((void *)products->product[i].category);
	}
	free(products->product);
	free(products);
}
