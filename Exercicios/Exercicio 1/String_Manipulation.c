#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char string[100];

	getchar();
	fgets(string, 100, stdin);

	printf("Antes: %s\n", string);

	strlw(string);
	printf("Depois: %s\n", string);

	return 0;
}