/*Compilação*/
//	gcc servidor_tcp.c play_alarm.c -o servidor_tcp

/*Execução*/
// ./servidor_tcp

//-------------------------------------------------------------------------------------------------------
/*Includes do programa*/
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "play_alarm.h"

//-------------------------------------------------------------------------------------------------------
/*Defines*/
#define DEVICE_FILE		"/dev/ttyAMA0"
#define DEBUG


//-------------------------------------------------------------------------------------------------------
/*Variáveis globais*/
char alarm_setor[] = {0, 0};
pthread_t alarm_id;
int uart0_filestream = -1;
unsigned short porta_4000 = 4034;
unsigned short porta_8080 = 8034;


//-------------------------------------------------------------------------------------------------------
/*Função de Log do sistema*/

void Log_data(char *clienteAddr, int setor, int dispositivo, int estado)
{
	FILE *fd;							//Descritor do arquivo

    time_t tempo;						//Struct da biblioteca time.h
	struct tm *data;
	time(&tempo);
	data=localtime(&tempo);
	
	fd = fopen("Client_log.txt", "a+");	//Abre o arquivo para incluir

	//HH:MM:SS - DD/MM/AAAA - Cliente <IP>; Setor <1-6>; Dispositivo <X>; Estado <0-1>
	fprintf(fd, "%d:%d:%d - %d/%d/%d",(*data).tm_hour,(*data).tm_min,(*data).tm_sec,(*data).tm_mday,(*data).tm_mon+1,(*data).tm_year+1900);
	fprintf(fd, " - Cliente %s; Setor %d; Dispositivo %d; Estado %d\n", clienteAddr, setor, dispositivo, estado);
	
	fclose(fd);							//Fecha o arquivo
}


//-------------------------------------------------------------------------------------------------------
/* Comunicação UART*/

//Configura os parametros da comunicacao UART
void Uart_Config(int uart0_filestream)
{
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;	// Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
}

//Recebimento de float via UART
void Recebe_float (int uart0_filestream, float *retorno)
{
	float temperatura;
	uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);

	int rx_length = read(uart0_filestream, &temperatura, sizeof(float));
		if (rx_length < 0)
			perror("Falha na leitura");
		
		else if (rx_length == 0)
			printf("Nenhum dado disponivel\n");

		else
			printf("\n|%i bytes lidos|", rx_length);


	close(uart0_filestream);

#ifdef DEBUG

	printf("\n\nRetorno float: %f\n\n\n", temperatura);
	*retorno = temperatura;

#endif
}

//Recebimento de char via UART
void Recebe_char (char * retorno, int tamanho, int uart0_filestream)
{
	int i;

	uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);
	int rx_length = read(uart0_filestream, retorno, tamanho);
		
	if (rx_length < 0)
		perror("Falha na leitura");

	else if (rx_length == 0)
		printf("Nenhum dado disponivel\n");

	else
		printf("| %i bytes lidos |", rx_length);

	close(uart0_filestream);

}

//Envio de char via UART
void Envia_char (char *Comando, int tamanho, int uart0_filestream)
{

#ifdef DEBUG
	printf("Comando : %X %d %X\n", Comando[0], Comando[1], Comando[2]);
#endif
	
	uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);
	int tx_length = write(uart0_filestream, Comando, tamanho);
		if (tx_length != tamanho)
			printf("Erro na transmissao - UART TX\n");
	
	close(uart0_filestream);
}

//-------------------------------------------------------------------------------------------------------
/*Thead de verificação dos sensores de presença, portas e janelas*/

