CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb -w -g

OBJ = analyseur_lexical.o premiers.o suivants.o util.o syntabs.o affiche_arbre_abstrait.o creationTabSymb.o tabsymboles.o

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

syntabs.o: syntabs.c util.o
	$(CC) $(CCFLAGS) -o syntabs.o -c syntabs.c

affiche_arbre_abstrait.o: affiche_arbre_abstrait.c syntabs.o util.o
	$(CC) $(CCFLAGS) -o affiche_arbre_abstrait.o -c affiche_arbre_abstrait.c

creationTabSymb.o: creationTabSymb.c syntabs.o util.o
	$(CC) $(CCFLAGS) -o creationTabSymb.o -c creationTabSymb.c 

tabsymboles.o: tabsymboles.c util.o
	$(CC) $(CCFLAGS) -o tabsymboles.o -c tabsymboles.c util.o



.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
