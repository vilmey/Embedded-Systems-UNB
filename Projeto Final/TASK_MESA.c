/////////////////////////////////////////////////////
// BIBLIOTECAS
/////////////////////////////////////////////////////

#include <rtdk.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


/////////////////////////////////////////////////////
// DEFINES
/////////////////////////////////////////////////////
#define Print_pulso

/////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS
/////////////////////////////////////////////////////
//Velocidade fixa em 1cm/s
float velocidade = 0.01;
int toggle_x = 0, toggle_y = 0;
int passos_x;
int passos_y; 
float tempo_execucao;
int passos_x;
int passos_y;

/////////////////////////////////////////////////////
// DECLARACAO DAS TASKS DO SISTEMA
/////////////////////////////////////////////////////
RT_TASK motor_x_task;
RT_TASK motor_y_task;

/////////////////////////////////////////////////////
// PROPRIEDADES TASK MOTOR X
/////////////////////////////////////////////////////
#define MOTOR_X_TASK_NAME  		"motor_x_task"
#define MOTOR_X_TASK_STKSZ 		0
#define MOTOR_X_TASK_PRIO  		50
#define MOTOR_X_TASK_MODE  		T_JOINABLE
RTIME MOTOR_X_TASK_SLEEP 	=	0;

#define MOTOR_Y_TASK_NAME  		"motor_y_task"
#define MOTOR_Y_TASK_STKSZ 		0
#define MOTOR_Y_TASK_PRIO  		50
#define MOTOR_Y_TASK_MODE  		T_JOINABLE
RTIME MOTOR_Y_TASK_SLEEP 	= 	0;


/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////

// PROTOTIPO DE FUNCOES
int menu(float *lado, float *altura, float *centro_x, float *centro_y);
void Calcula_quadrado(float lado, float altura, float centro_x, float centro_y);
void Calcula_triangulo(float lado, float altura, float centro_x, float centro_y);
void Calcula_trajeto(float atual_x, float atual_y, float final_x, float final_y);
void Move_motor_x(void *arg);
void Move_motor_y(void *arg);

void cleanup (void);
void catch_signal(int sig);


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

 	int figura;
	float lado, altura, centro_x, centro_y;

	while(1)
	{	
		figura = menu(&lado, &altura, &centro_x, &centro_y);
		printf("Figura %d\n", figura);
	
		//CALCULA RETANGULO
		if(figura == 1)
		{
			Calcula_quadrado(lado, altura, centro_x, centro_y);
		}
		
		//CALCULA TRIANGULO
		if(figura == 2)
		{
			Calcula_triangulo(lado, altura, centro_x, centro_y);
		}
	}

 	return 0;
}




/////////////////////////////////////////////////////
// BODY TASK MOTOR_X / MOTOR_Y
/////////////////////////////////////////////////////

void Move_motor_x(void *arg)
{
	int passo_atual_x;

	if(passos_x != 0) MOTOR_X_TASK_SLEEP = (int)((tempo_execucao*1000000)/abs(passos_x));
	rt_task_set_periodic(NULL, TM_NOW, MOTOR_X_TASK_SLEEP);
	rt_printf("Wait period motor x %d\n", MOTOR_X_TASK_SLEEP);

	if(passos_x > 0)
	{
		//Sentido positivo
		for(passo_atual_x = 0; passo_atual_x < passos_x; passo_atual_x ++)
		{
			#ifdef Print_pulso
				rt_printf("Passo_x 0, Pulso %d \n", toggle_x);
			#endif

			toggle_x ^= 1;
			rt_task_wait_period(NULL);
		}
	}

	if(passos_x < 0)
	{
		//Sentido negativo
		for(passo_atual_x = 0; passo_atual_x > passos_x; passo_atual_x --)
		{	
			#ifdef Print_pulso
				rt_printf("Passo_x 1, Pulso %d \n", toggle_x);
			#endif

			toggle_x ^= 1;
			rt_task_wait_period(NULL);
		}
	}
}

