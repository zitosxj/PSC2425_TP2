CFLAGS = -Wall -c
LDFLAGS = -lcurl -ljansson

main: main.o user_get.o http_get_json.o
	gcc -o $@ $^ $(LDFLAGS)

main.o: main.c user_get.h ../exercicio1/http_get_json.h
	gcc $(CFLAGS) $< -o $@

user_get.o: user_get.c user_get.h ../exercicio1/http_get_json.h
	gcc $(CFLAGS) $< -o $@

http_get_json.o: http_get_json.c ../exercicio1/http_get_json.h
	gcc $(CFLAGS) $< -o $@

libtodo.so: get_random_todo.c
	gcc -shared -fPIC -o libtodo.so get_random_todo.c

clean:
	rm -f main *.o
