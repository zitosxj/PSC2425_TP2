/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#ifndef PRODUCTS_GET_H
#define PRODUCTS_GET_H

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

Products *products_get();
void products_free(Products *products);

#endif // PRODUCTS_GET_H
