#include <stdio.h>
#include <jansson.h>
#include "http_get_json.h"

int main(){

	char uri[100] = "https://dummyjson.com/products/1";

	json_t *carts_json = http_get_json(uri);
	
	printf("%s", json_dumps(carts_json, JSON_INDENT(4)));
	
	return 0;
}
