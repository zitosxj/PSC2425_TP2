#include <stdio.h>
#include <jansson.h>
#include "http_post_json.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "usage: %s <userId>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/*------------------------------------------------------------------------*/
	json_error_t error;
	json_t *cart_json = json_loads("{\"userId\":1,\"products\":"
									"[{\"id\":144,\"quantity\":3},"
									 "{\"id\":97,\"quantity\":1},"
									 "{\"id\":98,\"quantity\":2}]}", 0, &error);
	if (cart_json == NULL) {
		fprintf(stderr, "Error: on line %d: %s\n", error.line, error.text);
		exit(EXIT_FAILURE);
	}
	json_t *new_cart_json = http_post_json("https://dummyjson.com/carts/add", cart_json);

	printf("%s", json_dumps(new_cart_json, JSON_INDENT(4)));
}
