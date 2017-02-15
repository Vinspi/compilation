#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "analyseurLL1.h"
#include "util.h"

int CC;
int trace_xml = 1;
FILE *test;

void programme(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_optDecVariables_,CC) || est_premier(_listeDecFonctions_,CC) || est_suivant(_programme_,CC)){
    optDecVariables();
    listeDecFonctions();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
}

void optDecVariables(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_listeDecVariables_,CC)){
    listeDecVariables();
    if(CC == POINT_VIRGULE){
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return;
    }
    else{
      erreur("point virgoule");
    }
  }
  if(est_suivant(_optDecVariables_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("optDecVariables");
}

void listeDecVariables(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_declarationVariable_,CC)){
    declarationVariable();
    listeDecVariableBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  else{
    erreur("declaration variable");
  }
}

void listeDecVariableBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    declarationVariable();
    listeDecVariableBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_listeDecVariablesBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("listeDecVariableBis");
}

void declarationVariable(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == ENTIER){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == ID_VAR){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      optTailleTableau();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return;
    }
    else{
      erreur("ID_VAR");
    }
  }
  else{
    erreur("ENTIER");
  }
}

void optTailleTableau() {
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == CROCHET_OUVRANT){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == NOMBRE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      if(CC == CROCHET_FERMANT){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
        affiche_balise_fermante(__FUNCTION__,trace_xml);
        return;
      }
      else{
        erreur("NOMBRE");
      }
    }
    else{
      erreur("CROCHET_FERMANT");
    }
  }
  if(est_suivant(_optTailleTableau_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("optTailleTableau");
}

void listeDecFonctions(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_declarationFonction_,CC)){
    declarationFonction();
    listeDecFonctions();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_listeDecFonctions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("listeDecFonctions");
}

void declarationFonction(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == ID_FCT){
    CC = yylex();
    listeParam();
    optDecVariables();
    instructionBloc();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  else{
    erreur("ID_FCT");
  }
}

void listeParam(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == PARENTHESE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    optListeDecVariables();
    if(CC == PARENTHESE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return;
    }
    else{
      erreur("PARENTHESE_FERMANTE");
    }
  }
  else{
    erreur("PARENTHESE_OUVRANTE");
  }
}

void optListeDecVariables(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_listeDecVariables_,CC)){
    listeDecVariables();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_optListeDecVariables_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("optListeDecVariables");
}

