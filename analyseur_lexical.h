#ifndef __LIRE_UNITE__
#define __LIRE_UNITE__
#define YYTEXT_MAX 99

#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void test_yylex_internal( FILE *yyin );
void affiche_token(int token);
#endif
