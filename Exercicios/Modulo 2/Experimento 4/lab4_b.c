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
// PROPRIEDADES ALARME
/////////////////////////////////////////////////////
RT_ALARM alarm;
RTIME alarm_period = 3000000;
#define ALARM_NAME		"alarm"

/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////
void cleanup(void);
void catch_signal(int sig);
void task_body(void *arg);
void alarm_handler(void *arg);


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
 	if(rt_task_spawn(&user_task, TASK_NAME, TASK_STKSZ, TASK_PRIO, TASK_MODE, &task_body, NULL) != 0)
 		printf("Erro task create\n");
 	
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
	RT_ALARM_INFO info_alarm;

	if(rt_task_set_periodic (NULL, TM_NOW, TASK_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

	//Cria um objeto alarme no kernel space
	rt_alarm_create (&alarm, ALARM_NAME, alarm_handler, NULL);

	//Inicia um alarme
	rt_alarm_start (&alarm, TM_NOW, alarm_period);

 	while(1)
 	{
 		rt_task_wait_period(NULL);

 		rt_alarm_inquire (&alarm, &info_alarm);

 		rt_printf("Alarme (%s)\n", info_alarm.name);
 		rt_printf("Vezes expiradas (%lu)\n", info_alarm.expires);
 		rt_printf("Ativo %d.\n", info_alarm.active);

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
 	rt_task_delete(&user_task);
 	
 	//Exclui um objeto alarme
	rt_alarm_delete (&alarm);

 	munlockall();
}

void alarm_handler(void *arg)
{
	rt_printf("Meu primeiro alarme !!!\n");
	rt_alarm_start (&alarm, TM_NOW, alarm_period);
}
