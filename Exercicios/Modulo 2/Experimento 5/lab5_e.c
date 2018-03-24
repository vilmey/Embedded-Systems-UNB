/////////////////////////////////////////////////////
// BIBLIOTECAS
/////////////////////////////////////////////////////

#include <math.h>
#include <rtdk.h>
#include <stdio.h>
#include <alarm.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>

/////////////////////////////////////////////////////
// DEFINES
/////////////////////////////////////////////////////
#define DEBUG
#define run_times 100
int nProduct = 0;
int produzido = 0;
int consumido = 0;

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
RT_TASK producer;
RT_TASK consumer;
RT_MUTEX mutex

/////////////////////////////////////////////////////
// PROPRIEDADES DAS TASKS
/////////////////////////////////////////////////////
#define PRODUCER_NAME  		"producer"
#define PRODUCER_STKSZ 		0
#define PRODUCER_PRIO  		50
#define PRODUCER_MODE  		T_JOINABLE
#define	PRODUCER_SLEEP 		1000000

#define CONSUMER_NAME  		"consumer"
#define CONSUMER_STKSZ 		0
#define CONSUMER_PRIO  		50
#define CONSUMER_MODE  		T_JOINABLE
#define	CONSUMER_SLEEP 		10000000

#define MUTEX_NAME			"mutex"
/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////
void cleanup(void);
void catch_signal(int sig);
void task_producer(void *arg);
void task_consumer(void *arg);


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

	rt_mutex_create (&mutex, MUTEX_NAME);
 	
 	// Criando a user_task
 	if(rt_task_spawn(&producer, PRODUCER_NAME, PRODUCER_STKSZ, PRODUCER_PRIO, PRODUCER_MODE, &task_producer, NULL) != 0)
 		printf("Erro task create\n");

 	if(rt_task_spawn(&consumer, CONSUMER_NAME, CONSUMER_STKSZ, CONSUMER_PRIO, CONSUMER_MODE, &task_consumer, NULL) != 0)
 		printf("Erro task create\n");
 	
 	//Espera que a user_task finalize suas atividades
 	rt_task_join(&producer);
 	rt_task_join(&consumer);

 	//Printa a vairiavel nProduct
 	rt_printf("\n\n Produto - ( %d ).\n", nProduct);
 	rt_printf("Produzido (%d)\n", produzido);
 	rt_printf("Consumido (%d)\n\n", consumido); 
 
 	//Exclui a user_task e destrava memória
 	cleanup();
 	
 	return 0;
}


/////////////////////////////////////////////////////
// BODY TASK_PRDUCER/CONSUMER
/////////////////////////////////////////////////////
void task_producer(void *arg)
{
	//Variaveis da user_task
	int i;

	RTIME cur_time;
	RTIME new_time;

	rt_task_set_periodic(NULL, TM_NOW, PRODUCER_SLEEP);

	cur_time = rt_timer_read();
	
 	for(i = 0; i < run_times; i++)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		if(nProduct < 50)
 		{
 			rt_mutex_acquire(&mutex, TM_INFINITE);
	 			produzido +=1;
	 			nProduct +=1;
 			rt_mutex_release(&mutex);
 			rt_printf("[%d] - nProduct %d  -   time %lf ms \n", i, (double)((new_time - cur_time)/1000000);
 		}
 		
 		rt_printf("Full queue \n");
 		
 	}
}

void task_consumer(void *arg)
{
	//Variaveis da user_task
	int i;

	RTIME cur_time;
	RTIME new_time;

	rt_task_set_periodic(NULL, TM_NOW, CONSUMER_SLEEP);

	cur_time = rt_timer_read();

 	for(i = 0; i < run_times; i++)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		if(nProduct > 0)
		{
	 		rt_mutex_acquire(&mutex, TM_INFINITE);	//Secao critica, Lock
	 			consumido +=1;
	 			nProduct -=1;
	 		rt_mutex_release(&mutex);				//Unlock
	 		
	 		rt_printf("[%d] - nProduct %d  -   time %lf ms \n", i, (double)((new_time - cur_time)/1000000);
	 	}
	 	rt_printf("Queue empty \n");
 	}
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
 	rt_task_delete(&producer);
 	rt_task_delete(&consumer);

 	munlockall();
}


