#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() 
{
	int fd;
	char * minhafifo = "/tmp/minha_fifo";

	mkfifo(minhafifo, 0666);

	fd = open(minhafifo, WROLNY);

	write(fd, "Ola", sizeof("Ola"));
	close(fd);

	unlink(minhafifo);
}