void *Verifica_sensor()
{
	int i;
	char estado_sensores[9];
	char TODOS_OS_SENSORES = 0xA4;

	while(1)
	{
		Envia_char (&TODOS_OS_SENSORES, 1, uart0_filestream);
		sleep(2);
		Recebe_char (estado_sensores, 9, uart0_filestream);

#ifdef DEBUG
		/*for (i = 0; i < 9; i++)
		{
			printf("S[%d] %d, ",i, estado_sensores[i] );
		}
		printf("\n");*/
#endif
		
		/*Monitoramento do Setor 1*/
		if(alarm_setor[0] == 1)
		{
			for (i = 0; i < 3; i++)
			{
				if(estado_sensores[i] != 0)
				{
					//Toca Alarme
					printf("Toca o ALARME !!!\n");
					play_alarm_music(&alarm_setor[0]);
				}
				else
					sleep(2);
	
			}
		}
	
		/*Monitoramento do Setor 2*/
		if(alarm_setor[1] == 1)
		{
			for (i = 3; i < 9; i++)
			{
				if(estado_sensores[i] != 0)
				{
					//Toca Alarme
					printf("Toca o ALARME !!!\n");
					play_alarm_music(&alarm_setor[1]);
				}
				else
					sleep(2);
	
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------------
/*Funções de envio de dados ao cliente*/

void Envia_float_cliente(int socketCliente, struct sockaddr_in clienteAddr, float temperatura) {
	
	if(send(socketCliente, &temperatura, sizeof(float), 0) != sizeof(float))
		printf("Erro no envio - send()\n");
		
}

void Envia_char_cliente(int socketCliente, struct sockaddr_in clienteAddr, char mensagem) {
	
	if(send(socketCliente, &mensagem, sizeof(char), 0) != sizeof(char))
			printf("Erro no envio - send()\n");

}


//-------------------------------------------------------------------------------------------------------
/*Tratamento da informação recebida do cliente*/


void enviaArduino(char setor, char dispositivo, char estado, int socketCliente, struct sockaddr_in clienteAddr)
{
	//Declaração das variaveis
	char SETOR_TEMPERATURA 	= 0xA1;
	char SETOR_PRESENCA		= 0xA2;
	char SETOR_PORTAS 		= 0xA3;
	char SETOR_AC			= 0xB1;
	char SETOR_LAMPADAS		= 0xB2;
	char SETOR_ALARME		= 0xB3;
	char comando[3];
	char resposta;
	float temperatura;

	//Divisão dos dispositivos em setores
	switch(setor)
	{
		//Menu temperatura
		case 1:
			//Passando parâmetros para comando
			comando[0] = SETOR_TEMPERATURA;
			comando[1] = dispositivo;
			comando[2] = estado;

			//Enviando o comando para o arduino
			Envia_char (comando, 2, uart0_filestream);

			//Recebe resposta
			Recebe_float(uart0_filestream, &temperatura);

			//Envia resposta ao cliente
			Envia_float_cliente(socketCliente, clienteAddr, temperatura);

#ifdef DEBUG
			switch(dispositivo)
			{
				case 1:
					printf("Temperatura - Externa\n");
					break;

				case 2:
					printf("Temperatura - Interna da Sala\n");
					break;

				case 3:
					printf("Temperatura - Interna do quarto\n");
					break;
			}
#endif
			break;

		//Menu Presenca
		case 2:

			comando[0] = SETOR_PRESENCA;
			comando[1] = dispositivo;
			comando[2] = estado;
			Envia_char (comando, 2, uart0_filestream);
			Recebe_char(&resposta, 1, uart0_filestream);
			Envia_char_cliente(socketCliente, clienteAddr, resposta);

#ifdef DEBUG
			switch(dispositivo)
			{
				case 1:	
					printf("Presença - Entrada Principal\n");
					break;

				case 2:	
					printf("Presença - Entrada de Serviço\n");
					break;

				case 3:	
					printf("Presença - Garagem\n");
					break;

			}
#endif
			break;

		//Menu Portas/Janelas
		case 3:

			comando[0] = SETOR_PORTAS;
			comando[1] = dispositivo;
			comando[2] = estado;
			Envia_char (comando, 2, uart0_filestream);
			Recebe_char(&resposta, 1, uart0_filestream);
			Envia_char_cliente(socketCliente, clienteAddr, resposta);

#ifdef DEBUG			
			switch(dispositivo)
			{

				case 1:
					printf("Estado - Porta principal\n");
					break;

				case 2:
					printf("Estado - Porta de serviço\n");
					break;

				case 3:
					printf("Estado - Porta garagem\n");
					break;

				case 4:
					printf("Estado - Janela da sala\n");
					break;

				case 5:
					printf("Estado - Janela do quarto\n");
					break;

				case 6:
					printf("Estado - Janela da cozinha\n");
					break;
			}
#endif
			break;

		//Menu Ar-condicionado
		case 4:

			comando[0] = SETOR_AC;
			comando[1] = dispositivo;
			comando[2] = estado;
			Envia_char (comando, 3, uart0_filestream);

#ifdef DEBUG
			switch(dispositivo)
			{	
				case 1:
					printf("AC - Sala - %d\n", estado);
					break;

				case 2:
					printf("AC - Quarto - %d\n", estado);
					break;
			}
#endif
			break;

		//Menu Lampadas
		case 5:

			comando[0] = SETOR_LAMPADAS;
			comando[1] = dispositivo;
			comando[2] = estado;
			Envia_char (comando, 3, uart0_filestream);

#ifdef DEBUG
			switch(dispositivo)
			{
				case 1:
					printf("Lampada - Entrada principal - %d\n", estado);
					break;

				case 2:
					printf("Lampada - Entrada de serviço - %d\n", estado);
					break;

				case 3:
					printf("Lampada - Garagem - %d\n", estado);
					break;

				case 4:
					printf("Lampada - Sala - %d\n", estado);
					break;

				case 5:
					printf("Lampada - Quarto - %d\n", estado);
					break;

				case 6:
					printf("Lampada - Cozinha - %d\n", estado);
					break;
			}
#endif
			break;

		//Menu Alarme
		case 6:

			comando[0] = SETOR_ALARME;
			comando[1] = dispositivo;
			comando[2] = estado;
			Envia_char (comando, 3, uart0_filestream);
			
			if(dispositivo == 1)
				alarm_setor[0] = estado;

			if(dispositivo == 2)
				alarm_setor[1] = estado;

			if((alarm_setor[0] == 0) && (alarm_setor[1] == 0))


#ifdef DEBUG
			switch(dispositivo)
			{
				case 1:
					printf("Alarme - Setor 1 (Sensores de Presença) - %d\n", estado);
					break;

				case 2:
					printf("Alarme - Setor 2 (Sensores de Portas/ Janelas) - %d\n", estado);
					break;
			}
			break;
#endif
	}
}


void TrataClienteTCP(int socketCliente, struct sockaddr_in clienteAddr) {
	int buffer;
	int tamanhoRecebido;
	int setor, dispositivo, estado;

	if((tamanhoRecebido = recv(socketCliente, &buffer, sizeof(int), 0)) < 0)
		printf("Erro no recv()\n");


	setor = buffer/1000;
	dispositivo = (buffer - (setor*1000))/10;
	estado = buffer - setor*1000 - dispositivo*10;

	printf("Setor %d, Dispositivo %d, Estado %d\n", setor, dispositivo, estado);

	Log_data(inet_ntoa(clienteAddr.sin_addr), setor, dispositivo, estado);
	enviaArduino((char)setor, (char)dispositivo, (char)estado, socketCliente, clienteAddr);

}

void *server(void *porta)
{
	unsigned short *servidorPorta = (unsigned short *) porta;
	int servidorSocket;
	int socketCliente;
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned int clienteLength;

	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

	// Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(*servidorPorta);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
		printf("Falha no Bind\n");

	// Listen
	if(listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");		

	while(1) {

		printf("Esperando por clientes na porta %d\n", *servidorPorta);
		clienteLength = sizeof(clienteAddr);
		if((socketCliente = accept(servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) < 0)
			printf("Falha no Accept\n");
		
		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		TrataClienteTCP(socketCliente, clienteAddr);
		close(socketCliente);

	}
	close(servidorSocket);
}

int main(int argc, char *argv[]) {

	pthread_t server_8080;
	pthread_t server_4000;

	Uart_Config(uart0_filestream);

	pthread_create(&server_4000, NULL, &server, &porta_4000);
	pthread_create(&server_8080, NULL, &server, &porta_8080);
	pthread_create(&alarm_id, NULL, &Verifica_sensor, NULL);

	pthread_join(server_8080, NULL);
	pthread_join(server_4000, NULL);

}