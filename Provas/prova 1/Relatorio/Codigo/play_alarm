#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>


void play_alarm_music( char *estado)
{
	int i, fd, size =0;
	char buffer[256], musica[256];

	FILE *audio_fd;
	FILE *stream;

	while(*estado == 1)
	{
		//Descritor para o arquivo de musica
		audio_fd = fopen("Tornado_Siren.wav", "r");
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
}
