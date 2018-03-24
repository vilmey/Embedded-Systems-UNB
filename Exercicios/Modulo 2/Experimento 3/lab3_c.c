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

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
RT_TASK user_task1;
RT_TASK user_task2;
RT_TASK user_task3;
RT_TASK user_task4;

/////////////////////////////////////////////////////
// PROPRIEDADES DAS TASKS
/////////////////////////////////////////////////////

//TASK1
#define TASK1_NAME 		"user_task1"
#define TASK1_PRIO 		50
#define TASK1_MODE 		T_JOINABLE
#define TASK1_STKSZ 	0 /* Stack size (use default one) */
#define	TASK1_SLEEP 	500000000

//TASK2
#define TASK2_NAME 		"user_task2"
#define TASK2_PRIO 		60
#define TASK2_MODE 		T_JOINABLE
#define TASK2_STKSZ 	0 /* Stack size (use default one) */
#define	TASK2_SLEEP 	600000000

//TASK3
#define TASK3_NAME 		"user_task3"
#define TASK3_PRIO 		70
#define TASK3_MODE 		T_JOINABLE
#define TASK3_STKSZ 	0 /* Stack size (use default one) */
#define	TASK3_SLEEP 	700000000

//TASK4
#define TASK4_NAME 		"user_task4"
#define TASK4_PRIO 		99 // Maior prioridade RT
#define TASK4_MODE 		T_JOINABLE
#define TASK4_STKSZ 	0 /* Stack size (use default one) */
#define	TASK4_SLEEP 	1000000000

/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////
void cleanup(void);
void catch_signal(int sig);

void task1_body(void *arg);
void task2_body(void *arg);
void task3_body(void *arg);
void task4_body(void *arg);


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
	if(rt_task_spawn(&user_task1, TASK1_NAME, TASK1_STKSZ, TASK1_PRIO, TASK1_MODE, &task1_body, NULL) != 0)
		rt_printf("Erro task create\n");

	if(rt_task_spawn(&user_task2, TASK2_NAME, TASK2_STKSZ, TASK2_PRIO, TASK2_MODE, &task2_body, NULL) != 0)
		rt_printf("Erro task create\n");

	if(rt_task_spawn(&user_task3, TASK3_NAME, TASK3_STKSZ, TASK3_PRIO, TASK3_MODE, &task3_body, NULL) != 0)
		rt_printf("Erro task create\n");

	if(rt_task_spawn(&user_task4, TASK4_NAME, TASK4_STKSZ, TASK4_PRIO, TASK4_MODE, &task4_body, NULL) != 0)
		rt_printf("Erro task create\n");
 	

 	
 	//Espera que a user_task finalize suas atividades
 	rt_task_join(&user_task1);
 	rt_task_join(&user_task2);
 	rt_task_join(&user_task3);
 	rt_task_join(&user_task4);
 
 	//Exclui a user_task e destrava memória
 	cleanup();
 	
 	return 0;
}


/////////////////////////////////////////////////////
// BODY USER_TASKS
/////////////////////////////////////////////////////
void task1_body(void *arg)
{
	//Variaveis da user_task
	int i;

	RTIME cur_time;
	RTIME new_time;

	//Setando a periodicidade da task
 	if(rt_task_set_periodic (NULL, TM_NOW, TASK1_SLEEP) !=0 )
 		rt_printf("Erro no set_perodic\n");

 	cur_time = rt_timer_read();

 	while(1)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		rt_printf("Task 1 - periodo %lf ms \n", (double)(new_time - cur_time)/1000000);
 		

 		cur_time = new_time;
 	}
}

void task2_body(void *arg)
{
	//Variaveis da user_task
	int i;

	RTIME cur_time;
	RTIME new_time;

	//Setando a periodicidade da task
 	if(rt_task_set_periodic (NULL, TM_NOW, TASK2_SLEEP) !=0 )
 		rt_printf("Erro no set_perodic\n");

 	cur_time = rt_timer_read();

 	while(1)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		rt_printf("Task 2 - periodo %lf ms \n", (double)(new_time - cur_time)/1000000);
 		

 		cur_time = new_time;
 	}
}

void task3_body(void *arg)
{
	//Variaveis da user_task
	int i;

	RTIME cur_time;
	RTIME new_time;

	//Setando a periodicidade da task
 	if(rt_task_set_periodic (NULL, TM_NOW, TASK3_SLEEP) !=0 )
 		rt_printf("Erro no set_perodic\n");

 	cur_time = rt_timer_read();

 	while(1)
 	{
 		rt_task_wait_period(NULL);
 		new_time = rt_timer_read();

 		rt_printf("Task 3 - periodo %lf ms \n", (double)(new_time - cur_time)/1000000);
 		

 		cur_time = new_time;
 	}

}

void task4_body(void *arg)
{
	//Variaveis da user_task
	int i;

	RT_TASK_INFO info_rand_task;

	//Setando a periodicidade da task
 	if(rt_task_set_periodic (NULL, TM_NOW, TASK4_SLEEP) !=0 )
 		rt_printf("Erro no set_perodic\n");

 	while(1)
 	{
 		rt_task_wait_period(NULL);

 		i = rand()%3;

 		switch(i)
 		{
 			case 0:
 				rt_task_inquire(&user_task1, &info_rand_task);
 				break;

 			case 1:
 				rt_task_inquire(&user_task2, &info_rand_task);
 				break;

 			case 2:
 				rt_task_inquire(&user_task3, &info_rand_task);
 				break;
 		}
 		
 		rt_printf("Task_%d (%s)\n", i+1, info_rand_task.name);
 		rt_printf("Prioridade base: %d.\n", info_rand_task.bprio);
 		rt_printf("Status: %u.\n", info_rand_task.status);
 		rt_printf("Trocas de contexto: %d.\n", info_rand_task.modeswitches);

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
	rt_printf("Signal received (%d) ...\n", sig);
 	rt_task_delete(&user_task1);
 	rt_task_delete(&user_task2);
 	rt_task_delete(&user_task3);
 	rt_task_delete(&user_task4);

 	munlockall();
}

