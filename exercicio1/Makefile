CFLAGS = -Wall -c 
LDFLAGS = -lcurl -ljansson

main: main.o http_get_json.o
	gcc -o $@ $^ $(LDFLAGS)
	
main.o: main.c http_get_json.h
	gcc $(CFLAGS) $< -o $@

http_get_json.o: http_get_json.c http_get_json.h
	gcc $(CFLAGS) $< -o $@
	
clean:
	rm -f main *.o
