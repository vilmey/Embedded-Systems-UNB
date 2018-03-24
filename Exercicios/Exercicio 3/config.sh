#!/bin/sh

## Uso <. ./nome_do_arquivo.sh>  Exportar as flags
## Uso <. ./nome_do_arquivo.sh programa.c executavel>  Exportar e compilar programa

#Configuracao das variaveis
export PATH=$PATH:/usr/xenomai/bin
export CFLAGS="$(xeno-config --skin=native --cflags)"
export LDFLAGS="$(xeno-config --skin=native --ldflags)"
export LD_LIBRARY_PATH=/usr/xenomai/lib

if [ $# -eq 2 ]
	then
		echo "Lista de exports feitas CFLAGS, LDFLAGS"
		echo "Arquivo sendo compilado <$1> "

		gcc $CFLAGS $LDFLAGS $1 -o $2

		echo "Executavel <$2> criado"

		exit 0
fi
	echo "Lista de exports feitas CFLAGS, LDFLAGS"
	echo "Para compilar - <. ./nome_do_arquivo.sh> <programa.c> <executavel>"

