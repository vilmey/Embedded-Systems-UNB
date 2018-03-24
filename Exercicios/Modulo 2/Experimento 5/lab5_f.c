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
int error = 0;
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

void error_alarm_wait(int erro);
void error_alarm_delete(int erro);
void error_alarm_start(int erro);
void error_alarm_create(int erro);

void error_spawn(int erro);
void error_join(int erro);
void error_set_periodic(int erro);
void error_wait_period(int erro);
void error_inquire(int erro);
void error_delete(int erro);

void error_mutex_create(int erro);
void error_mutex_acquire(int erro);
void error_mutex_release(int erro);


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

	if((error = rt_mutex_create (&mutex, MUTEX_NAME)) != 0)
		error_mutex_create(error);
 	
 	// Criando a user_task
 	if((error = rt_task_spawn(&producer, PRODUCER_NAME, PRODUCER_STKSZ, PRODUCER_PRIO, PRODUCER_MODE, &task_producer, NULL)) != 0)
 		error_spawn(error);

 	if((error = rt_task_spawn(&consumer, CONSUMER_NAME, CONSUMER_STKSZ, CONSUMER_PRIO, CONSUMER_MODE, &task_consumer, NULL)) != 0)
 		error_spawn(error);
 	
 	//Espera que a user_task finalize suas atividades
 	if((error = rt_task_join(&producer)) != 0)
 		error_join(error);
 	if((error = rt_task_join(&consumer)) != 0)
 		error_join(error);

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

	if((error = rt_task_set_periodic(NULL, TM_NOW, PRODUCER_SLEEP)) != 0)
		error_set_periodic(error);

	cur_time = rt_timer_read();
	
 	for(i = 0; i < run_times; i++)
 	{
 		if((error = rt_task_wait_period(NULL)) != 0)
 			error_wait_period(error);

 		new_time = rt_timer_read();

 		if(nProduct < 50)
 		{
 			if((error = rt_mutex_acquire(&mutex, TM_INFINITE)) != 0)
 				error_mutex_acquire(error);

	 			produzido +=1;
	 			nProduct +=1;
 			if((error = rt_mutex_release(&mutex)) != 0)
 				error_mutex_release(error);

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

	if((error = rt_task_set_periodic(NULL, TM_NOW, CONSUMER_SLEEP)) != 0)
		error_set_periodic(error);

	cur_time = rt_timer_read();

 	for(i = 0; i < run_times; i++)
 	{
 		if((error = rt_task_wait_period(NULL)) != 0)
 			error_wait_period(error);

 		new_time = rt_timer_read();

 		if(nProduct > 0)
		{
	 		if((error = rt_mutex_acquire(&mutex, TM_INFINITE)) != 0)
 				error_mutex_acquire(error);	//Secao critica, Lock

	 			consumido +=1;
	 			nProduct -=1;

	 		if((error = rt_mutex_release(&mutex)) != 0)
 				error_mutex_release(error);				//Unlock
	 		
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
	
 	if((error = rt_task_delete(&producer)) != 0)
 		error_delete(error);

 	if((error = rt_task_delete(&consumer)) != 0)
 		error_delete(error);

 	munlockall();
}



/////////////////////////////////////////////////////
// TRATAMENTO DE ERROS
/////////////////////////////////////////////////////
void error_mutex_release(int erro)
{
	switch(erro)
	{
		
		case(-EINVAL) : 
			rt_printf("EINVAL is returned if alarm is not a valid mutex descriptor.\n\n"); 
			break;

		case(-EPERM) : 
			rt_printf("EPERM is returned if mutex is not owned by the current task.\n\n"); 
			break;
	}
	
	exit(1);
}

void error_mutex_acquire(int erro)
{
	switch(erro)
	{
		
		case(-ETIMEDOUT): 
			rt_printf("ETIMEDOUT is returned if abs_timeout is reached before the mutex is available.\n\n"); 
			break;
		
		case(-EWOULDBLOCK) : 
			rt_printf("EWOULDBLOCK is returned if timeout is { .tv_sec = 0, .tv_nsec = 0 } and the mutex is not immediately available.\n\n"); 
			break;

		case(-EINTR) : 
			rt_printf("EINTR is returned if rt_task_unblock() was called for the current task.\n\n"); 
			break;

		case(-EINVAL) : 
			rt_printf("EINVAL is returned if mutex is not a valid mutex descriptor.\n\n"); 
			break;

		case(-EIDRM) : 
			rt_printf("EIDRM is returned if mutex is deleted while the caller was waiting on it.\n\n"); 
			break;

		case(-EPERM) : 
			rt_printf("EPERM is returned if this service should block, but was not called from a Xenomai thread.\n\n"); 
			break;
	}
	
	exit(1);
}

void error_mutex_create(int erro)
{
	switch(erro)
	{
		
		case(-ENOMEM): 
			rt_printf("ENOMEM is returned if the system fails to get memory from the main heap in order to create the mutex.\n\n"); 
			break;
		
		case(-EEXIST) : 
			rt_printf("EEXIST is returned if the name is conflicting with an already registered mutex.\n\n"); 
			break;

		case(-EPERM) : 
			rt_printf("EPERM is returned if this service was called from an asynchronous context.\n\n"); 
			break;
	}
	
	exit(1);
}


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


