CFLAGS = -Wall -c 
LDFLAGS = -lcurl -ljansson

main: main.o http_post_json.o
	gcc -o $@ $^ $(LDFLAGS)
	
main.o: main.c http_post_json.h
	gcc $(CFLAGS) $< -o $@

http_get_json.o: http_post_json.c http_post_json.h
	gcc $(CFLAGS) $< -o $@
	
clean:
	rm -f main *.o
