#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <rtdk.h>

#define DEBUG

RT_TASK user_task1;
RT_TASK user_task2;
RT_TASK user_task3;
RT_TASK user_task4;

RT_TASK_INFO info_task1;
RT_TASK_INFO info_task2;
RT_TASK_INFO info_task3;

/////////////////////////////////////////////////////
// Definicao de macros
///////////////////////////////////////////////////////

#define TASK1_NAME "user_task1"
#define TASK1_PRIO 50 // Maior prioridade RT
#define TASK1_MODE T_JOINABLE
#define TASK1_STKSZ 0 /* Stack size (use default one) */
#define	TASK1_SLEEP 500000000

#define TASK2_NAME "user_task2"
#define TASK2_PRIO 60 // Maior prioridade RT
#define TASK2_MODE T_JOINABLE
#define TASK2_STKSZ 0 /* Stack size (use default one) */
#define	TASK2_SLEEP 600000000

#define TASK3_NAME "user_task3"
#define TASK3_PRIO 70 // Maior prioridade RT
#define TASK3_MODE T_JOINABLE
#define TASK3_STKSZ 0 /* Stack size (use default one) */
#define	TASK3_SLEEP 700000000

#define TASK4_NAME "user_task4"
#define TASK4_PRIO 99 // Maior prioridade RT
#define TASK4_MODE T_JOINABLE
#define TASK4_STKSZ 0 /* Stack size (use default one) */
#define	TASK4_SLEEP 1000000000


/////////////////////////////////////////////////////
// Prototipo de funcoes
/////////////////////////////////////////////////////


RTIME timer = 0;
RTIME timer_new = 0;
int elapsed_time = 0;
volatile long media1 = 0;
volatile long media2 = 0;
volatile long media3 = 0;
int count = 0;

void cleanup (void);
void catch_signal(int sig);

void task_body(void *arg)
{
	timer_new = rt_timer_read();
	rt_task_wait_period(NULL);
	timer = timer_new;
	
	while(count < 10)
	{ 	
	 	timer_new = rt_timer_read();
	 	elapsed_time = (int)(timer_new - timer);
	 	
	 	timer = timer_new;
	 	media += (long)(elapsed_time/1000);

	 	//rt_print_auto_init(1);
	 	
	 	rt_printf("Task (%d) - Elapsed time %d\n",(int)(&arg), elapsed_time);

	 	//rt_print_cleanup(1);
	 	count ++;

	 	rt_task_wait_period(NULL);
	}

	rt_printf("Media Task (%d) - %ld\n", (int)(&arg), (media));

}

void task_status(void *arg)
{
	int i = 0;
	while(i < 10)
	{
		int rt_task_inquire (&user_task1, &info_task1);
		rt_printf("Task 1 - Status %d\n", intfo_task1.prio);

		int rt_task_inquire (&user_task2, &info_task2);
		rt_printf("Task 2 - Status %d\n", intfo_task2.prio);

		int rt_task_inquire (&user_task3, &info_task3);
		rt_printf("Task 3 - Status %d\n", intfo_task3.prio);
		i ++;
	}
}

int main()
{
	//Instalando sinais
 	signal(SIGTERM, catch_signal);
 	signal(SIGINT, 	catch_signal);
 	signal(SIGKILL, catch_signal);
 	signal(SIGHUP, 	catch_signal);

 
	// Realizando um lock de memoria
 	mlockall(MCL_CURRENT|MCL_FUTURE);	//Trava na memoria, o atual e qualquer chamada futura

//---------------------------------------------------------------------------------------------- 	
 	// Criando a rt_task
 	if(rt_task_create(&user_task1,TASK1_NAME, TASK1_STKSZ,TASK1_PRIO,TASK1_MODE) != 0)
 		printf("Erro task create\n");

 	if(rt_task_create(&user_task2,TASK2_NAME, TASK2_STKSZ,TASK2_PRIO,TASK2_MODE) != 0)
 		printf("Erro task create\n");

 	if(rt_task_create(&user_task3,TASK3_NAME, TASK3_STKSZ,TASK3_PRIO,TASK3_MODE) != 0)
 		printf("Erro task create\n");

 	if(rt_task_create(&user_task4,TASK4_NAME, TASK4_STKSZ,TASK4_PRIO,TASK4_MODE) != 0)
 		printf("Erro task create\n");
//---------------------------------------------------------------------------------------------
 	//Setando a periodicidade da task

 	if(rt_task_set_periodic (&user_task1, TM_NOW, TASK1_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

 	if(rt_task_set_periodic (&user_task2, TM_NOW, TASK2_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

 	if(rt_task_set_periodic (&user_task3, TM_NOW, TASK3_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

 	if(rt_task_set_periodic (&user_task4, TM_NOW, TASK4_SLEEP) !=0 )
 		printf("Erro no set_perodic\n");

//---------------------------------------------------------------------------------------------

 	// Inicializando a rt_task
 	if(rt_task_start(&user_task1, &task_body, 1) != 0)
 		printf("Erro task start\n");

 	if(rt_task_start(&user_task2, &task_body, 2) != 0)
 		printf("Erro task start\n");

 	if(rt_task_start(&user_task3, &task_body, 3) != 0)
 		printf("Erro task start\n");

 	if(rt_task_start(&user_task4, &task_status, 4) != 0)
 		printf("Erro task start\n");

//---------------------------------------------------------------------------------------------
 	
 	//Espera que a rt_task finalize suas atividades
 	rt_task_join(&user_task1);
 	rt_task_join(&user_task2);
 	rt_task_join(&user_task3);
 	rt_task_join(&user_task4);
 
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
 	
 	rt_task_delete(&user_task1);
 	rt_task_delete(&user_task2);
 	rt_task_delete(&user_task3);
 	rt_task_delete(&user_task4);

 	munlockall();
}





/*



RTIME timer[10] = {0};
RTIME timer_new = 0;
int elapsed_time = 0;
volatile long media = 0;
volatile long s = 0;
int count = 0;

void cleanup (void);
void catch_signal(int sig);

void task_body(void *arg)
{
        int var = (int)arg;
        timer_new = rt_timer_read();
        timer[count]  = timer_new/1000000;
        elapsed_time  = timer[count];
        rt_task_wait_period(NULL);
        timer[count] = timer_new/1000000;
        while(count < 10)
        {
                timer_new = rt_timer_read();
                elapsed_time = timer_new/1000000 - timer[count];

                timer[count+1] = timer_new/1000000;
                media += (long)(elapsed_time);

                rt_printf("Task (%d) - Elapsed time %d ms\n", var, elapsed_time);

                count ++;
                rt_task_wait_period(NULL);
        }

        rt_printf("Media -> %ld\n", (media/10));

        for(count  = 0; count < 10; count ++)
        {
                s +=((timer[count] -  media)*(timer[count] - media));
        }

        rt_printf("Desvio padrao -> %lf \n", sqrt((s/9)));

        //write_RTIMES()

}




*/