void Move_motor_y(void *arg)
{
	int passo_atual_y;
	if(passos_y != 0) MOTOR_Y_TASK_SLEEP = (int)((tempo_execucao*1000000)/abs(passos_y));
	rt_task_set_periodic(NULL, TM_NOW, MOTOR_Y_TASK_SLEEP);
	rt_printf("Wait period motor y %d\n", MOTOR_Y_TASK_SLEEP);

	if(passos_y > 0)
	{
		//Sentido positivo
		for(passo_atual_y = 0; passo_atual_y < passos_y; passo_atual_y ++)
		{
			#ifdef Print_pulso
			rt_printf("Passo_y 0, Pulso %d \n", toggle_y);
			#endif

			toggle_y ^= 1;
			rt_task_wait_period(NULL);
		}
	}

	if(passos_y < 0)
	{
		//Sentido negativo
		for(passo_atual_y = 0; passo_atual_y > passos_y; passo_atual_y --)
		{
			#ifdef Print_pulso
			rt_printf("Passo_y 1, Pulso %d \n", toggle_y);
			#endif

			toggle_y ^= 1;
			rt_task_wait_period(NULL);
		}
	}
}




/////////////////////////////////////////////////////
// FUNCOES DE CALCULO GEOMETRICO
/////////////////////////////////////////////////////

//CALCULO DAS COORDENADAS DO TRIANGULO
void Calcula_triangulo(float lado, float altura, float centro_x, float centro_y)
{
	//TRAJETO ORIGEM AO VERTICE 1
	printf("\n\n TRAJETO ORIGEM AO VERTICE 1\n\n");
	Calcula_trajeto(0, 0, (centro_x - (lado/3)), (centro_y - (altura/3)));

	//TRAJETO VERTICE 1 AO VERTICE 2
	printf("\n\n TRAJETO VERTICE 1 AO VERTICE 2\n\n");
	Calcula_trajeto(centro_x - (lado/3), centro_y - (altura/3), centro_x + ((2*lado)/3), centro_y - (altura/3));

	//TRAJETO VERTICE 2 AO VERTICE 3
	printf("\n\n TRAJETO VERTICE 2 AO VERTICE 3\n\n");
	Calcula_trajeto(centro_x + ((2*lado)/3), centro_y - (altura/3), centro_x - (lado/3), centro_y + ((2*altura)/3));

	//TRAJETO VERTICE 3 AO VERTICE 1
	printf("\n\n TRAJETO VERTICE 3 AO VERTICE 1\n\n");
	Calcula_trajeto(centro_x - (lado/3), centro_y + ((2*altura)/3), centro_x - (lado/3), centro_y - (altura/3));

}

//CALCULO DAS COORDENADAS DO QUADRADO
void Calcula_quadrado(float lado, float altura, float centro_x, float centro_y)
{
	//TRAJETO ORIGEM AO VERTICE 1
	printf("\n\n TRAJETO ORIGEM AO VERTICE 1\n\n");
	Calcula_trajeto(0, 0, (centro_x - (lado/2)), (centro_y - (altura/2)));

	//TRAJETO VERTICE 1 AO VERTICE 2
	printf("\n\n TRAJETO VERTICE 1 AO VERTICE 2\n\n");
	Calcula_trajeto(centro_x - (lado/2), centro_y - (altura/2), centro_x + (lado/2), centro_y - (altura/2));

	//TRAJETO VERTICE 2 AO VERTICE 3
	printf("\n\n TRAJETO VERTICE 2 AO VERTICE 3\n\n");
	Calcula_trajeto(centro_x + (lado/2), centro_y - (altura/2), centro_x + (lado/2), centro_y + (altura/2));

	//TRAJETO VERTICE 3 AO VERTICE 4
	printf("\n\n TRAJETO VERTICE 3 AO VERTICE 4\n\n");
	Calcula_trajeto(centro_x + (lado/2), centro_y + (altura/2), centro_x - (lado/2), centro_y + (altura/2));

	//TRAJETO VERTICE 4 AO VERTICE 1
	printf("\n\n TRAJETO VERTICE 4 AO VERTICE 1\n\n");
	Calcula_trajeto(centro_x - (lado/2), centro_y + (altura/2), centro_x - (lado/2), centro_y - (altura/2));

}

