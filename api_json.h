/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#ifndef API_JSON_H
#define API_JSON_H

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

typedef struct {
	int id;
	const char *name;
} User;

typedef struct {
    size_t count;
    User *users;
} Users;

typedef struct {
	int user_id;
	size_t n_products;
	struct {
		int id;
		size_t quantity;
	} products[];
} Cart;

Products *products_get();
void products_free(Products *products);

Users *user_get();
void user_free(Users *users);

bool cart_put(Cart *cart);
void cart_free(Cart *cart);

#endif // API_JSON_H
