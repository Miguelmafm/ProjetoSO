#*****************************************************************
#			Sistemas Operativos		- Projecto 2017/2018
#
# 		Miguel Marques			- nº 2068614
# 		Rúben Marques				- nº 2072212
# 		Vitor Paixão				- nº 2021212
#*****************************************************************

RUNMO = ./monitor
RUNSI = ./simulador
CC = gcc -o
CFLAGS = -g
LIBS = -lns -lnsl
LIBS = -lm -lnsl -lpthread

all: clean monitor simulador runm

monitor: monitor.o util.o util_monitor.o
	$(CC) -g -o monitor monitor.o  util.o util_monitor.o $(LIBS)

simulador: simulador.o util.o util_simulador.o
	$(CC) -g -o simulador simulador.o util.o util_simulador.o $(LIBS)\

runm:
	x-terminal-emulator --geometry "5x5" -e "bash -c $(RUNSI);bash"; resize -s 37 80; clear; $(RUNMO);

clean:
	rm -f *.o *~ simulador
	rm -f *.o *~ monitor
