/////////////////////////////////////////////////////
// BIBLIOTECAS
/////////////////////////////////////////////////////

#include <math.h>
#include <rtdk.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>

/////////////////////////////////////////////////////
// DEFINES
/////////////////////////////////////////////////////
#define DEBUG
#define run_times 10

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
RT_TASK user_task;

/////////////////////////////////////////////////////
// PROPRIEDADES TASK 1
/////////////////////////////////////////////////////
#define TASK_NAME  		"user_task"
#define TASK_STKSZ 		0
#define TASK_PRIO  		99
#define TASK_MODE  		T_JOINABLE
#define TASK_SLEEP     	1000000000

/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////
void cleanup(void);
void catch_signal(int sig);
void task_body(void *arg);


/////////////////////////////////////////////////////
// FUNCAO PRINCIPAL
/////////////////////////////////////////////////////

int main()
{
	//Tratamento de sinais
	signal(SIGTERM, catch_signal);
 	signal(SIGINT, 	catch_signal);
 	signal(SIGKILL, catch_signal);
 	signal(SIGHUP, 	catch_signal);

 	// Realizando um lock de memoria
 	mlockall(MCL_CURRENT|MCL_FUTURE);	//Trava na memoria, o atual e qualquer chamada futura

 	//Inicia o rt_printf
	rt_print_auto_init(1);
 	
 	// Criando a user_task
 	if(rt_task_create(&user_task, TASK_NAME, TASK_STKSZ, TASK_PRIO, TASK_MODE) != 0)
 		printf("Erro task create\n");
 	
 	// Inicializando a user_task
 	if(rt_task_start(&user_task, &task_body, NULL) != 0)
 		printf("Erro task start\n");
 	
 	//Espera que a user_task finalize suas atividades
 	rt_task_join(&user_task);
 
 	//Exclui a user_task e destrava memória
 	cleanup();
 	
 	return 0;
}


/////////////////////////////////////////////////////
// BODY USER_TASK
/////////////////////////////////////////////////////
void task_body(void *arg)
{
	//Variaveis da user_task
	int i;
	double tempo[run_times];
	double media 	= 0;
	double desvio 	= 0;
	double soma 	= 0;

	RTIME cur_time;
	RTIME new_time;

	//Setando a periodicidade da task
 	if(rt_task_set_periodic (NULL, TM_NOW, TASK_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

 	cur_time = rt_timer_read();

 	for(i = 0; i< run_times; i++)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		tempo[i] = (double)((new_time - cur_time)/1000000);
 		sum += tempo[i];

 		rt_printf("[%d] - Periodo %lf ms \n", i, (double)tempo[i]);
 		

 		cur_time = new_time;
 	}

 	media = (double)sum/run_times;

 	for(i = 0; i<run_times; i++)
 	{
 		desvio += (double)(pow((tempo[i]-media), 2));
 	}

 	desvio = sqrt(desvio/(run_times-1));
 		
 	rt_printf("Media %lf ms \n", media);
 	rt_printf("Devio %lf ms \n", desvio);
 	

}



/////////////////////////////////////////////////////
// TRATAMENTO DE SINAIS
/////////////////////////////////////////////////////

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

