CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o

all: analyseur_syntaxique

analyseur_lexical: analyseur_lexical.o
	$(CC) $(CCFLAGS) -o analyseur_lexical analyseur_lexical.o

analyseur_syntaxique: analyseur_syntaxique.c $(OBJ)
	$(CC) $(CCFLAGS) -o analyseur_syntaxique analyseur_syntaxique.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c analyseur_lexical.h
	$(CC) $(CCFLAGS) -o analyseur_lexical.o -c analyseur_lexical.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
