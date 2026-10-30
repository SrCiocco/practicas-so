/* Escriba un código que realice lo siguiente.
 * El proceso padre debe crear exactamente tres hijos.
 * El primer hijo creado debe finalizar voluntariamente luego de imprimir su pid.
 * El segundo hijo creado debe quedar iterando en un bucle infinito a la espera de la señal 
 * SIGUSR1, cuando llega la gestiona y hace que el proceso termine imprimiendo "chau soy 
 * el hijo PID".
 * El hijo restante debe "convertirse" en el proceso "ps -ef".
 * El padre termina con un mensaje por pantalla que dice "Fin Padre Parcial", pero debe 
 * esperar por la finalización del tercer proceso hijo.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handle_sig(int signum);

int main(void)
{
	pid_t hijo1 = fork();

	switch (hijo1) {
		case -1:
			perror("fork");
			_exit(1);
		case 0:
			printf("Hijo 1 pid: %d\n", getpid());
			_exit(0);
	}
	
	pid_t hijo2 = fork();

	switch (hijo2) {
		case -1:
			perror("fork");
			_exit(1);
		case 0:
			signal(SIGUSR1, handle_sig);
			while(1)
				pause();
			_exit(1);
	}
	
	pid_t hijo3 = fork();

	switch (hijo3) {
		case -1:
			perror("fork");
			_exit(1);
		case 0:
		{
			char *args[] = {"ps", "-ef", NULL};
			execv("/bin/ps", args);
			perror("exec");
			_exit(1);
		}
	}

	if (waitpid(hijo3, NULL, 0) < 0)
	{
		perror("waitpid");
		_exit(1);
	}

	printf("Fin padre parcial\n");
	return 0;
}

void handle_sig(int signum)
{
	printf("Recibida señal %d SIGUSR1\n", signum);
	printf("Chau soy el hijo %d\n", getpid());

	_exit(0);
}
