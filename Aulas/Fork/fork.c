#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	int pid, fd[2], estado;

	char mensagem[30];

	if(pipe(fd) < 0)
	{
		printf("Erro\n");
		exit(1);
	}

	if((pid = fork()) < 0)
	{
		printf("Erro\n");
		exit(1);
	}

	if(pid == 0)
	{
		strcpy(mensagem, "Mensagem teste do pipe");
		if(write(fd[1], mensagem, sizeof(mensagem)) < 0)
		{
			printf("Erro na escrita !\n");
			wait(&estado);
		}
		
	}

	if(read(fd[0], mensagem, 30) < 0)
		printf("Erro\n");

	else
		printf("Valor lido %s\n", mensagem);
		exit(0);
	return 0;
}