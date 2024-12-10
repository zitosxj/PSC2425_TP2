#include <stdio.h>
#include "user_get.h"

int main(){

Users *my_users = user_get();

    printf("Total de users: %ld\n", my_users->count);

    for (size_t i = 0; i < my_users->count; i++) {
        printf("Id: %d - Nome: %s\n", my_users->users[i].id, my_users->users[i].name);
    }

	free_users(my_users);

	return 0;
}
