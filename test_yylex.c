#include <stdio.h>
#include <stdlib.h>
#include "lexique.h"
#include "unitesLexicales.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
  int uniteCourante;
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante = yylex();
  while (uniteCourante != '.') {
    printf("%s, %d\n", yytext, uniteCourante);
    uniteCourante = yylex();
  }
  printf("%s, %d\n", ".", uniteCourante);  
  return 0;
}
