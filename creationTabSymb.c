#include <stdio.h>
#include "syntabs.h"
#include "tabsymboles.h"

void ajoute_l_dec(n_l_dec* listeDec);
void ajoute_l_dec(n_l_dec* n);
void ajoute_dec(n_dec* n);
void ajoute_dec_fonc(n_dec* n);
void ajoute_dec_tab(n_dec* n);
void ajoute_dec_var(n_dec *n);
void compteParametres(n_l_dec* n);

// int ajouteIdentificateur(char *identif,  int portee, int type, int adresse, int complement);

int portee;
int adresseLocaleCourante;
int adresseArgumentCourant;
int adresseGlobaleCourante;
int nb_param;

/*-------------------------------------------------------------------------*/

void cree_n_tab_dec(n_prog *n)
{

	adresseLocaleCourante = 0;
	adresseArgumentCourant = 0;
	adresseGlobaleCourante = 0;
	portee = P_VARIABLE_GLOBALE;


  ajoute_l_dec(n->variables);
  ajoute_l_dec(n->fonctions);
}

void ajoute_l_dec(n_l_dec* n){
	if(n){
		ajoute_dec(n->tete);
		ajoute_l_dec(n->queue);
	}
}

/*-------------------------------------------------------------------------------------------*/

void ajoute_dec(n_dec *n)
{
  if(n){
    if(n->type == foncDec) {
      ajoute_dec_fonc(n);
    }
    else if(n->type == varDec) {
      ajoute_dec_var(n);
    }
    else if(n->type == tabDec) {
      ajoute_dec_tab(n);
    }
  }
}

/*-------------------------------------------------------------------------------------------*/

void ajoute_dec_tab(n_dec *n){
	if(portee == P_VARIABLE_GLOBALE){
		ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseGlobaleCourante,n->u.tabDec_.taille);
		adresseGlobaleCourante+= (n->u.tabDec_.taille*4);
		return;
	}
	if(portee == P_VARIABLE_LOCALE){
		ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseLocaleCourante,n->u.tabDec_.taille);
		adresseLocaleCourante+= (n->u.tabDec_.taille*4);
		return;
	}
	else{
		ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseArgumentCourant,n->u.tabDec_.taille);
		adresseArgumentCourant+= (n->u.tabDec_.taille*4);
		return;
	}
}

/*-------------------------------------------------------------------------------------------*/

void ajoute_dec_var(n_dec* n){
	if(portee == P_VARIABLE_GLOBALE){
		ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseGlobaleCourante,1);
		adresseGlobaleCourante+=4;
		return;
	}
	if(portee == P_VARIABLE_LOCALE){
		ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseLocaleCourante,1);
		adresseLocaleCourante+=4;
		return;
	}
	else{
		ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseArgumentCourant,1);
		adresseArgumentCourant+=4;
		nb_param++;
		return;
	}
}

/*-------------------------------------------------------------------------------------------*/

void ajoute_dec_fonc(n_dec* n){
	nb_param = 0;
	compteParametres(n->u.foncDec_.param);
	ajouteIdentificateur(n->nom,P_VARIABLE_GLOBALE,T_FONCTION,0,nb_param); // ici on connait le nombre d'args de la fonction
	entreeFonction();
	portee = P_ARGUMENT;
	ajoute_l_dec(n->u.foncDec_.param);
	portee = P_VARIABLE_LOCALE;
	//entreeFonction();
	ajoute_l_dec(n->u.foncDec_.variables);
	// on ne fait pas encore le check du corps de la fonction
	afficheTabsymboles();
	sortieFonction();
}

/*-------------------------------------------------------------------------------------------*/

void compteParametres(n_l_dec* n){
	if(n){
		nb_param++;
		compteParametres(n->queue);
	}
}
