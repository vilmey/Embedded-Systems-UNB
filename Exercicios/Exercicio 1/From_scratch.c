#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define DEVICE_FILE		"/dev/ttyACM0"


//Configura os parametros da comunicacao UART
void Uart_Config(int * uart0_filestream)
{
	struct termios options;
	tcgetattr(*uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;	// Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(*uart0_filestream, TCIFLUSH);
	tcsetattr(*uart0_filestream, TCSANOW, &options);
}

//Constroi o menu de opcoes
void Menu ()
{
	printf("(1) Solicita int\n");
	printf("(2) Solicita float\n");
	printf("(3) Solicita char\n");
	printf("(4) Envia int\n");
	printf("(5) Envia float\n");
	printf("(6) Envia char\n");

	printf("Digite uma opção: ");
}


void Solicita_int (char Comando, char *matricula, int *uart0_filestream)
{
	int retorno;
	char chave[60] = {Comando};

	memcpy(&chave[1], matricula, 4);

	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);

	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");


	int rx_length = read(*uart0_filestream, &retorno, sizeof(int));
		if (rx_length < 0)
			perror("Falha na leitura");

		else if (rx_length == 0)
			printf("Nenhum dado disponivel\n");

		else
			printf("\n|%i bytes lidos|", rx_length);

	close(*uart0_filestream);

	printf("\n\nRetorno int: %d\n\n\n", retorno);
}

void Solicita_float (char Comando, char *matricula, int *uart0_filestream)
{
	float retorno;
	char chave[60] = {Comando};

	memcpy(&chave[1], matricula, 4);

	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);

	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");

	int rx_length = read(*uart0_filestream, &retorno, sizeof(float));
		if (rx_length < 0)
			perror("Falha na leitura");
		
		else if (rx_length == 0)
			printf("Nenhum dado disponivel\n");

		else
			printf("\n|%i bytes lidos|", rx_length);


	close(*uart0_filestream);

	printf("\n\nRetorno float: %f\n\n\n", retorno);
}

void Solicita_char (char Comando, char *matricula, int *uart0_filestream)
{
	char retorno[256], string[256];
	char chave[60] = {Comando};

	memcpy(&chave[1], matricula, 4);
	
	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);
	
	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");

	sleep(1);

	int rx_length = read(*uart0_filestream, retorno, 255);
		if (rx_length < 0)
			perror("Falha na leitura");
		
		else if (rx_length == 0)
			printf("Nenhum dado disponivel\n");

		else
			printf("| %i bytes lidos |", rx_length);

	close(*uart0_filestream);

	printf("\n\nTamanho recebido : %d \n", (int)retorno[0]); //Imprime o tamanho da string recebida
	printf("Retorno string: (%s) (%s)\n\n\n", string, retorno);
}

void Envia_int (char Comando, char *matricula, int *uart0_filestream)
{
	int i, retorno;
	char chave[60] = {Comando};

	printf("\nDigite o inteiro a ser enviado: ");
	scanf("%d", &i);
	
	memcpy(&chave[1], &i, 4);
	memcpy(&chave[5], matricula, 4);


	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);

	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");

	int rx_length = read(*uart0_filestream, &retorno, sizeof(int));
	if (rx_length < 0)
		perror("Falha na leitura");
	
	else if (rx_length == 0)
		printf("Nenhum dado disponivel \n");

	else
		printf("\n|%i bytes lidos |", rx_length);


	close(*uart0_filestream);

	printf("\nRetorno int: %d\n\n\n", retorno);
}

void Envia_float (char Comando, char *matricula, int *uart0_filestream)
{
	float i, retorno;
	char chave[60] = {Comando};
	
	printf("\nDigite o real a ser enviado: ");
	scanf("%f", &i);
	
	memcpy(&chave[1], &i, 4);
	memcpy(&chave[5], matricula, 4);
	

	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);

	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");

	int rx_length = read(*uart0_filestream, &retorno, sizeof(float));
		if (rx_length < 0)
			perror("Falha na leitura \n");
		
		else if (rx_length == 0)
			printf("Nenhum dado disponivel \n");

		else
			printf("\n|%i bytes lidos |", rx_length);

	close(*uart0_filestream);

	printf("\nRetorno float: %f\n\n\n", retorno);
}


void Envia_char (char Comando, char *matricula, int *uart0_filestream)
{
	char retorno[256];
	int tam;
	char tamanho_msg, buffer[60], chave[60] = {Comando};
	
	printf("\nDigite a string a ser enviada: ");
	
	getchar();
	fgets(buffer, 60, stdin); //String a ser enviada
   	tamanho_msg = (char)strlen(buffer);

   	buffer[strcspn(buffer, "\n")] = 0;
	
	memcpy(&chave[1], &tamanho_msg, 1);
	memcpy(&chave[2], buffer, (int)strlen(buffer));
	memcpy(&chave[2+((int)strlen(buffer))], matricula, 4);

	
	*uart0_filestream = open(DEVICE_FILE, O_RDWR | O_NOCTTY |O_NDELAY);
	int tx_length = write(*uart0_filestream, chave, (int)strlen(chave));
		if (tx_length < 0)
			printf("Erro na transmissao - UART TX\n");

	int rx_length = read(*uart0_filestream, retorno, 1);
		if (rx_length < 0)
			perror("Falha na leitura");
		
		else if (rx_length == 0)
			printf("Nenhum dado disponivel \n");

		else
			printf("\n|%i bytes lidos |", rx_length);
	
	close(*uart0_filestream);

	printf("\nRetorno string: %s\n\n\n", retorno);
}

int main ()
{	
	int uart0_filestream = -1;
	char matricula[4] = {'1', '3', '8', '0'};
	char RCV_INTEGER 	= 0xA1;
	char RCV_FLOAT		= 0xA2;
	char RCV_CHAR 		= 0xA3;
	char SND_INTEGER	= 0xB1;
	char SND_FLOAT		= 0xB2;
	char SND_CHAR		= 0xB3; 

	Uart_Config(&uart0_filestream);


	while(1)
	{
		int opcao;

		Menu();

		scanf("%d", &opcao);
		system("clear");

		switch(opcao)
		{
			case 1:
				Solicita_int(RCV_INTEGER, matricula, &uart0_filestream);
				break;
			case 2:
				Solicita_float(RCV_FLOAT, matricula, &uart0_filestream);
				break;
			case 3:
				Solicita_char(RCV_CHAR, matricula, &uart0_filestream);
				break;
			case 4:
				Envia_int(SND_INTEGER, matricula, &uart0_filestream);
				break;
			case 5:
				Envia_float(SND_FLOAT, matricula, &uart0_filestream);
				break;
			case 6:
				Envia_char(SND_CHAR, matricula, &uart0_filestream);
				break;

			default:
				printf("Opção Inválida !\n");
		}
	}

	return 0;
}

