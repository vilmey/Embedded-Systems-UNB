#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	//Apresenta o PID deste programa quando em execucao
	printf("O identificador de processo (PID) é: %d\n", (int)getpid());
	
	//O processo pai deste programa sera o terminal
	printf("O identificador do processo pai (PPID) é: %d\n", (int)getppid());

	return 0;
}