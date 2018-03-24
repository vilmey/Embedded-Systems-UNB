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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <termios.h>
#include <fcntl.h>


/////////////////////////////////////////////////////
// DEFINES
/////////////////////////////////////////////////////

struct TFigura
{
	int 	tipo;
	float 	largura;
	float 	altura;
	float 	centro_x;
	float 	centro_y;
	float 	velocidade;
};

struct TFigura Figura;

/////////////////////////////////////////////////////
// FUNCOES
/////////////////////////////////////////////////////

// PROTOTIPO DE FUNCOES
void menu();
void socket_open(int *clienteSocket, struct sockaddr_in *servidorAddr, char *IP_Servidor, unsigned short servidorPorta);
void request(int *clienteSocket);



/////////////////////////////////////////////////////
// FUNCAO PRINCIPAL
/////////////////////////////////////////////////////


int main()
{
 	int clienteSocket;
 	unsigned short servidorPorta = 4040;
 	struct sockaddr_in servidorAddr;
 	char *IP_Servidor;

 	if ((argc < 2) | (argc > 3)) 
	{
		printf("Uso: %s <IP do Servidor>\n", argv[0]);
		exit(1);
	}

	//Ip via linha de comando
	IP_Servidor = argv[1];

	while(1)
	{	
		figura = menu(&lado, &altura, &centro_x, &centro_y);
		printf("Figura %d\n", figura);
		
		socket_open(&clienteSocket, &servidorAddr, IP_Servidor, servidorPorta);

		//Enviamos um dado do menu
		request(&clienteSocket);

		//Fechamos a conexão com o servidor
		close(clienteSocket);
	}

 	return 0;
}


/////////////////////////////////////////////////////
// MENU DE FUNCOES
/////////////////////////////////////////////////////

//MENU DE OPCOES DO SISTEMA
void menu()
{
	int figura = 0;

	printf("\n\n[1] - Retangulo \n");
	printf("[2] - Triangulo \n");
	scanf("%d", &Figura.tipo);

	switch(figura)
	{
		case 1:
			printf("Digite o lado: ");
			scanf("%f", &Figura.lado);
			if((*lado) > 20)
			{ 
				printf("lado invalido\n");
				Figura.lado = 0, Figura.altura = 0; Figura.centro_x = 0; Figura.centro_y = 0;
				menu();
			}
			
			printf("Digite a altura: ");
			scanf("%f", &Figura.altura);
			if((Figura.altura) > 20)
			{ 
				printf("altura invalido\n");
				menu();
			}

			printf("Posicao do centro (x): ");
			scanf("%f", &Figura.centro_x);
			if((((Figura.centro_x) - (Figura.lado)/2) < 0) || (((Figura.lado)/2 + (Figura.centro_x)) > 20)) 
			{ 
				printf("centro_x invalido\n");
				menu();
			}

			printf("Posicao do centro (y): ");
			scanf("%f", &Figura.centro_y);
			if((((Figura.centro_y) - (Figura.altura)/2) < 0) || (((Figura.altura)/2 + (Figura.centro_y)) > 20))
			{ 
				printf("centro_y invalido\n");
				menu();
			}

			printf("Velocidade de execuccao (cm/s): ");
			scanf("%f", &Figura.velocidade);

			break;

		case 2:
			printf("Digite o lado: ");
			scanf("%f", &Figura.lado);
			if(*lado > 20)
			{ 
				printf("lado invalido\n");
				menu();
			}
			
			printf("Digite a altura: ");
			scanf("%f", &Figura.altura);
			if(*altura > 20)
			{ 
				printf("altura invalido\n");
				menu();
			}

			printf("Posicao do centro (x): ");
			scanf("%f", &Figura.centro_x);
			if((((Figura.centro_x) - (Figura.lado)/3) < 0) || (((Figura.lado)/3 + (Figura.centro_x)) > 20))
			{ 
				printf("centro_x invalido\n");
				menu();
			}

			printf("Posicao do centro (y): ");
			scanf("%f", &Figura.centro_y);
			if((((Figura.centro_y) - (Figura.altura)/3) < 0) || (((Figura.altura)/3 + (Figura.centro_y)) > 20))
			{ 
				printf("centro_y invalido\n");
				menu();
			}

			printf("Velocidade de execuccao (cm/s): ");
			scanf("%f", &Figura.velocidade);

			return figura;
			break;

		default:
			printf("Opcao Invalida !\n");
			menu();

	}
}




//-------------------------------------------------------------------------------------------------------
//Abertura do socket//
void socket_open(int *clienteSocket, struct sockaddr_in *servidorAddr, char *IP_Servidor, unsigned short servidorPorta)
{
	// Criar Socket
	if((*clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Erro no socket()\n");

	// Construir struct sockaddr_in
	memset(servidorAddr, 0, sizeof(*servidorAddr)); // Zerando a estrutura de dados
	servidorAddr->sin_family = AF_INET;
	servidorAddr->sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr->sin_port = htons(servidorPorta);

	// Connect
	if(connect(*clienteSocket, (struct sockaddr *) servidorAddr, sizeof(*servidorAddr)) < 0)
		printf("Erro no connect()\n");

}

//Envia um request de para o servidor
void request(int *clienteSocket)
{

	if(send(*clienteSocket, &Figura, sizeof(struct TFigura), 0) != sizeof(struct TFigura))
		printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
	
}

