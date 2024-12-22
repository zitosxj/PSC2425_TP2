CFLAGS = -Wall -pedantic -c -Og
LDFLAGS = -lcurl -ljansson

main: main.o api_json.so http_json.so
	gcc -o $@ $< ./api_json.so ./http_json.so $(LDFLAGS)

main.o: main.c
	gcc $(CFLAGS) -g $< -o $@


api_json.so: api_json.o
	gcc -shared $^ -o $@

http_json.so: http_json.o
	gcc -shared $^ -o $@


api_json.o: api_json.c api_json.h http_json.h
	gcc $(CFLAGS) -fpic $< -o $@

http_json.o: http_json.c http_json.h
	gcc $(CFLAGS) -fpic $< -o $@
	
		
clean:
	rm -f products_users_cart_so_test *.o
