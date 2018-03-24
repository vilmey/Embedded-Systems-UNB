#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct char_print_parms
{
	char charactere;
	int count;
};

void *char_print (void *parameters)
{
	struct char_print_parms *p = (struct char_print_parms*) parameters;
	
	int i;
	for(i=0; i < p->count; i++)
		printf("%c", p->charactere);

	return NULL;
}


int main ()
{
	pthread_t thread_id;
	pthread_t thread_id2;

	struct char_print_parms thread_args;
	struct char_print_parms thread2_args;
	
	thread_args.charactere = 'x';
	thread_args.count = 3000;
	
	thread2_args.charactere = 'o';
	thread2_args.count = 2000;

	pthread_create(&thread_id, NULL, &char_print, &thread_args);
	pthread_create(&thread_id2, NULL, &char_print, &thread2_args);
	pthread_join(thread_id, NULL);
	pthread_join(thread_id2, NULL);
	

	return 0;

}