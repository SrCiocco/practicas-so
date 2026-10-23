#!/bin/bash

# Escribir un script que reciba n argumentos.
#
# - Si el argumento recibido es un número > 10 o es una secuencia de chars > 10
#	la lista en un archivo llamado long.txt
#
# - En caso contrario la lista en el archivo start.txt
#
# Debe consumirse todos los argumentos y exigirse al menos uno.

if [[ $# == 0 ]]; then
	echo "Error: No se proporcionaron argumentos."
	echo "Uso: $0 <argumento1> <argumento2> ... <argumentoN>"
	exit 1
fi

mkdir -p output
> "output/long.txt"
> "output/start.txt"

for arg in "$@"; do
	longitud=${#arg}
	if [[ $arg =~ ^[0-9]+$ ]]; then
		if [[ $arg -gt 10 ]]; then
			echo "$arg" >> "output/long.txt"
		else 
			echo "$arg" >> "output/start.txt"
		fi
	else
		if [[ $longitud -gt 10 ]]; then
			echo "$arg" >> "output/long.txt"
		else
			echo "$arg" >> "output/start.txt"
		fi
	fi
done