void instruction() {
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_instructionAffect_,CC)){
    instructionAffect();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_premier(_instructionBloc_,CC)){
    instructionBloc();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  if(est_premier(_instructionSi_,CC)){
     instructionSi();
     affiche_balise_fermante(__FUNCTION__,trace_xml);
     return;

  }
  if(est_premier(_instructionTantque_,CC)){
    instructionTantQue();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  if(est_premier(_instructionAppel_,CC)){
    instructionAppel();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  if(est_premier(_instructionRetour_,CC)){
    instructionRetour();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  if(est_premier(_instructionEcriture_,CC)){
    instructionEcriture();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  if(est_premier(_instructionVide_,CC)){
    instructionVide();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;

  }
  erreur("instruction");
}

void instructionAffect(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_var_,CC)){
    var();
    if(CC == EGAL){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      expression();
      if(CC == POINT_VIRGULE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
      }
      else{
        erreur("POINT_VIRGULE");
      }
    }
    else{
      erreur("EGAL AFFECT");
    }
  }
  else{
    erreur("ID_VAR");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void instructionBloc(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ACCOLADE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    listeInstructions();
    if(CC == ACCOLADE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
    }
    else{
      erreur("ACCOLADE_FERMANTE");
    }
  }
  else{
    erreur("ACCOLADE_OUVRANTE");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void listeInstructions(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_instruction_,CC)){
    instruction();
    listeInstructions();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_listeInstructions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("listeInstructions");
}

void instructionSi(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == SI){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    if(CC == ALORS){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      instructionBloc();
      optSinon();
    }
    else{
      erreur("ALORS");
    }
  }
  else{
    erreur("SI");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void optSinon(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == SINON){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    instructionBloc();
  }
  if(est_suivant(_optSinon_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("optSinon");
}

void instructionTantQue(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == TANTQUE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    if(CC == FAIRE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      instructionBloc();
    }
    else{
      erreur("FAIRE");
    }
  }
  else{
    erreur("TANTQUE");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}


void instructionAppel(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_appelFct_,CC)){
    appelFct();
    if(CC == POINT_VIRGULE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
    }
    else{
      erreur("POINT_VIRGULE appelFct");
    }
  }
  else{
    erreur("appelFct");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void instructionRetour(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == RETOUR){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    if(CC == POINT_VIRGULE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
    }
    else{
      erreur("POINT_VIRGULE instructionRetour");
    }
  }
  else{
    erreur("instructionRetour");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void instructionEcriture(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ECRIRE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == PARENTHESE_OUVRANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      expression();
      if(CC == PARENTHESE_FERMANTE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
      }
      else{
        erreur("PARENTHESE_FERMANTE");
      }
    }
    else{
      erreur("PARENTHESE_OUVRANTE");
    }
  }
  else{
    erreur("ECRIRE");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void instructionVide(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == POINT_VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
  }
  else{
    erreur("instructionVide");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void expression(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_conjonction_,CC)){
    conjonction();
    expressionBis();
  }
  else{
    erreur("expression");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void expressionBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == OU){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    conjonction();
    expressionBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_expressionBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("expressionBis");
}

void conjonction(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_comparaison_,CC)){
    comparaison();
    conjonctionBis();
  }
  else{
    erreur("conjonction");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void conjonctionBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ET){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    comparaison();
    conjonctionBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_conjonctionBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("conjonctionBis");
}

void comparaison(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_expArith_,CC)){
    expArith();
    comparaisonBis();
  }
  else{
    erreur("comparaison");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void comparaisonBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == EGAL || CC == INFERIEUR){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expArith();
    comparaisonBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_comparaisonBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("comparaisonBis");
}

void expArith(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_terme_,CC)){
    terme();
    expArithBis();
  }
  else{
    erreur("expArith");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void expArithBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == PLUS || CC == MOINS){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    terme();
    expArithBis();
  }
  if(est_suivant(_expArithBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("expArithBis");
}

void terme(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_negation_,CC)){
    negation();
    termeBis();
  }
  else{
    erreur("terme");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void termeBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == FOIS || CC == DIVISE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    negation();
    termeBis();
  }
  if(est_suivant(_termeBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("termeBis");
}

void negation(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == NON){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    negation();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }

  if(est_premier(_facteur_,CC)){
    facteur();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("negation");

}

void facteur(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == PARENTHESE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    if(CC == PARENTHESE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return;
    }
    else{
      erreur("POINT_VIRGULE facteur");
    }
  }
  if(CC == NOMBRE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_suivant(_appelFct_,CC)){
    appelFct();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(est_premier(_var_,CC)){
    var();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  if(CC == LIRE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == PARENTHESE_OUVRANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      if(CC == PARENTHESE_FERMANTE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
        affiche_balise_fermante(__FUNCTION__,trace_xml);
        return;
      }
      else{
        erreur("PARENTHESE_FERMANTE LIRE");
      }
    }
    else{
      erreur("PARENTHESE_OUVRANTE LIRE");
    }
  }
  erreur("facteur");
  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void var(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ID_VAR){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    optIndice();
  }
  else{
    erreur("var");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void optIndice(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == CROCHET_OUVRANT){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    if(CC == CROCHET_FERMANT){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
    }
    else{
      erreur("CROCHET_FERMANT optIndice");
    }
  }
  if(est_suivant(_optIndice_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("optIndice");
}
void appelFct(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ID_FCT){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == PARENTHESE_OUVRANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      listeExpressions();
      if(CC == PARENTHESE_FERMANTE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
      }
      else{
        erreur("PARENTHESE_FERMANTE appelFct");
      }
    }
    else{
      erreur("PARENTHESE_OUVRANTE appelFct");
    }
  }
  else{
    erreur("appelFct");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
}

void listeExpressions(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_expression_,CC)){
    expression();
    listeExpressionsBis();
  }
  if(est_suivant(_listeExpressions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("listeExpressions");
}

void listeExpressionsBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expression();
    listeExpressionsBis();
  }
  if(est_suivant(_listeExpressionsBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return;
  }
  erreur("listeExpressionsBis");
}

int main() {

  test = fopen("test.xml","w");
  yyin = fopen("LFile/tri.l","r");
  CC = yylex();
  initialise_premiers();
  initialise_suivants();
  programme();
  return 0;

}
