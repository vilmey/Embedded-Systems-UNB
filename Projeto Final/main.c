#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>


#define Print_pulso

//Velocidade fixa em 1cm/s
float velocidade = 0.01;
int toggle_x = 0, toggle_y = 0;

// PROTOTIPO DE FUNCOES
int menu(float *lado, float *altura, float *centro_x, float *centro_y);
void Calcula_quadrado(float lado, float altura, float centro_x, float centro_y);
void Calcula_triangulo(float lado, float altura, float centro_x, float centro_y);
void Calcula_trajeto(float atual_x, float atual_y, float final_x, float final_y);
void Move_motor_x(int passos_x, float tempo_execucao);
void Move_motor_y(int passos_y, float tempo_execucao);

//FUNCAO PRINCIPAL
int main ()
{
	int figura;
	float lado, altura, centro_x, centro_y;
	figura = menu(&lado, &altura, &centro_x, &centro_y);
	printf("Figura %d\n", figura);

	//CALCULA RETANGULO
	if(figura == 1)
	{
		Calcula_quadrado(lado, altura, centro_x, centro_y);
	}

	if(figura == 2)
	{
		Calcula_triangulo(lado, altura, centro_x, centro_y);
	}
}

//MENU DE OPCOES DO SISTEMA
int menu(float *lado, float *altura, float *centro_x, float *centro_y)
{
	int figura = 0;

	printf("[1] - Retangulo \n");
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

			printf("Posicao do centro (y):");
			scanf("%f", centro_y);
			if((((*centro_y) - (*altura)/3) < 0) || (((*altura)/3 + (*centro_y)) > 20))
			{ 
				printf("centro_y invalido\n");
				menu(lado, altura, centro_x, centro_y);
			}
			return figura;
			break;

		default:
			printf("Opcao Invalida !\n");
			menu(lado, altura, centro_x, centro_y);

	}
}

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
	float tempo_execucao = 0;
	float distancia_linear = sqrt(pow(final_x - atual_x, 2) + pow(final_y - atual_y, 2));

	if(distancia_linear > 0) tempo_execucao = velocidade/distancia_linear;

	int passos_x = (final_x - atual_x)/0.0157;
	int passos_y = (final_y - atual_y)/0.0157;

	printf("X_atual = %.3f, Y_atual = %.3f, X_final = %.3f, Y_final = %.3f\n", atual_x, atual_y, final_x, final_y);
	printf("D = %.3f, T = %.3f, P_x = %d, P_y = %d\n",distancia_linear, tempo_execucao, passos_x, passos_y );


	printf("Cria uma task para cada motor\n");
	if (passos_x != 0) Move_motor_x(passos_x, tempo_execucao);
	if (passos_y != 0) Move_motor_y(passos_y, tempo_execucao);
}

void Move_motor_x(int passos_x, float tempo_execucao)
{
	int passo_atual_x;
	double period_us = ((tempo_execucao*1000000)/abs(passos_x));
	printf("Wait period motor x %.3f\n", period_us);

	if(passos_x > 0)
	{
		//Sentido positivo
		for(passo_atual_x = 0; passo_atual_x < passos_x; passo_atual_x ++)
		{
			#ifdef Print_pulso
				printf("Passo_x 0, Pulso %d \n", toggle_x);
			#endif

			toggle_x ^= 1;
			usleep((int)(period_us*10000));
		}
	}

	else
	{
		//Sentido negativo
		for(passo_atual_x = 0; passo_atual_x > passos_x; passo_atual_x --)
		{	
			#ifdef Print_pulso
				printf("Passo_x 1, Pulso %d \n", toggle_x);
			#endif

			toggle_x ^= 1;
			usleep((int)(period_us*10000));
		}
	}
}

void Move_motor_y(int passos_y, float tempo_execucao)
{
	int passo_atual_y;
	double period_us = ((tempo_execucao*1000000)/abs(passos_y));
	printf("Wait period motor y %.3f\n", period_us);

	if(passos_y > 0)
	{
		//Sentido positivo
		for(passo_atual_y = 0; passo_atual_y < passos_y; passo_atual_y ++)
		{
			#ifdef Print_pulso
			printf("Passo_y 0, Pulso %d \n", toggle_y);
			#endif

			toggle_y ^= 1;
			usleep((int)(period_us*10000));
		}
	}

	else
	{
		//Sentido negativo
		for(passo_atual_y = 0; passo_atual_y > passos_y; passo_atual_y --)
		{
			#ifdef Print_pulso
			printf("Passo_y 1, Pulso %d \n", toggle_y);
			#endif

			toggle_y ^= 1;
			usleep((int)(period_us*10000));
		}
	}
}
