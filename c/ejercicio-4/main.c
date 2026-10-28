/* 
 * Crear un programa que genere dos procesos hijos.
 *
 * El padre debe esperar por la terminación de ambos procesos hijos.
 *     Antes debe imprimir por pantalla “Proceso padre esperando...”, 
 *     y posteriormente imprimirá “Hijo PID_1er_hijo finalizó” 
 *     e “Hijo PID_2do_hijo finalizó”.
 *
 * El primer proceso hijo debe realizar un exec imprimiendo en pantalla 
 *     el calendario del mes y año (formato mm aaaa). Parámetros establecidos 
 *     por el padre y recibidos por línea de comandos (el usuario lo introduce).
 *
 * El segundo hijo deberá recibir una señal SIGUSR1 y mediante un handler
 *     emitirá por pantalla el mensaje “Recibida la señal SIGUSR1” y luego 
 *     finalizará con _exit().
 *
 * Finalmente el proceso padre mostrará por pantalla su propio PID y finalizará.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handler_sigusr1(int signum);

int main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(stderr, "Uso: %s, <mm> <aaaa>", argv[0]);
		return 1;
	}

	pid_t hijo1 = fork();

	switch(hijo1) {
		case -1:
			perror("fork 1");
			_exit(1);

		case 0:
			char *args[] = {"cal", argv[1], argv[2], NULL};
			execv("/usr/bin/cal", args);
			perror("execv");
			_exit(0);
	}

	pid_t hijo2 = fork();

	switch(hijo2) {
		case -1:
			perror("fork 2");
			_exit(1);

		case 0:
			signal(SIGUSR1, handler_sigusr1);
			while(1)
				pause();
	}

	printf("Proceso padre esperando...\n");

	int status1;
	if(waitpid(hijo1, &status1, 0) != -1)
		printf("Hijo %d finalizó\n", (int)hijo1);

	int status2;
	if(waitpid(hijo2, &status2, 0) != -1)
		printf("Hijo %d finalizó\n", (int)hijo2);

	printf("%d", getpid());

	return 0;
}

void handler_sigusr1(int signum)
{
	printf("Recibida la señal SIGUSR1\n");
	_exit(0);
}
