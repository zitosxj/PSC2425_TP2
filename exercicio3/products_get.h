#ifndef PRODUCTS_GET_H
#define PRODUCTS_GET_H

#include <curl/curl.h>
#include <jansson.h>

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

void products_free(Products *products);
Products *products_get();

#endif // PRODUCTS_GET_H
