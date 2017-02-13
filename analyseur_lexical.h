#ifndef __LIRE_UNITE__
#define __LIRE_UNITE__
#define YYTEXT_MAX 99

#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void test_yylex_internal( FILE *yyin );
char* affiche_tokenBis(int token);

char yytext[YYTEXT_MAX];
int yyleng;
FILE *yyin;

#endif
