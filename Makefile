CFLAGS = -Wall -c 
LDFLAGS = -lcurl -ljansson

products_users_cart_test: products_users_cart_test.o products_get.o user_get.o cart_put.o http_get_json.o http_post_json.o
	gcc -o $@ $^ $(LDFLAGS)

products_users_cart_test.o: products_users_cart_test.c products_get.h user_get.h
	gcc $(CFLAGS) $< -o $@


products_get.o: products_get.c products_get.h http_get_json.h
	gcc $(CFLAGS) $< -o $@

user_get.o: user_get.c user_get.h http_get_json.h
	gcc $(CFLAGS) $< -o $@

cart_put.o: cart_put.c cart_put.h http_post_json.h
	gcc $(CFLAGS) $< -o $@


http_get_json_test: http_get_json_test.o http_get_json.o
	gcc -o $@ $^ $(LDFLAGS)
	
http_get_json_test.o: http_get_json_test.c http_get_json.h
	gcc $(CFLAGS) $< -o $@

http_get_json.o: http_get_json.c http_get_json.h
	gcc $(CFLAGS) $< -o $@

	
http_post_json_test: http_post_json_test.o http_post_json.o
	gcc -o $@ $^ $(LDFLAGS)
	
http_post_json_test.o: http_post_json_test.c http_post_json.h
	gcc $(CFLAGS) $< -o $@

http_post_json.o: http_post_json.c http_post_json.h
	gcc $(CFLAGS) $< -o $@
	
		
clean:
	rm -f http_get_json_test -f http_post_json_test *.o
