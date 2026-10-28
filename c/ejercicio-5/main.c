/* Crear un programa denominado my_cat que imite al comando UNIX cat. 
 *
 * El mismo concatena el contenido de uno o más archivos en un único flujo de caracteres
 *	(por defecto) dirigido a stdout. Su uso será:
 *	my_cat <archivo1> <archivo2> ... [-o <archivo_salida>]
 *
 *	Al pasar la opción -o en lugar de escribir a la salida estándar (pantalla)
 *	escribirá a un archivo llamado <archivo_salida>.
 *
 * Nota: Usar las llamadas al sistema: open, write, read, close 
 *	y cualquier otra que sea necesaria.
 *
*/

#include <stdio.h>
#include <fcntl.h> /* open() */
#include <unistd.h> /* read(), write() */
#include <string.h> /* strcmp() */

#define BUFFER_SIZE 1024

void print_file(const int argc, const char *argv[]);
void write_file(const int argc, const char *argv[], const int idx);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "uso: %s <archivo1> <archivo2> ... [-o <archivo_salida>]", argv[0]);
		return 1;
	}

	int idx = 0;

	/* Para saber el indice donde esta la bandera '-o' */
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-o") != 0)
			continue;
		idx = i;
	}

	switch (idx) {
		case 0:
			print_file((const int) argc, (const char **) argv);
			break;
		default:
			write_file((const int) argc, (const char **) argv, idx);
			break;
	}

	return 0;
}

void print_file(const int argc, const char *argv[])
{
	char buf[BUFFER_SIZE];
	ssize_t readBytes;

	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);

		if (fd < 0) {
			perror(argv[i]);
			continue;
		}

		while ((readBytes = read(fd, buf, BUFFER_SIZE - 1)) > 0) {
			buf[readBytes] = '\0';
			printf("%s", buf);
		}

		close(fd);
	}
}

void write_file(const int argc, const char *argv[], const int idx)
{
	char buf[BUFFER_SIZE];
	ssize_t readBytes;
	ssize_t writtenBytes;
	int fd;
	int fdOut;

	fdOut = open(argv[idx + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	/* En caso de que -o sea el primer argumento de myCat se escribe lo que esta en stdin */
	if (idx == 1) {
		fd = 0;

		while ((readBytes = read(fd, buf, BUFFER_SIZE - 1)) > 0) {
			buf[readBytes] = '\0';
			writtenBytes = write(fdOut, buf, readBytes);
		}
			if (writtenBytes < 0) {
				perror("write");
				_exit(1);
			}
	}

	for (int i = 1; i < argc; i++) {
		if (i == idx || i == idx + 1) 
			continue;

		fd = open(argv[i], O_RDONLY);

		if (fd < 0) {
			perror(argv[i]);
			continue;
		}

		while ((readBytes = read(fd, buf, BUFFER_SIZE - 1)) > 0) {
			buf[readBytes] = '\0';
			writtenBytes = write(fdOut, buf, readBytes);

			if (writtenBytes < 0) {
				perror("write");
				continue;
			}
		}

		close(fd);
	}

	close(fdOut);
}
