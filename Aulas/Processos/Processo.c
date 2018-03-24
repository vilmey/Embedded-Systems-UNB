#include <stdlib.h>
#include <stdio.h>

int main()
{
	int retorna_valor;
	retorna_valor = system("ls -la /");

	return retorna_valor;
}