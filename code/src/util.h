#ifndef __UTIL__
#define __UTIL__

char *duplique_chaine(char *s);
void erreur(char *message);
void affiche_balise_ouvrante(const char *fct_, int trace_xml);
void affiche_balise_fermante(const char *fct_, int trace_xml);
void affiche_token(int fct_, char *texte_, int trace_xml);
void affiche_texte(char *texte_, int trace_xml);
void affiche_element(char *fct,char* texte,int trace_xml);

#endif
