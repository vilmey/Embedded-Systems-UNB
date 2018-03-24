#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void funcao_sigusr1()
{
	printf("recebi sigusr1\n");
}

void funcao_sigusr2()
{
	printf("recebi sigusr2\n");
}

int main()
{
	signal(SIGUSR1, funcao_sigusr1);
 	signal(SIGUSR2, funcao_sigusr2);
 	kill(getpid(), SIGUSR1);
 	kill(getpid(), SIGUSR2);

 	return 0;
}