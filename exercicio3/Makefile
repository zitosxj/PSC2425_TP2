CFLAGS = -Wall -c -g
LDFLAGS = -lcurl -ljansson

products_get_test: products_get_test.o products_get.o ../exercicio1/http_get_json.o
	gcc -o $@ $^ $(LDFLAGS)
	
products_get_test.o: products_get_test.c products_get.h
	gcc $(CFLAGS) $< -o $@
	
products_get.o: products_get.c ../exercicio1/http_get_json.h
	gcc $(CFLAGS) $< -o $@
	
clean:
	rm -f products_get_test *.o
