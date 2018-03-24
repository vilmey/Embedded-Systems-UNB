#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(int argc, char * argv[])
{
	int socket_cliente;
	struct sockaddr_in servidorAddr;
	unsigned short porta_servidor;
	char *IP_Servidor;
	int bytesRecebidos;


	if(argc < 2)
	{
		printf("Uso: %s <IP_Servidor> <Porta> \n", argv[0]);
		exit(0);
	}

	IP_Servidor = argv[1];
	porta_servidor = atoi(argv[2]);
	

	//Abrir socket
	if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
	{
		printf("Erro no socket()\n");
		exit(0);
	}

	//Configurar struct
	memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(porta_servidor);

	//Connect
	if((connect(socket_cliente, (struct sockaddr*) &servidorAddr, sizeof(servidorAddr))) < 0)
	{
		printf("Erro connect()\n");
		exit(0);
	}



//---------------------------------------------------------------------------------------------

	char request[10];				//Mensagem que sera enviada ao servidor
	unsigned int tamanhorequest;
	int matricula = 1380;			//4 ultimos digitos da matricula

	/*Mensagem de Inicio de comunicacao*/

	request[0] = 0xAA;				//Codigo para envio da matricula
	request[1] = 4;					//Tamanho da mensagem
	memcpy(&request[2], &matricula, 4);

//---------------------------------------------------------------------------------------------
	/*Inicio da Comunicacao */

	tamanhorequest = strlen(request);
	if((send(socket_cliente, request, tamanhorequest, 0)) < 0)
	{
		printf("Erro no send()\n");
		exit(0);
	}


//---------------------------------------------------------------------------------------------
	/*Recebe a resposta do servidor*/

	char codigo_inteiro, tamanho_inteiro;
	int inteiro;

	//Recebe o codigo do inteiro
	if((bytesRecebidos = recv(socket_cliente, &codigo_inteiro, 1, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	

	//Recebe o tamanho do inteiro
	if((bytesRecebidos = recv(socket_cliente, &tamanho_inteiro, 1, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	
	//Recebe o inteiro
	if((bytesRecebidos = recv(socket_cliente, &inteiro, 4, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	

//---------------------------------------------------------------------------------------------
	/*Envia o inteiro multiplicado por 2*/

	inteiro = 2*inteiro;
	request[0] = 0xAB;				//Codigo para envio de inteiro
	request[1] = 4;					//Tamanho da mensagem
	memcpy(&request[2], &inteiro, 4);

	printf("2* Inteiro %d\n", inteiro);

	//Envio da mensagem ao servidor
	if((send(socket_cliente, &request, 6, 0)) < 0)
	{
		printf("Erro no send()\n");
		exit(0);
	}

//---------------------------------------------------------------------------------------------
	/*Recebendo a string*/

	char codigo_string, tamanho_string;

	//Recebe o codigo da string
	if((bytesRecebidos = recv(socket_cliente, &codigo_string, 1, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	

	//Recebe o tamanho da string
	if((bytesRecebidos = recv(socket_cliente, &tamanho_string, 1, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	


	//Define o tamanho da string
	char request_recebida[(int)tamanho_string+1];

	if((bytesRecebidos = recv(socket_cliente, &request_recebida, (int)tamanho_string, 0)) <= 0)
	{
		printf("Os bytes nao foram recebidos\n");
	}
	request_recebida[(int)tamanho_string+1] = '\0';
	
//----------------------------------------------------------------------------------------------
	/*Impreme Resultados*/

	printf("Codigo int: %X\n", codigo_inteiro);
	printf("Tamanho int: %d\n", tamanho_inteiro);
	printf("Inteiro: %d\n", inteiro);

	printf("Codigo string: %X\n", codigo_string);
	printf("Tamanho string %d\n", tamanho_string);
	printf("String |%s|\n", request_recebida);



	close(socket_cliente);

	return 0;
}