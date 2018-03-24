#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>


/*Funcao de tratamento do SIGINT*/
void kill_child(int signum)
{
	printf("Estou sendo Encerrado !!!\n");
}

/*Funcao para printar o PID do filho*/
void child()
{
	printf("Esse é um processo filho, PID: %d\n", (int)getpid());
	while(1);
}


int main ()
{	
	pid_t child_pid1, child_pid2;	//Declaracao dos pids dos filhos

	signal(SIGINT, kill_child);		//Declaracao sera comum aos filhos, pois antes do fork()


	child_pid1 = fork();	//Child_pid vai receber o PID do filho (caso seja o pai)
							//Se for o processo filho child_pid recebe 0


	//Se for o processo pai
	if(child_pid1 != 0)
	{	
		child_pid2 = fork();			//Criamos o segundo processo filho

		if(child_pid2 != 0)				//Se ainda for o pai
		{
			printf("Esse é o processo pai, ID %d\n", (int)getpid());
			
			sleep(10);					//Aguarda por 10 segundos

			kill(child_pid1, SIGINT);	//Mata o filho 1
			kill(child_pid2, SIGINT);	//Mata o filho 2
		}
		
		else
			child();	//Se for o filho entra na funcao
	}

	//Se for o processo filho
	else
		child();

	return 0;
}
