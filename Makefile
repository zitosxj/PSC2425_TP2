CFLAGS = -Wall -c 
LDFLAGS = -lcurl -ljansson

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
