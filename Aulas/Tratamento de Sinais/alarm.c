#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>

void alarm_handler(int signum)
{
	printf("Buzz Buzz %d\n",signum);

	alarm(1);//Novo alarme de 1 seg
}

void soneca (int signum)
{
	printf("Soneca !!! %d\n", signum);

	alarm(3);//Novo alarme de 1 seg
}

void desliga (int signum)
{
	printf("Vamos desligar ? %d\n", signum);
	alarm(0);
	//signal(SIGALRM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//signal(SIGINT, SIG_DFL);
}

int main()
{
	signal(SIGALRM, alarm_handler); //Configura o tratamento
	signal(SIGINT, soneca);
	signal(SIGQUIT, desliga);

	//alarm(1); //Define o alarme inicial
	
	alarm_handler(14);

	while(1)
	{
		pause(); //Aguarda um sinal
	}
}
