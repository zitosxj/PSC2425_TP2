#include <stdio.h>
#include <stdlib.h>

#define RETURN_MSG "Prima ENTER para retornar ao menu."

void todo(const char *args){
	system("clear");
	printf("This is printed by the imported lib\n");
	
	printf("%s \n", RETURN_MSG);
	getchar();
}
