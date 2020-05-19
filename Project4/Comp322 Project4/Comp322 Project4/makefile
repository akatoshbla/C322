all: djk93533
djk93533: djk93533.o pid.o
	gcc -lpthread -o djk93533 djk93533.o pid.o
pid.o: pid.c pid.h
	gcc -c pid.c
djk93533.o: djk93533.c pid.h
	gcc -lpthread -c djk93533.c