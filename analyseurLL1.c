#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "analyseurLL1.h"
#include "util.h"
#include "affiche_arbre_abstrait.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affiche_arbre_abstrait.h"


int CC;
int trace_xml = 1;
FILE *test;

n_prog* programme(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_optDecVariables_,CC) || est_premier(_listeDecFonctions_,CC) || est_suivant(_programme_,CC)){
    n_l_dec* variables = optDecVariables();
    n_l_dec* fonctions = listeDecFonctions();
    n_prog* prog = cree_n_prog(variables,fonctions);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return prog;
  }
  else return NULL;
}

n_l_dec* optDecVariables(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_listeDecVariables_,CC)){
    n_l_dec* variables = listeDecVariables();
    if(CC == POINT_VIRGULE){
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return variables;
    }
    else{
      erreur("point virgoule");
    }
  }
  if(est_suivant(_optDecVariables_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("optDecVariables");
  return NULL;
}

n_l_dec* listeDecVariables(){
  n_l_dec *liste;
  n_dec *dec;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_declarationVariable_,CC)){
    dec = declarationVariable();
    liste = listeDecVariableBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    liste = cree_n_l_dec(dec,liste);
    return liste;
  }
  else{
    erreur("declaration variable");
  }
  return NULL;
}

n_l_dec* listeDecVariableBis(){
  n_l_dec* liste;
  n_dec *dec;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    dec = declarationVariable();
    liste = listeDecVariableBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    liste = cree_n_l_dec(dec,NULL);
    return liste;
  }
  if(est_suivant(_listeDecVariablesBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("listeDecVariableBis");
  return NULL;
}

n_dec* declarationVariable(){
  char *id_var = malloc(YYTEXT_MAX*sizeof(char));
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  n_dec *dec;
  if(CC == ENTIER){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == ID_VAR){
      strcpy(id_var,yytext);
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      dec = optTailleTableau(id_var);
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return dec;
    }
    else{
      erreur("ID_VAR");
    }
  }
  else{
    erreur("ENTIER");
  }
  return NULL;
}

n_dec* optTailleTableau(char* nom_var) {
  printf("---------------------------%s---------------------------\n", nom_var);
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
        return cree_n_dec_tab(nom_var,atoi(yytext));
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
    return cree_n_dec_var(nom_var);
  }
  erreur("optTailleTableau");
  return NULL;
}

n_l_dec* listeDecFonctions(){
  n_l_dec* liste;
  n_dec* dec;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_declarationFonction_,CC)){
    dec = declarationFonction();
    liste = listeDecFonctions();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    liste = cree_n_l_dec(dec,liste);
    return liste;
  }
  if(est_suivant(_listeDecFonctions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("listeDecFonctions");
  return NULL;
}

n_dec* declarationFonction(){
  char *nom = malloc(YYTEXT_MAX*sizeof(char));
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == ID_FCT){
    strcpy(nom,yytext);
    printf("----%s----\n", nom);
    CC = yylex();
    n_l_dec* listeparam = listeParam();
    n_l_dec* listeVar = optDecVariables();
    n_instr* corps = instructionBloc();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_dec_fonc(nom,listeparam,listeVar,corps);
  }
  else{
    erreur("ID_FCT");
  }
  return NULL;
}

n_l_dec* listeParam(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == PARENTHESE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    n_l_dec* liste = optListeDecVariables();
    if(CC == PARENTHESE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return liste;
    }
    else{
      erreur("PARENTHESE_FERMANTE");
    }
  }
  else{
    erreur("PARENTHESE_OUVRANTE");
  }
  return NULL;
}

