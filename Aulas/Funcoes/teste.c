#include <stdio.h>		//Pastas padrao do sistema
#include <stdlib.h>
#include "funcoes.h"	//Pasta local


int main(int argc, char ** argv)
{
	//void imprime_soma();
	//void subtracao();

	float a,b;

	printf("\nValor de a: ");
	scanf("%f",&a);

	printf("Valor de b: ");
	scanf("%f",&b);

	imprime_soma(a,b);
	imprime_subtracao(a,b);

	return 0;

}
