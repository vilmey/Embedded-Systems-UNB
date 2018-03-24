#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#define DEBUG
RT_TASK user_task;

/////////////////////////////////////////////////////
// Definicao de macros
///////////////////////////////////////////////////////

#define TASK_NAME "user_task"
#define TASK_PRIO 99 // Maior prioridade RT
#define TASK_MODE T_JOINABLE
#define TASK_STKSZ 0 /* Stack size (use default one) */


/////////////////////////////////////////////////////
// Prototipo de funcoes
/////////////////////////////////////////////////////

void cleanup (void);
void catch_signal(int sig);

void task_body(void *arg)
{
 	int i;
 	for(i=0;i<10;i++)
 	{
 		printf("Executando o laço %d\n",i);
 	}
}

int main()
{
	//Instalando sinais
 	signal(SIGTERM, catch_signal);
 	signal(SIGINT, 	catch_signal);
 	signal(SIGKILL, catch_signal);
 	signal(SIGHUP, 	catch_signal);

	#ifdef DEBUG

		printf("--------------------------------------\n");
	 	printf("- Faculdade Gama - UNB -\n");
	 	printf("- Disciplina: Sistemas embarcados -\n");
	 	printf("- Professor: Evandro Teixeira -\n");
	 	printf("- Laboratorio: lab02a -\n");
	 	printf("--------------------------------------\n");

	#endif
 
	// Realizando um lock de memoria
 	mlockall(MCL_CURRENT|MCL_FUTURE);	//Trava na memoria, o atual e qualquer chamada futura
 	
 	// Criando a rt_task
 	if(rt_task_create(&user_task,TASK_NAME, TASK_STKSZ,TASK_PRIO,TASK_MODE) != 0)
 		printf("Erro task create\n");
 	
 	// Inicializando a rt_task
 	if(rt_task_start(&user_task, &task_body, NULL) != 0)
 		printf("Erro task start\n");
 	
 	//Espera que a rt_task finalize suas atividades
 	rt_task_join(&user_task);
 
 	//Exclui a rt_task e destrava memória
 	cleanup();
 	
 	return 0;
}

void cleanup (void)
{
	catch_signal(2);
}

void catch_signal(int sig)
{
	printf("Signal received (%d) ...\n", sig);
 	rt_task_delete(&user_task);
 	munlockall();
}