n_l_dec* optListeDecVariables(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_listeDecVariables_,CC)){
    n_l_dec* liste = listeDecVariables();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return liste;
  }
  if(est_suivant(_optListeDecVariables_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("optListeDecVariables");
  return NULL;
}

n_instr* instruction() {
  n_instr* instru;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_instructionAffect_,CC)){
    instru = instructionAffect();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;
  }
  if(est_premier(_instructionBloc_,CC)){
    instru = instructionBloc();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  if(est_premier(_instructionSi_,CC)){
     instru = instructionSi();
     affiche_balise_fermante(__FUNCTION__,trace_xml);
     return instru;

  }
  if(est_premier(_instructionTantque_,CC)){
    instru = instructionTantQue();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  if(est_premier(_instructionAppel_,CC)){
    instru = instructionAppel();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  if(est_premier(_instructionRetour_,CC)){
    instru = instructionRetour();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  if(est_premier(_instructionEcriture_,CC)){
    instru = instructionEcriture();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  if(est_premier(_instructionVide_,CC)){
    instru = instructionVide();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return instru;

  }
  erreur("instruction");
  return NULL;
}

n_instr* instructionAffect(){
  n_var* variable;
  n_exp* expr;

  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(est_premier(_var_,CC)){
    variable = var();
    if(CC == EGAL){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      expr = expression();
      if(CC == POINT_VIRGULE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
        affiche_balise_fermante(__FUNCTION__,trace_xml);
        return cree_n_instr_affect(variable,expr);
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
  return NULL;
}

n_instr* instructionBloc(){
  n_l_instr* liste;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ACCOLADE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    liste = listeInstructions();
    if(CC == ACCOLADE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_instr_bloc(liste);
    }
    else{
      erreur("ACCOLADE_FERMANTE");
    }
  }
  else{
    erreur("ACCOLADE_OUVRANTE");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_l_instr* listeInstructions(){
  n_l_instr* liste;
  n_instr* instr;

  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_instruction_,CC)){
    instr = instruction();
    liste = listeInstructions();
    liste = cree_n_l_instr(instr,liste);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return liste;
  }
  if(est_suivant(_listeInstructions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("listeInstructions");
  return NULL;
}

n_instr* instructionSi(){
  n_instr* bloc;
  n_exp* expr;
  n_instr* sinon;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == SI){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expr = expression();
    if(CC == ALORS){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      bloc = instructionBloc();
      sinon = optSinon();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_instr_si(expr,bloc,sinon);
    }
    else{
      erreur("ALORS");
    }
  }
  else{
    erreur("SI");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_instr* optSinon(){
  n_instr *bloc;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  if(CC == SINON){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    bloc = instructionBloc();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return bloc;
  }
  if(est_suivant(_optSinon_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("optSinon");
  return NULL;
}

n_instr* instructionTantQue(){
  n_instr *bloc;
  n_exp *expr;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == TANTQUE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expr = expression();
    if(CC == FAIRE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      bloc = instructionBloc();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_instr_tantque(expr,bloc);
    }
    else{
      erreur("FAIRE");
    }
  }
  else{
    erreur("TANTQUE");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}


n_instr* instructionAppel(){
  n_appel* appel;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_appelFct_,CC)){
    appel = appelFct();
    if(CC == POINT_VIRGULE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_instr_appel(appel);
    }
    else{
      erreur("POINT_VIRGULE appelFct");
    }
  }
  else{
    erreur("appelFct");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_instr* instructionRetour(){
  n_exp* expr;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == RETOUR){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    expr = expression();
    if(CC == POINT_VIRGULE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_instr_retour(expr);
    }
    else{
      erreur("POINT_VIRGULE instructionRetour");
    }
  }
  else{
    erreur("instructionRetour");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_instr* instructionEcriture(){
  n_exp* expr;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ECRIRE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    if(CC == PARENTHESE_OUVRANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      expr = expression();
      if(CC == PARENTHESE_FERMANTE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
        affiche_balise_fermante(__FUNCTION__,trace_xml);
        return cree_n_instr_ecrire(expr);
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
  return NULL;
}

n_instr* instructionVide(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == POINT_VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_instr_vide();
  }
  else{
    erreur("instructionVide");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

/* continuer à partir de la */

n_exp* expression(){
  n_exp* conj;
  n_exp* exp;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_conjonction_,CC)){
    conj = conjonction();
    exp = expressionBis(conj);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return exp;
  }
  else{
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    erreur("expression");
  }
  return NULL;
}

n_exp* expressionBis(n_exp *herite){

  n_exp* conj;
  n_exp* exp;

  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == OU){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    conj = conjonction();
    exp = expressionBis(conj);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(ou,exp,herite);

  }
  if(est_suivant(_expressionBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return herite;
  }
  erreur("expressionBis");
  return NULL;
}

n_exp* conjonction(){
  n_exp* comp;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_comparaison_,CC)){
    comp = comparaison();
    n_exp* conjb = conjonctionBis(comp);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return conjb;

  }
  else{
    erreur("conjonction");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_exp* conjonctionBis(n_exp *herite){
  n_exp* comp;
  n_exp* conjbis;

  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ET){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    comp = comparaison();
    conjbis = conjonctionBis(comp);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(et,conjbis,herite);
  }
  if(est_suivant(_conjonctionBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return herite;
  }
  erreur("conjonctionBis");
  return NULL;
}

n_exp* comparaison(){
  n_exp* exparith;
  n_exp* exp;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_expArith_,CC)){
    exparith = expArith();
    exp = comparaisonBis(exparith);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return exp;
  }
  else{
    erreur("comparaison");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_exp* comparaisonBis(n_exp* herite){
  n_exp* exparith;
  n_exp* compbis;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == EGAL){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    exparith = expArith();
    compbis = comparaisonBis(exparith);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(egal,compbis,herite);
  }
  if(CC == INFERIEUR){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    exparith = expArith();
    compbis = comparaisonBis(exparith);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(inf,compbis,herite);
  }
  if(est_suivant(_comparaisonBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return herite;
  }
  erreur("comparaisonBis");
  return NULL;
}

n_exp* expArith(){
  n_exp* term;
  n_exp* exp;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_terme_,CC)){
    term = terme();
    exp = expArithBis(term);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return exp;
  }
  else{
    erreur("expArith");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_exp* expArithBis(n_exp *herite){
  n_exp* term;
  n_exp* exparithbis;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == PLUS){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    term = terme();
    exparithbis = expArithBis(term);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(plus,exparithbis,herite);
  }
  if(CC == MOINS){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    term = terme();
    exparithbis = expArithBis(term);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(moins,exparithbis,herite);
  }
  if(est_suivant(_expArithBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return herite;
  }
  erreur("expArithBis");
  return NULL;
}

n_exp* terme(){
  n_exp* neg;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_negation_,CC)){
    neg = negation();
    n_exp* termeb = termeBis(neg);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return termeb;
  }
  else{
    erreur("terme");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_exp* termeBis(n_exp *herite){
  n_exp* neg;
  n_exp* termebis;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == FOIS){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    neg = negation();
    termebis = termeBis(neg);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(fois,termebis,herite);
  }
  if(CC == DIVISE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    neg = negation();
    termebis = termeBis(neg);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(divise,termebis,herite);
  }
  if(est_suivant(_termeBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return herite;
  }
  erreur("termeBis");
  return NULL;
}

n_exp* negation(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);
  n_exp* neg;
  if(CC == NON){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    neg = negation();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_op(non,neg,NULL);
  }

  if(est_premier(_facteur_,CC)){
    n_exp* fact = facteur();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return fact;
  }
  erreur("negation");
  return NULL;
}

n_exp* facteur(){
  n_exp* exp;
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == PARENTHESE_OUVRANTE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    exp = expression();
    if(CC == PARENTHESE_FERMANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return exp;
    }
    else{
      erreur("POINT_VIRGULE facteur");
    }
  }
  if(CC == NOMBRE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_entier(atoi(yytext));
  }
  if(est_suivant(_appelFct_,CC)){
    n_appel* apl = appelFct();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_appel(apl);
  }
  if(est_premier(_var_,CC)){
    n_var* v = var();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_exp_var(v);
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
        return cree_n_exp_lire();
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
  return NULL;
}

n_var* var(){
  char* id_var = malloc(YYTEXT_MAX*sizeof(char));
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ID_VAR){
    affiche_token(CC,yytext,trace_xml);
    strcpy(id_var,yytext);
    CC = yylex();
    n_var* indice = optIndice(id_var);
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return indice;
  }
  else{
    erreur("var");
  }

  affiche_balise_fermante(__FUNCTION__,trace_xml);
  return NULL;
}

n_var* optIndice(char* id_var){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == CROCHET_OUVRANT){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    n_exp* exp = expression();
    if(CC == CROCHET_FERMANT){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      affiche_balise_fermante(__FUNCTION__,trace_xml);
      return cree_n_var_indicee(id_var,exp);
    }
    else{
      erreur("CROCHET_FERMANT optIndice");
    }
  }
  if(est_suivant(_optIndice_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_var_simple(id_var);
  }
  erreur("optIndice");
  return NULL;
}

n_appel* appelFct(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == ID_FCT){
    affiche_token(CC,yytext,trace_xml);
    char *id_fct = malloc(YYTEXT_MAX*sizeof(char));;
    strcpy(id_fct,yytext);
    CC = yylex();

    printf("-------------------%s--------------------------\n", id_fct);
    if(CC == PARENTHESE_OUVRANTE){
      affiche_token(CC,yytext,trace_xml);
      CC = yylex();
      n_l_exp* listexp = listeExpressions();
      if(CC == PARENTHESE_FERMANTE){
        affiche_token(CC,yytext,trace_xml);
        CC = yylex();
        affiche_balise_fermante(__FUNCTION__,trace_xml);
        return cree_n_appel(id_fct,listexp);
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
  return NULL;
}

n_l_exp* listeExpressions(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(est_premier(_expression_,CC)){
    n_exp* exp = expression();
    n_l_exp* l_exp = listeExpressionsBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_l_exp(exp,l_exp);
  }
  if(est_suivant(_listeExpressions_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("listeExpressions");
  return NULL;
}

n_l_exp* listeExpressionsBis(){
  affiche_balise_ouvrante(__FUNCTION__,trace_xml);

  if(CC == VIRGULE){
    affiche_token(CC,yytext,trace_xml);
    CC = yylex();
    n_exp* exp = expression();
    n_l_exp* l_exp = listeExpressionsBis();
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return cree_n_l_exp(exp,l_exp);
  }
  if(est_suivant(_listeExpressionsBis_,CC)){
    affiche_balise_fermante(__FUNCTION__,trace_xml);
    return NULL;
  }
  erreur("listeExpressionsBis");
  return NULL;
}

int main() {

  test = fopen("test.xml","w");
  yyin = fopen("LFile/boucle.l","r");
  CC = yylex();
  initialise_premiers();
  initialise_suivants();
  n_prog* prog = programme();
  printf("%s\n", "---------------------------------------------------------------------------------------------------------------");
  affiche_n_prog(prog);
  return 0;

}
