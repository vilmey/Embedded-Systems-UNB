#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void TrataClienteTCP(int socket);

int main(int argc, char * argv[])
{
	int socket_Servidor;
	int socket_Cliente;
	struct sockaddr_in servidorAddr, clienteAddr;
	unsigned short porta_servidor;
	unsigned int length_cliente;

	if(argc != 2)
	{
		printf("Uso: %s <porta do servidor>\n", argv[0]);
		exit (1);
	}
	porta_servidor = atoi(argv[1]); //Converte string para inteiro

	//Abrir socket
	if((socket_Servidor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Falha no socket\n");
		exit (1);
	}

	//Preparar estrutura SockAddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr));

	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(porta_servidor);


	//Bind
	if((bind(socket_Servidor, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr))) < 0)
	{
		printf("Erro no bind()\n");
		exit(1);
	}

	//Listen
	if(listen(socket_Servidor, 10) < 0)
	{
		printf("Erro no listen()\n");
		exit(1);
	}

	//Accept
	while(1)
	{
		length_cliente = sizeof(clienteAddr);
		if((socket_Cliente = accept(socket_Servidor, (struct sockaddr *) &clienteAddr, &length_cliente)) < 0)
		{
			printf("Erro accept()\n"); //Nao precisa de exit(1), pois podemos conectar com outro cliente
		}

		TrataClienteTCP(socket_Cliente);
		close(socket_Cliente);
	}

	close(socket_Servidor);
	return 0;
}


void TrataClienteTCP(int socket)
{
	char buffer[20];
	int tamanho_recebido;

	//Receive
	if((tamanho_recebido = recv(socket, buffer, 20, 0)) < 0)
	{
		printf("Erro no recv()\n");
	}

	while(tamanho_recebido > 0)
	{
		if((send(socket, buffer, tamanho_recebido, 0))  < 0 )
		{
			printf("Erro no send()\n");
		}

		//Caso mandarmos um arquivo com mais de 20 bytes
		if((tamanho_recebido = recv(socket, buffer, 20, 0)) < 0)
		{
			printf("Erro no recv()\n");
		}
	}
}