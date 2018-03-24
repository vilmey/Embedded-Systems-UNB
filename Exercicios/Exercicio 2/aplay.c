#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

//Funcao para listar arquivos no diretorio atual, recebe o ponteiro para musica
void Listar(char *musica)
{
	int i=0, j=0, numero;
	char buffer[30], musicas[30][30];
	FILE *media;

	//Lista as musicas .wav para media.log
	system("ls | grep wav > media.log");

	media = fopen("media.log", "r");

	while(1)
	{
		int ret = fscanf(media, "%s \n", buffer);
		if(errno != 0)
		{
			perror("scanf:");
			break;
		}

		//Fim do arquivo, sair do loop
		else if(ret == EOF)
			break;

		strcpy(musicas[i],buffer);
		//memcpy(musicas[i],buffer, strlen(buffer));
		i++;	
	}


	fclose(media);

	//Printa as opcoes de musica
	for(j=0; j<i; j++)
		printf("Musica[%d]: %s\n", j, musicas[j]);

	printf("Digite o numero da musica: ");
	scanf("%d", &numero);

	//Passamos a musica selecionada para a variavel musica
	strcpy(musica,musicas[numero]);

}




int main()
{
	int i, fd, size =0;
	char buffer[256], musica[256];

	FILE *audio_fd;
	FILE *stream;

	while(1)
	{
		//Passa um ponteiro de char para receber o nome da musica
		Listar(musica);

		//Descritor para o arquivo de musica
		audio_fd = fopen(musica, "r");
		if(!audio_fd)
		{
			printf("Erro na abertura fopem");
		}

		//Abre um pipe entre o programa e o aplay
		stream = popen("aplay -t wav -", "w");
		if(!stream)
		{
			printf("Erro na abertura do pipe\n");
		}

		//Fileno retorna um descritor da stream no formato int.
		fd = fileno(stream);

		//Executa arquivo ate o FEOF
		while(!feof(audio_fd))
		{
			fread(buffer, sizeof(buffer), 1, audio_fd);
			write(fd,buffer,sizeof(buffer));
		}

		//Fecha o pipe e o arquivo de audio
		pclose(stream);
		fclose(audio_fd);
	}

	return 0;

}