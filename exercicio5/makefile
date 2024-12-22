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


# make libget_todos.so
# import on app "M E get_todos Obter lista de TODOS
libget_todos.so: get_todos.c
	gcc -shared -fPIC -o libget_todos.so get_todos.c

# make libget_categories.so
# import on app "M E get_todos Obter lista de TODOS
libget_categories.so: get_categories.c
	gcc -shared -fPIC -o libget_categories.so get_categories.c

#not used
libtodo.so: get_random_todo.c
	gcc -shared -fPIC -o libtodo.so get_random_todo.c

clean:
	rm -f main *.o
