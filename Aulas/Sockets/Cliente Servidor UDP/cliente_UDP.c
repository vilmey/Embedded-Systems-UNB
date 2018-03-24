//Servidor UDP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main ()
{
	int socket_cliente, si_len;
	char mensagem[1024];
	struct sockaddr_in si_servidor;
	char buffer[1024];

	socket_cliente = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_cliente == -1)
	{
		printf("Erro na abertura do socket\n");
		exit(1);
	}

	memset((char *) &si_servidor, 0, sizeof(si_servidor));
	
	si_servidor.sin_family 			= AF_INET;
	si_servidor.sin_port			= htons(3000);
	inet_aton("127.0.0.1", &si_servidor.sin_addr);

	si_len = sizeof(si_servidor);

	while(1)
	{
		
		printf("Mensagem a ser enviada: ");
		gets(mensagem);

		if(sendto(socket_cliente, mensagem, strlen(mensagem), 0, (struct sockaddr*) &si_servidor, si_len) == -1)
		{
			printf("Erro sendto\n");
			exit(1);
		}

		//Recebe resposta do servidor
		memset(buffer, '\0', 1024);
		if(recvfrom(socket_cliente, buffer, 1024, 0, (struct sockaddr*) &si_servidor, &si_len) == -1)
		{
			printf("Erro recvfrom\n");
			exit(1);
		}

		printf("Resposta do servidor: %s\n", buffer);
	}

	close(socket_cliente);

	return 0;
}