//CALCULOS DO TRAJETO, DISTANCIA LINEAR E PASSOS DO MOTOR
void Calcula_trajeto(float atual_x, float atual_y, float final_x, float final_y)
{
	//DISTANCIA LINEAR A SER PERCORRIDA
	float distancia_linear = sqrt(pow(final_x - atual_x, 2) + pow(final_y - atual_y, 2));

	//CALCULO DO TEMPO DE EXECUCAO DO TRECHO
	if(distancia_linear > 0) tempo_execucao = distancia_linear/velocidade;

	passos_x = (int)((final_x - atual_x)/0.0157);
	passos_y = (int)((final_y - atual_y)/0.0157);

	printf("(X_atual = %.3f, Y_atual = %.3f)  (X_final = %.3f, Y_final = %.3f\n)", atual_x, atual_y, final_x, final_y);
	printf("D = %.3f [cm], T = %.3f [s], P_x = %d, P_y = %d\n",distancia_linear, tempo_execucao, passos_x, passos_y );


	// Realizando um lock de memoria
 	mlockall(MCL_CURRENT|MCL_FUTURE);	//Trava na memoria, o atual e qualquer chamada futura

 	//Inicia o rt_printf
	rt_print_auto_init(1);


	printf("Cria uma task para cada motor\n");
	// Criando a Motor_task
 	if (passos_x != 0) rt_task_spawn(&motor_x_task, MOTOR_X_TASK_NAME, MOTOR_X_TASK_STKSZ, MOTOR_X_TASK_PRIO, MOTOR_X_TASK_MODE, &Move_motor_x, NULL);
 	if (passos_y != 0) rt_task_spawn(&motor_y_task, MOTOR_Y_TASK_NAME, MOTOR_Y_TASK_STKSZ, MOTOR_Y_TASK_PRIO, MOTOR_Y_TASK_MODE, &Move_motor_y, NULL);

 	rt_printf("Task started");

 	//Espera que a user_task finalize suas atividades
 	if (passos_x != 0) rt_task_join(&motor_x_task);
 	if (passos_y != 0) rt_task_join(&motor_y_task);

 	//Exclui a user_task e destrava memória
 	cleanup();

}

/////////////////////////////////////////////////////
// MENU DE FUNCOES
/////////////////////////////////////////////////////

//MENU DE OPCOES DO SISTEMA
int menu(float *lado, float *altura, float *centro_x, float *centro_y)
{
	int figura = 0;

	printf("\n\n[1] - Retangulo \n");
	printf("[2] - Triangulo \n");
	scanf("%d", &figura);

	switch(figura)
	{
		case 1:
			printf("Digite o lado: ");
			scanf("%f", lado);
			if((*lado) > 20)
			{ 
				printf("lado invalido\n");
				*lado = 0, *altura = 0; centro_x = 0; centro_y = 0;
				menu(lado, altura, centro_x, centro_y);
			}
			
			printf("Digite a altura: ");
			scanf("%f", altura);
			if((*altura) > 20)
			{ 
				printf("altura invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Posicao do centro (x): ");
			scanf("%f", centro_x);
			if((((*centro_x) - (*lado)/2) < 0) || (((*lado)/2 + (*centro_x)) > 20)) 
			{ 
				printf("centro_x invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Posicao do centro (y): ");
			scanf("%f", centro_y);
			if((((*centro_y) - (*altura)/2) < 0) || (((*altura)/2 + (*centro_y)) > 20))
			{ 
				printf("centro_y invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Velocidade de execuccao (cm/s): ");
			scanf("%f", &velocidade);

			return figura;
			break;

		case 2:
			printf("Digite o lado: ");
			scanf("%f", lado);
			if(*lado > 20)
			{ 
				printf("lado invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}
			
			printf("Digite a altura: ");
			scanf("%f", altura);
			if(*altura > 20)
			{ 
				printf("altura invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Posicao do centro (x): ");
			scanf("%f", centro_x);
			if((((*centro_x) - (*lado)/3) < 0) || (((*lado)/3 + (*centro_x)) > 20))
			{ 
				printf("centro_x invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Posicao do centro (y): ");
			scanf("%f", centro_y);
			if((((*centro_y) - (*altura)/3) < 0) || (((*altura)/3 + (*centro_y)) > 20))
			{ 
				printf("centro_y invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}

			printf("Velocidade de execuccao (cm/s): ");
			scanf("%f", &velocidade);

			return figura;
			break;

		default:
			printf("Opcao Invalida !\n");
			menu(lado, altura, centro_x, centro_y);

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
	
 	rt_task_delete(&motor_x_task);
 	rt_task_delete(&motor_y_task);

 	munlockall();
}


