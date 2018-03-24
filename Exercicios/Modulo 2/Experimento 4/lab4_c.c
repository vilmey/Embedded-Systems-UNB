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
int error = 0;

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
RT_TASK user_task;

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
struct T_time
{
	int hour;
	int min;
	int sec;
};

T_time rt_clock;

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
RTIME alarm_period = 1000000000;
#define ALARM_NAME		"alarm"

/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////
void cleanup(void);
void catch_signal(int sig);

void task_body(void *arg);

void alarm_handler(void *arg);
void error_alarm_delete(int erro);
void error_alarm_start(int erro);
void error_alarm_create(int erro);


void error_spawn(int erro);
void error_join(int erro);
void error_set_periodic(int erro);
void error_wait_period(int erro);
void error_inquire(int erro);
void error_delete(int erro);


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
 	if((error = rt_task_spawn(&user_task, TASK_NAME, TASK_STKSZ, TASK_PRIO, TASK_MODE, &task_body, NULL)) != 0)
 		error_spawn(erro);
 	
 	//Espera que a user_task finalize suas atividades
 	if((error = rt_task_join(&user_task)) != 0)
 		error_join(error);
 
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

	//if((error = rt_task_set_periodic (NULL, TM_NOW, TASK_SLEEP)) !=0 )
 	//	error_set_periodic(error);

 	//Seta a struct
 	rt_clock.hour = 0; rt_clock.min = 0; rt_clock.sec = 0;  

	//Cria um objeto alarme no kernel space
	if((error = rt_alarm_create(&alarm, ALARM_NAME, alarm_handler, NULL)) != 0 )
		error_alarm_create(error);

	//Inicia um alarme
	if((error = rt_alarm_start (&alarm, TM_NOW, alarm_period)) ! = 0)
		error_alarm_start(error);

 	while(1)
 	{
 		//if((error = rt_task_wait_period(NULL)) != 0)
 		//	error_wait_period(error);

 		if((error = rt_alarm_wait (&alarm)) != 0)
 			error_alarm_wait(error);

 		system("clear");
 		rt_printf("Tempo : %02d : %02d : %02d", rt_clock.hour, rt_clock.min, rt_clock.sec);
 		
 	}
}




/////////////////////////////////////////////////////
// TRATAMENTO DE ERROS
/////////////////////////////////////////////////////
void error_alarm_wait(int erro)
{
	switch(erro)
	{
		
		case(-EINVAL): 
			rt_printf("EINVAL is returned if alarm is not a valid alarm descriptor.\n\n"); 
			break;
		
		case(-EPERM) : 
			rt_printf("EPERM is returned if this service was called from an asynchronous context.\n\n"); 
			break;

		case(-EINTR) : 
			rt_printf("EINTR is returned if rt_task_unblock() was called for the current task before the request is satisfied.\n\n"); 
			break;

		case(-EIDRM) : 
			rt_printf("EIDRM is returned if alarm is deleted while the caller was sleeping on it.\n\n"); 
			break;
	}
	
	exit(1);
}

void error_alarm_delete(int erro)
{
	switch(erro)
	{
		
		case(-EINVAL): 
			rt_printf("EINVAL is returned if alarm is not a valid alarm descriptor.\n\n"); 
			break;
		
		case(-EPERM) : 
			rt_printf("EPERM is returned if this service was called from an asynchronous context.\n\n"); 
			break;
	}
	
	exit(1);
}


void error_alarm_start(int erro)
{
	switch(erro)
	{
		
		case(-EINVAL): 
			rt_printf("EINVAL is returned if alarm is not a valid alarm descriptor.\n\n"); 
			break;
		
		case(-EPERM) : 
			rt_printf("EPERM is returned if this service was called from an asynchronous context.\n\n"); 
			break;
	}
	
	exit(1);
}


void error_alarm_create(int erro)
{
	switch(erro)
	{
		case(-ENOMEM): 
			rt_printf("ENOMEM is returned if the system fails to get memory from the main heap in order to create the task.\n\n"); 
			break;
		
		case(-EEXIST): 
			rt_printf("EEXIST is returned if the name is conflicting with an already registered task.\n\n"); 
			break;
		
		case(-EPERM) : 
			rt_printf("EPERM is returned if this service was called from an asynchronous context.\n\n"); 
			break;
	}
	
	exit(1);
}


