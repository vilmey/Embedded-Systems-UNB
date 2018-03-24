#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_xs (void *c)
{
	char *charactere = (char *) c;
	while(1)
		fputc(*charactere, stderr);

	//return NULL;
}


int main ()
{
	pthread_t thread_id;
	pthread_t thread_id2;

	char c = '1';
	char c2 = '*';

	pthread_create(&thread_id, NULL, &print_xs, &c);
	pthread_create(&thread_id2, NULL, &print_xs, &c2);

	while(1)
		fputc('-', stderr);

	return 0;

}