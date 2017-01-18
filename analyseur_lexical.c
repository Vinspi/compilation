

char yytext[YYTEXT_MAX];
int yyleng;

int mangeEspaces()
{ 
  char c = fgetc(yyin);
  int comment = 0;
  while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
    if( c == '#' ) {
        comment = 1;
    }
    if( c == '\n' ) {
      nb_ligne++;
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
	
	if(!strcmp(text,"si") return SI;
	if(!strcmp(text,"alors") return ALORS;
	if(!strcmp(text,"sinon") return SINON;
	if(!strcmp(text,"entier") return ENTIER;
	if(!strcmp(text,"retour") return RETOUR;
	if(!strcmp(text,"tantque") return TANTQUE;
	if(!strcmp(text,"faire") return FAIRE;
	if(!strcmp(text,"lire") return LIRE;
	if(!strcmp(text,"ecrire") return ECRIRE;
	return -1;
	
}

int isVariable(){
	int i = 1;
	if(!yytext[0] == '$')
		return -1;
	
	
	if(strstr(yytext+1,"$") != NULL) return -1;
	
	if(isTokenFull(yytext+1) != -1) return -1;

	return 1;

}

int yylex(void)
{
  char c;
  int i,r;
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

  if(yytext[0] == '$'){
	
  }

}

