void error_spawn(int erro)
{
	switch(erro)
	{
		case(-ENOMEM): 
			rt_printf("ENOMEM is returned if the system fails to get memory from the main heap in order to create the task.\n\n"); 
			break;
		
		case(-EEXIST): 
			rt_printf("EEXIST is returned if the name is conflicting with an already registered task.\n\n"); 
			break;
		
		case(-EINVAL) : 
			rt_printf("EINVAL is returned if either prio, mode or stksize are invalid.\n\n"); 
			break;
	}
	
	exit(1);
}

void error_join(int erro)
{
	switch(erro)
	{
		case(-EINVAL):  
			rt_printf("EINVAL is returned if task is not a valid task descriptor.\n\n"); 
			break;

		case(-EDEADLK): 
			rt_printf("EDEADLK is returned if task refers to the caller.\n\n"); 
			break;

		case(-ESRCH) :  
			rt_printf("ESRCH is returned if task no longer exists or refers to task created by a different process.\n\n"); 
			break;

	}

	exit(1);
}

void error_set_periodic(int erro)
{
	switch(erro)
	{
		case(-EINVAL):      
			rt_printf("EINVAL is returned if task is NULL but the caller is not a Xenomai task, or if task is non-NULL but not a valid task descriptor.\n\n"); 
			break;

		case(-ETIMEDOUT) :  
			rt_printf("ETIMEDOUT is returned if idate is different from TM_INFINITE and represents a date in the past.\n\n"); 
			break;

	}
	
	exit(1);
}

void error_wait_period(int erro)
{
	switch(erro)
	{
		case(-EWOULDBLOCK): 
			rt_printf("EWOULDBLOCK is returned if rt_task_set_periodic() was not called for the current task.\n\n"); 
			break;

		case(-EINTR):       
			rt_printf("EINTR is returned if rt_task_unblock() was called for the waiting task before the next periodic release point was reached. In this case, the overrun counter is also cleared.\n\n"); 
			break;

		case(-ETIMEDOUT) :  
			rt_printf("ETIMEDOUT is returned if a timer overrun occurred.\n\n"); 
			break;

		case(-EPERM):       
			rt_printf("EPERM is returned if this service was called from an invalid context.\n\n"); 
			break;

	}
	
	exit(1);
}

void error_inquire(int erro)
{
	switch(erro)
	{
		case(-EINVAL): 
			rt_printf("EINVAL is returned if task is not a valid task descriptor, or if prio is invalid.\n\n"); 
			break;

		case(-EPERM):  
			rt_printf("EPERM is returned if task is NULL and this service was called from an invalid context.\n\n"); 
			break;

	}
	
	exit(1);
}

void error_delete(int erro)
{
	switch(erro)
	{
		case(-EINVAL): 
			rt_printf("EINVAL is returned if task is not a valid task descriptor.\n\n"); 
			break;

		case(-EPERM):  
			rt_printf("EPERM is returned if task is NULL and this service was called from an invalid context.\n\n"); 
			break;

	}
	
	exit(1);
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
 	if((error = rt_task_delete(&user_task)) != 0)
 		error_delete(error);
 	
 	//Exclui um objeto alarme
	if((error = rt_alarm_delete(&alarm)) != 0)
		error_alarm_delete(error);

 	munlockall();
}

void alarm_handler(void *arg)
{
	
	if((error = rt_alarm_start(&alarm, TM_NOW, alarm_period)) != 0)
		error_alarm_start(error);

	rt_clock.sec +=1;

	if(rt_clock.sec == 60)
	{
		rt_clock.sec = 0;
		rt_clock.min += 1;
	}

	if(rt_clock.min == 60)
	{
		rt_clock.min = 0;
		rt_clock.hour += 1;
	}

	if(rt_clock.hour == 24)
	{
		rt_clock.hour = 0;
	}
}
