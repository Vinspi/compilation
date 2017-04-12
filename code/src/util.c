#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "analyseur_lexical.h"

int col = 0;
int ligne = 0;

/* ------------------------------------------------------------------------*/

int mangeEspaces()
{
  char c = fgetc(yyin);
  int comment = 0;
  while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
    if( c == '#' ) {
        comment = 1;
    }
    if( c == '\n' ) {
      ligne++;
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


/*-------------------------------------------------------------------------*/

int nb_ligne;
int indent_xml = 0;
int indent_step = 1; // set to 0 for no indentation

/*-------------------------------------------------------------------------*/

void erreur(char *message) {
  fprintf (stderr, "Ligne %d : \n", ligne);
  fprintf (stderr, "%s\n", message);
  exit(1);
}

/*-------------------------------------------------------------------------*/

char *duplique_chaine(char *src) {
  char *dest = malloc(sizeof(char) * strlen(src));
  strcpy(dest, src);
  return dest;
}

/*-------------------------------------------------------------------------*/

void indent() {
    int i;
    for( i = 0; i < indent_xml; i++ ) {
      printf( "  " );
    }
}
/*-------------------------------------------------------------------------*/
void affiche_balise_ouvrante(const char *fct_, int trace_xml) {
  if( trace_xml ) {
    indent();
    indent_xml += indent_step ;
	  fprintf (stdout, "<%s>\n", fct_);
	}
}

/*-------------------------------------------------------------------------*/

void affiche_balise_fermante(const char *fct_, int trace_xml) {
  if(trace_xml) {
    indent_xml -= indent_step ;
    indent();
    fprintf (stdout, "</%s>\n", fct_);
  }
}

/*-------------------------------------------------------------------------*/


void affiche_texte(char *texte_, int trace_xml) {
  if(trace_xml) {
    indent();
    fprintf (stdout, "%s\n", texte_);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_xml_texte( char *texte_ ) {
  int i = 0;
  while( texte_[ i ] != '\0' ) {
    if( texte_[ i ] == '<' ) {
      fprintf( stdout, "&lt;" );
    }
    else if( texte_[ i ] == '>' ) {
      fprintf( stdout, "&gt;" );
    }
    else if( texte_[ i ] == '&' ) {
      fprintf( stdout, "&amp;" );
    }
    else {
      putchar( texte_[i] );
    }
    i++;
  }
}

/*-------------------------------------------------------------------------*/

void affiche_token(int fct, char *texte_, int trace_xml) {
  if(trace_xml) {
    indent();
    fprintf (stdout, "<%s>", affiche_tokenBis(fct));
    affiche_xml_texte( texte_ );
    fprintf (stdout, "</%s>\n", affiche_tokenBis(fct));
  }
}

void affiche_element(char *fct,char* texte,int trace_xml){
  if(trace_xml){
    indent();
    fprintf(stdout, "<%s>",fct);
    affiche_xml_texte(texte);
    fprintf(stdout, "</%s>\n",fct);
  }
}
