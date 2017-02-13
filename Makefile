CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o premiers.o suivants.o util.o

all: analyseurLL1

util: util.c
	$(CC) $(CCFLAGS) -o util.o -c util.c

premiers: premiers.c
	$(CC) $(CCFLAGS) -o premiers.o -c premiers.c

suivants: suivants.c
	$(CC) $(CCFLAGS) -o suivants.o -c suivants.c

analyseur_lexical: analyseur_lexical.o
	$(CC) $(CCFLAGS) -o analyseur_lexical analyseur_lexical.o

analyseurLL1: analyseurLL1.c analyseurLL1.h $(OBJ)
	$(CC) $(CCFLAGS) -o analyseurLL1 analyseurLL1.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c analyseur_lexical.h
	$(CC) $(CCFLAGS) -o analyseur_lexical.o -c analyseur_lexical.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
