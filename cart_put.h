/* ********************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * ********************************************************************/

#ifndef CART_PUT_H
#define CART_PUT_H

typedef struct {
	int user_id;
	size_t n_products;
	struct {
		int id;
		size_t quantity;
	} products[];
} Cart;

bool cart_put(Cart *cart);
void cart_free(Cart *cart);

#endif // CART_PUT_H
