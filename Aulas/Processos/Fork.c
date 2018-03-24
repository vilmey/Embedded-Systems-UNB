#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
	pid_t child_pid;

	printf("The main program PID: %d\n", (int)getpid());

	//Criamos um processo filho identico ao processo pai
	child_pid = fork();	//Child_pid vai receber o PID do filho (caso seja o pai)
						//Se for o processo filho child_pid recebe 0 !!!

	//Se for o processo pai
	if(child_pid != 0)
	{
		printf("This is the parent process ID %d\n", (int)getpid());
		printf("Child PID %d \n", (int)child_pid);
	}

	//Se for o processo filho, retorna 0
	else
		printf("This is the child process, PID: %d\n", (int)child_pid);

	return 0;
}
