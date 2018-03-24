//Servidor UDP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main ()
{
	int socket_servidor;
	struct sockaddr_in si_servidor, si_cliente;

	socket_servidor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_servidor == -1)
	{
		printf("Erro na abertura do socket\n");
		exit(1);
	}

	memset((char *) &si_servidor, 0, sizeof(si_servidor));
	
	si_servidor.sin_family 			= AF_INET;
	si_servidor.sin_port			= htons(3000);
	si_servidor.sin_addr.s_addr 	= htonl(INADDR_ANY);

	if(bind(socket_servidor, (struct sockaddr*) &si_servidor, sizeof(si_servidor)) == -1)
	{
		printf("Erro no bind\n");
		exit(1);
	}

	while(1)
	{
		printf("Aguardando mensagens ...\n");

		char buffer[1024];
		memset(buffer, '\0', 1024);

		int t_mensagem, t_cliente;

		//Recebendo a mensagem
		t_mensagem = recvfrom(socket_servidor, buffer, 1024, 0, (struct sockaddr*) &si_cliente, &t_cliente);

		printf("Buffer - %s\n", buffer);

		//Enviando a mensagem
		if(sendto(socket_servidor, buffer, t_mensagem, 0, (struct sockaddr*) &si_cliente, sizeof(si_cliente)) == -1)
		{
			printf("Erro no sendto\n");
			exit(1);
		}
		
		memset(buffer, '\0', 1024);


	}

	close(socket_servidor);

	return 0;
}