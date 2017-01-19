#include "unitesLexicales.h"
#include "analyseur_lexical.h"
#include <stdio.h>
#include <string.h>


int mangeEspaces()
{
  char c = fgetc(yyin);
  int comment = 0;
  while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
    if( c == '#' ) {
        comment = 1;
    }
    if( c == '\n' ) {

      comment = 0;
    }
    c = fgetc(yyin);
  }
  if ( feof(yyin) ) {
    return -1;
  }
  ungetc(c, yyin);
  return 0;
}


char lireCar(void)
{
  yytext[yyleng++] = fgetc(yyin);
  yytext[yyleng] = '\0';
  return yytext[yyleng - 1];
}

void delireCar()
{
  char c;
  c = yytext[yyleng - 1];
  yytext[--yyleng] = '\0';
  ungetc(c, yyin);
}

int isTokenUn(char token){
	switch(token){
		case '+':
			return PLUS;
		case '-':
			return MOINS;
		case '*':
			return FOIS;
		case '/':
			return DIVISE;
		case '=':
			return EGAL;
		case ';':
			return POINT_VIRGULE;
		case '(':
			return PARENTHESE_OUVRANTE;
		case ')':
			return PARENTHESE_FERMANTE;
		case '[':
			return CROCHET_OUVRANT;
		case ']':
			return CROCHET_FERMANT;
		case '{':
			return ACCOLADE_OUVRANTE;
		case '}':
			return ACCOLADE_FERMANTE;
		case '&':
			return ET;
		case '|':
			return OU;
		case '!':
			return NON;
		case '<':
			return INFERIEUR;
		case '>':
			return SUPERIEUR;
		case ',':
			return VIRGULE;
		case EOF:
			return FIN;


		default:
			return -1;

	}
}



int isTokenFull(char *text){

	if(!strcmp(text,"si")) return SI;
	if(!strcmp(text,"alors")) return ALORS;
	if(!strcmp(text,"sinon")) return SINON;
	if(!strcmp(text,"entier")) return ENTIER;
	if(!strcmp(text,"retour")) return RETOUR;
	if(!strcmp(text,"tantque")) return TANTQUE;
	if(!strcmp(text,"faire")) return FAIRE;
	if(!strcmp(text,"lire")) return LIRE;
	if(!strcmp(text,"ecrire")) return ECRIRE;
	return -1;

}
/* retourne 1 pour vrai 0 pour faux */
int isVariable(){

  /* on verifie la 1ere lettre */
	if(!(yytext[0] == '$'))
		return 0;

  /* PUIS LES AUTRES */
  for(int i=1;i<yyleng;i++)
    if(!(yytext[i] == '_' || (yytext[i] < 91 && yytext[i] > 64) || (yytext[i] < 123 && yytext[i] > 96 || yytext[i] == '$' || (yytext[i] < 58 && yytext[i] > 47))))
      return 0;
	//if(strstr(yytext+1,"$") != NULL) return 0;

	//if(isTokenFull(yytext+1) != -1) return 0;

	return 1;

}

int isFonction(){

  /* on limite les caracteres du nom des fonction avec l'ascii */

  /* on teste le caractère de début */

  if(!(yytext[0] == '_' || (yytext[0] < 91 && yytext[0] > 64) || (yytext[0] < 123 && yytext[0] > 96)))
    return 0;

  /* puis toutes les autres lettres */
  for(int i=1;i<yyleng;i++)
    if(!(yytext[i] == '_' || (yytext[i] < 91 && yytext[i] > 64) || (yytext[i] < 123 && yytext[i] > 96 || yytext[i] == '$' || (yytext[i] < 58 && yytext[i] > 47))))
      return 0;

  /* avec le contexte impossible que yytext contienne un mot clé */

  return 1;
}

int isNombre(){
  /* si toute les lettres sont des chiffres alors c'est un nombre */
  for(int i=1;i<yyleng;i++)
    if(!(yytext[i] < 58 && yytext[i] > 47))
      return 0;

  return 1;
}

int yylex(void)
{
  char c;
  int r;
  mangeEspaces();
  yytext[yyleng = 0] = '\0';

  do{
  	c = lireCar();
  	r = isTokenUn(c);

  	if(yyleng == 1 && r > 0){
  		return r;
  	}
  	else{
  		if(r > 0){
  			break;
  		}
  	}

  }
  while(c != ' ' && c != EOF && c != '#' && c != '\t' && c != '\n');
  delireCar();

  r = isTokenFull(yytext);
  if(r > 0) return r;

  // on recherche number, variable ou fonction

  if(isVariable()){
    return ID_VAR;
  }
  if(isFonction()){
    return ID_FCT;
  }
  if(isNombre()){
    return NOMBRE;
  }
  return INCONNU;

}

void affiche_token(int token) {
  switch (token) {
    case INCONNU: printf("%s ", "INCONNU"); break;
    case POINT_VIRGULE: printf("%s ", "POINT_VIRGULE"); break;
    case PLUS: printf("%s", "PLUS"); break;
    case MOINS: printf("%s ", "MOINS"); break;
    case FOIS: printf("%s ", "FOIS"); break;
    case DIVISE: printf("%s ", "DIVISE"); break;
    case PARENTHESE_OUVRANTE: printf("%s ", "PARENTHESE_OUVRANTE"); break;
    case PARENTHESE_FERMANTE: printf("%s ", "PARENTHESE_FERMANTE"); break;
    case CROCHET_OUVRANT: printf("%s ", "CROCHET_OUVRANT"); break;
    case CROCHET_FERMANT: printf("%s ", "CROCHET_FERMANT"); break;
    case ACCOLADE_OUVRANTE: printf("%s ", "ACCOLADE_OUVRANTE"); break;
    case ACCOLADE_FERMANTE: printf("%s ", "ACCOLADE_FERMANTE"); break;
    case EGAL: printf("%s ", "EGAL"); break;
    case INFERIEUR: printf("%s ", "INFERIEUR"); break;
    case ET: printf("%s ", "ET"); break;
    case OU: printf("%s ", "OU"); break;
    case NON: printf("%s ", "NON"); break;
    case SUPERIEUR: printf("%s ", "SUPERIEUR"); break;
    case SI: printf("%s ", "SI"); break;
    case ALORS: printf("%s ", "ALORS"); break;
    case SINON: printf("%s ", "SINON"); break;
    case TANTQUE: printf("%s ", "TANTQUE"); break;
    case FAIRE: printf("%s ", "FAIRE"); break;
    case ENTIER: printf("%s ", "ENTIER"); break;
    case RETOUR: printf("%s ", "RETOUR"); break;
    case LIRE: printf("%s ", "LIRE"); break;
    case ECRIRE: printf("%s ", "ECRIRE"); break;
    case ID_VAR: printf("%s ", "ID_VAR"); break;
    case ID_FCT: printf("%s ", "ID_FCT"); break;
    case NOMBRE: printf("%s ", "NOMBRE"); break;
    case FIN: printf("%s ", "FIN"); break;
    case VIRGULE: printf("%s ", "VIRGULE"); break;


  }
}

void test_yylex() {
  int uniteCourante;

  do {
    uniteCourante = yylex();
    affiche_token(uniteCourante);
  } while (uniteCourante != FIN);
}

/*
int main(int argc, char const *argv[]) {

  yyin = fopen("test.l","r");
  test_yylex();
  printf("\n");

  return 0;
}
*/
