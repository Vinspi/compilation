#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "unitesLexicales.h"

/***********************

ATTENTION !
<= et >= sont a inclure en tant qu'élément de grammaire reconnaissable dans unitesLexicales.h et modifier le code en consequence dans analyseur_lexical.c

**********************/

/* grammaire :
  EBOOLEAN -> COMPARAISON ou EBOOLEAN | COMPARAISON & EBOOLEAN | !COMPARAISON | COMPARAISON ***** à implementer *****
  COMPARAISON -> EXPRESSION < EXPRESSION | EXPRESSION > EXPRESSION | EXPRESSION = EXPRESSION | EXPRESSION >= EXPRESSION | EXPRESSION <= EXPRESSION | EXPRESSION != EXPRESSION | E ***** à implementer *****
  EXPRESSION -> T ADDITION
  ADDITION -> +EXPRESSION | -EXPRESSION | epsilon
  T -> FOIS TP
  TP -> xFOIS | /FOIS | epsilon
  FOIS -> (EXPRESSION) | nombre | ID_VAR
*/

int uniteCourante;

void EXPRESSION(){
  T();
  addition();
}

void T(){
  fois();
  TP();
}

void COMPARAISON(){
    EXPRESSION();
    if(uniteCourante == INFERIEUR || uniteCourante == SUPERIEUR){
      uniteCourante = yylex();
      if(uniteCourante == EGAL)
        uniteCourante = yylex();
        //EXPRESSION();
    }
    else if(uniteCourante == NON){
      uniteCourante = yylex();
      if(uniteCourante == EGAL){
        uniteCourante = yylex();
      }
      //EXPRESSION();
    }
    else if(uniteCourante == EGAL){
      printf("uniteCourante = %d", uniteCourante);
      uniteCourante = yylex();
      printf("uniteCourante = %d", uniteCourante);
      //EXPRESSION();
    }
    else{
      return;
    }
    EXPRESSION();
}

void fois(){
  if(uniteCourante == PARENTHESE_OUVRANTE){
    uniteCourante = yylex();
    EXPRESSION();
    if(uniteCourante == PARENTHESE_FERMANTE){
      uniteCourante = yylex();
      return;
    }
    else{
      printf("%s\n", "Erreur de syntaxe");
      exit(1);
    }
  }
  if(uniteCourante == FIN){
    return;
  }
  if(uniteCourante == NOMBRE || uniteCourante == ID_VAR){
    uniteCourante = yylex();
    return;
  }

  printf("%s\n", "Erreur de syntaxe");
  exit(1);

}

void TP() {
  if(uniteCourante == FOIS || uniteCourante == DIVISE){
    uniteCourante = yylex();
    EXPRESSION();
  }
  if(uniteCourante == FIN){
    return;
  }
}

void addition(){
  if(uniteCourante == PLUS || uniteCourante == MOINS){
    uniteCourante = yylex();
    EXPRESSION();
  }
  if(uniteCourante == FIN){
    return;
  }
}

int main() {
  /* test */
  yyin = fopen("testSyntax.l","r");
  uniteCourante = yylex();
  printf("%s %d\n", "unite courante : ",uniteCourante);
  COMPARAISON();
  if(uniteCourante == FIN)
    printf("%s\n", "Syntaxe correcte");
  else printf("%s\n", "erreur de syntaxe");

  return 0;
}
