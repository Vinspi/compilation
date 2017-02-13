#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "unitesLexicales.h"

/***********************

ATTENTION !
<= et >= sont a inclure en tant qu'élément de grammaire reconnaissable dans unitesLexicales.h et modifier le code en consequence dans analyseur_lexical.c

**********************/

/* grammaire :

  EBOOLEAN -> COMPARAISON ou EBOOLEAN | COMPARAISON & EBOOLEAN | !EBOOLEAN | COMPARAISON ***** à implementer *****
  COMPARAISON ->  EXPRESSION < COMPARAISON | EXPRESSION > COMPARAISON | EXPRESSION = COMPARAISON | EXPRESSION >= COMPARAISON | EXPRESSION <= COMPARAISON | EXPRESSION != COMPARAISON | EXPRESSION ***** à implementer *****
  EXPRESSION -> T ADDITION
  ADDITION -> +EXPRESSION | -EXPRESSION | epsilon
  T -> FOIS TP
  TP -> xFOIS | /FOIS | epsilon
  FOIS -> (COMPARAISON) | nombre | ID_VAR
*/
FILE *test;
int uniteCourante;

void EXPRESSION(){
  fprintf(test, "%s\n", "<EXPRESSION>");
  T();
  addition();
  fprintf(test, "%s\n", "</EXPRESSION>");
}

void T(){
  fprintf(test, "%s\n", "<T>");
  fois();
  TP();
  fprintf(test, "%s\n", "</T>");
}
/*
void EBOOLEAN( ) {
  fprintf(test, "%s\n", "<EBOOLEAN>");

  if(uniteCourante == NON){
      uniteCourante = yylex();
      if(uniteCourante == FIN){
        printf("%s\n", "erreur de syntaxe");
        exit(1);
      }
      EBOOLEAN();
      fprintf(test, "%s\n", "</EBOOLEAN>");
      return;
  }
  COMPARAISON();
  if(uniteCourante == OU || uniteCourante == ET){
    uniteCourante = yylex();
    if(uniteCourante == FIN){
      printf("%s\n", "erreur de syntaxe");
      exit(1);
    }
    EBOOLEAN();
  }
}
*/


/*
void COMPARAISON(){
  fprintf(test, "%s\n", "<COMPARAISON>");

      EXPRESSION();
      if(uniteCourante == INFERIEUR || uniteCourante == SUPERIEUR){
        uniteCourante = yylex();
        if(uniteCourante == EGAL)
          uniteCourante = yylex();
        if(uniteCourante == FIN){
          printf("%s\n", "erreur de syntaxe");
          exit(1);
        }
        COMPARAISON();
      }
      else if(uniteCourante == NON){
        uniteCourante = yylex();
        if(uniteCourante == EGAL){
          uniteCourante = yylex();
          if(uniteCourante == FIN){
            printf("%s\n", "erreur de syntaxe");
            exit(1);
          }
          COMPARAISON();
        }
        printf("%s\n", "erreur de syntaxe");
        exit(1);
      }
      else if(uniteCourante == EGAL){
        uniteCourante = yylex();
        if(uniteCourante == FIN){
          printf("%s\n", "erreur de syntaxe");
          exit(1);
        }
        COMPARAISON();
      }
      fprintf(test, "%s\n", "</COMPARAISON>");
}
*/

void Z() {
  fprintf(test, "%s\n", "<Z>");

  if(uniteCourante == CROCHET_OUVRANT){
    uniteCourante = yylex();
    OR();
    if(uniteCourante == CROCHET_FERMANT){
      uniteCourante = yylex();
      return;
    }
    else {
      printf("%s\n", "Error do la syntax");
      fprintf(test, "%s\n", "</Z>");

      exit(1);
    }
  }
  fprintf(test, "%s\n", "</Z>");
  return;
}

void fois(){
  fprintf(test, "%s\n", "<FOIS>");

  if(uniteCourante == PARENTHESE_OUVRANTE){
    uniteCourante = yylex();
    OR();
    if(uniteCourante == PARENTHESE_FERMANTE){
      fprintf(test, "%d\n", uniteCourante);

      uniteCourante = yylex();

      fprintf(test, "%s\n", "</FOIS>");

      return;
    }
    else{
      printf("%s\n", "Erreur de syntaxe");
      fprintf(test, "%s\n", "</FOIS>");
      exit(1);
    }
  }
  if(uniteCourante == FIN){
    fprintf(test, "%d\n", uniteCourante);
    fprintf(test, "%s\n", "</FOIS>");

    return;
  }
  if(uniteCourante == ID_VAR){
    uniteCourante = yylex();
    Z();
    fprintf(test, "%s\n", "</FOIS>");
    return;
  }
  if(uniteCourante == NOMBRE){
    fprintf(test, "%d\n", uniteCourante);

    uniteCourante = yylex();

    fprintf(test, "%s\n", "</FOIS>");

    return;
  }
  if(uniteCourante == NON){
    uniteCourante = yylex();
    OR();
    return;
  }


  printf("%s\n", "Erreur de syntaxe");
  fprintf(test, "%s\n", "</FOIS>");

  exit(1);

}

void TP() {
  fprintf(test, "%s\n", "<TP>");

  if(uniteCourante == FOIS || uniteCourante == DIVISE){
    uniteCourante = yylex();
    if (uniteCourante == FIN) {
      printf("%s\n", "erreur de syntaxe");
      exit(1);
    }
    EXPRESSION();
  }
  fprintf(test, "%s\n", "</TP>");


}

void addition(){
  fprintf(test, "%s\n", "<ADDITION>");

  if(uniteCourante == PLUS || uniteCourante == MOINS){
    fprintf(test, "%d\n", uniteCourante);

    uniteCourante = yylex();
    if (uniteCourante == FIN) {
      printf("%s\n", "erreur de syntaxe");
      exit(1);
    }
    EXPRESSION();
  }
  fprintf(test, "%s\n", "</ADDITION>");

}

 void COMPARAISONPRIME() {
   if(uniteCourante == EGAL || uniteCourante == INFERIEUR || uniteCourante == SUPERIEUR){
     uniteCourante = yylex();
     EXPRESSION();
   }
 }

 void ANDPRIME(){
   if(uniteCourante == ET){
     uniteCourante = yylex();
     COMPARAISON();
   }
 }


 void AND(){
   COMPARAISON();
   ANDPRIME();
 }

void COMPARAISON(){
  EXPRESSION();
  COMPARAISONPRIME();
}



void ORPRIME(){
  if(uniteCourante == OU){
    uniteCourante = yylex();
    AND();
  }

}

void OR(){
  AND();
  ORPRIME();
}


int main() {
  /* test */
  test = fopen("test.xml","w");

  yyin = fopen("LFile/testSyntax.l","r");
  uniteCourante = yylex();
  OR();
  if(uniteCourante == FIN)
    printf("%s\n", "Syntaxe correcte");
  else printf("%s\n", "erreur de syntaxe");

  return 0;
}
