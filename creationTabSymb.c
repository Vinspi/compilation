#include <stdio.h>
#include "syntabs.h"
#include "tabsymboles.h"
#include <string.h>
#include "util.h"

void ajoute_l_dec(n_l_dec* listeDec);
void ajoute_l_dec(n_l_dec* n);
void ajoute_dec(n_dec* n);
void ajoute_dec_fonc(n_dec* n);
void ajoute_dec_tab(n_dec* n);
void ajoute_dec_var(n_dec *n);
void compteParametres(n_l_dec* n);
void analyse_n_instr(n_instr *corps);
int analyse_n_exp(n_exp *exp);
void analyse_n_l_exp(n_l_exp *l_exp);
void analyser_n_appel(n_appel *appel);
void analyse_n_l_instr(n_l_instr *l_instr);
void verification_main();

// int ajouteIdentificateur(char *identif,  int portee, int type, int adresse, int complement);

int portee;
int adresseLocaleCourante;
int adresseArgumentCourant;
int adresseGlobaleCourante;
int nb_param;
int trace = 1;
int buffer[80];
int nu_registre = 0;


tabsymboles_ tabsymboles;

/*-------------------------------------------------------------------------*/

void cree_n_tab_dec(n_prog *n)
{

	adresseLocaleCourante = 0;
	adresseArgumentCourant = 0;
	adresseGlobaleCourante = 0;
	portee = P_VARIABLE_GLOBALE;

	affiche_texte(".data",trace);
  ajoute_l_dec(n->variables);
	affiche_texte(".text",trace);
	affiche_texte("__start:",trace);
	affiche_texte("\tjal main",trace);
	affiche_texte("\tli $vo 10",trace);
	affiche_texte("\tsyscall\t#exit",trace);
  ajoute_l_dec(n->fonctions);
	verification_main();
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
	if(rechercheDeclarative(n->nom) >= 0){
		printf("ERROR: redeclaration de la variable %s\n",n->nom);
		exit(1);
	}
	if(portee == P_VARIABLE_GLOBALE){
		ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseGlobaleCourante,n->u.tabDec_.taille);
		adresseGlobaleCourante+= (n->u.tabDec_.taille*4);

		sprintf(buffer,"\t%s: .space %d", n->nom,n->u.tabDec_.taille*4);
		affiche_texte(buffer,trace);


		return;
	}
	else{
		printf("ERROR : tableau locaux interdit : %s\n", n->nom);
		exit(1);
	}
}

/*-------------------------------------------------------------------------------------------*/

void ajoute_dec_var(n_dec* n){
	if(rechercheDeclarative(n->nom) >= 0){
		printf("ERROR: redeclaration de la variable %s\n",n->nom);
		exit(1);
	}
	if(portee == P_VARIABLE_GLOBALE){

		ajouteIdentificateur(n->nom,portee,T_ENTIER,adresseGlobaleCourante,1);
		adresseGlobaleCourante+=4;
		sprintf(buffer,"\t%s: .word 0", n->nom);
		affiche_texte(buffer,trace);

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
	if(rechercheDeclarative(n->nom) >= 0){
		printf("ERROR: redeclaration de la fonction %s\n",n->nom);
		exit(1);
	}
	sprintf(buffer,"%s:",n->nom);
	affiche_texte(buffer,trace);



	compteParametres(n->u.foncDec_.param);
	ajouteIdentificateur(n->nom,P_VARIABLE_GLOBALE,T_FONCTION,0,nb_param); // ici on connait le nombre d'args de la fonction
	entreeFonction();
	portee = P_ARGUMENT;
	ajoute_l_dec(n->u.foncDec_.param);
	portee = P_VARIABLE_LOCALE;
	//entreeFonction();
	ajoute_l_dec(n->u.foncDec_.variables);
	// on ne fait pas encore le check du corps de la fonction

	analyse_n_instr(n->u.foncDec_.corps);

	//afficheTabsymboles();
	sortieFonction();
}

/*-------------------------------------------------------------------------------------------*/

void compteParametres(n_l_dec* n){
	if(n){
		nb_param++;
		compteParametres(n->queue);
	}
}

/*-------------------------------------------------------------------------------------------*/

void analyse_n_instr(n_instr *corps){
	if(corps == NULL) return;
	int current_register;
	switch (corps->type) {
		case incrInst:
			analyse_n_exp(corps->u.incr);
			break;
		case affecteInst:
			if(rechercheExecutable(corps->u.affecte_.var->nom) < 0){
				printf("ERROR : variable %s non déclarée\n", corps->u.affecte_.var->nom);
				exit(1);
			}
			if(corps->u.affecte_.var->type == simple && tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].type != T_ENTIER){
				printf("ERROR : variable %s mauvais type\n", corps->u.affecte_.var->nom);
				exit(1);
			}
			if(corps->u.affecte_.var->type == indicee && tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].type != T_TABLEAU_ENTIER){
				printf("ERROR : variable %s mauvais type\n", corps->u.affecte_.var->nom);
				exit(1);
			}
			current_register = analyse_n_exp(corps->u.affecte_.exp);
			sprintf(buffer,"\tsw $t%d, %s \t#sauve la variable",current_register,corps->u.affecte_.var->nom);
			affiche_texte(buffer,trace);
			break;
		case siInst:
			analyse_n_exp(corps->u.si_.test);
			analyse_n_instr(corps->u.si_.alors);
			analyse_n_instr(corps->u.si_.sinon);
			break;
		case faireInst:
			analyse_n_exp(corps->u.faire_.test);
			analyse_n_instr(corps->u.faire_.faire);
			break;
		case tantqueInst:
			analyse_n_exp(corps->u.tantque_.test);
			analyse_n_instr(corps->u.tantque_.faire);
			break;
		case appelInst:
			analyser_n_appel(corps->u.appel);
			break;
		case retourInst:
			analyse_n_exp(corps->u.retour_.expression);
			break;
		case ecrireInst:
			current_register = analyse_n_exp(corps->u.retour_.expression);
			sprintf(buffer,"\tli $vo 1");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tmove $ao $t%d",current_register);
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tsyscall");
			affiche_texte(buffer,trace);

			break;
		case videInst:

			break;
		case blocInst:
			analyse_n_l_instr(corps->u.liste);
			break;
		}

}

/*-------------------------------------------------------------------------------------------*/

int analyse_n_exp(n_exp *exp){
	int reg_1, reg_2;
	switch (exp->type) {
		case varExp:
			if(rechercheExecutable(exp->u.var->nom) < 0){
				printf("ERROR : variable %s non déclarée\n", exp->u.var->nom);
				exit(1);
			}
			if(exp->u.var->type == simple && tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].type != T_ENTIER){
				printf("type de variable %d, type déclaré : %d\n", exp->u.var->type,tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].type);
				printf("ERROR : variable %s mauvais type\n", exp->u.var->nom);
				exit(1);
			}
			if(exp->u.var->type == indicee && tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].type != T_TABLEAU_ENTIER){
				printf("typooe de variable %d, type déclaré : %d\n", exp->u.var->type,tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].type);
				printf("ERROR : variable %s mauvais type\n", exp->u.var->nom);
				exit(1);
			}
			if(tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].portee == P_VARIABLE_GLOBALE) printf("\tlw $t%d, %s\n",nu_registre,exp->u.var->nom);
			nu_registre++;
			return nu_registre-1;

		case opExp:
			reg_1 = analyse_n_exp(exp->u.opExp_.op1);
			reg_2 = analyse_n_exp(exp->u.opExp_.op2);
			switch (exp->u.opExp_.op) {
				case plus:
					sprintf(buffer,"\tadd $t%d, $t%d, $t%d",nu_registre,reg_1,reg_2);
					affiche_texte(buffer,trace);
					break;
				case moins:
					sprintf(buffer,"\tsub $t%d, $t%d, $t%d",nu_registre,reg_1,reg_2);
					affiche_texte(buffer,trace);
					break;
				case fois:
					sprintf(buffer,"\tmul $t%d, $t%d",reg_1,reg_2);
					affiche_texte(buffer,trace);
					sprintf(buffer,"\tmflo $t%d",nu_registre);
					affiche_texte(buffer,trace);
					break;
			}
			nu_registre++;
			return nu_registre-1;
		case intExp:
			sprintf(buffer,"\tli $t%d, %d",nu_registre,exp->u.entier);
			affiche_texte(buffer,trace);
			nu_registre++;
			return nu_registre-1;

		case appelExp:
			analyser_n_appel(exp->u.appel);
			break;

		case lireExp:
			sprintf(buffer,"\tli $vo, 5");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tsyscall");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tmove $t%d, $vo",nu_registre);
			affiche_texte(buffer,trace);
			nu_registre++;
			return nu_registre-1;

	}
}

/*-------------------------------------------------------------------------------------------*/

void analyser_n_appel(n_appel *appel){

	/* verifier le nombre de parametres de la fonction */

	int i = rechercheExecutable(appel->fonction);

	if(i < 0){
		printf("ERROR : fonction %s inconnue\n", appel->fonction);
		exit(1);
	}

	nb_param = 0;
	compter_n_param(appel->args);

	if(tabsymboles.tab[i].complement != nb_param){
		printf("ERROR : nombre de parametres incorrect pour la fonction %s\n", appel->fonction);
		exit(1);
	}

	analyse_n_l_exp(appel->args);
}

/*-------------------------------------------------------------------------------------------*/

void analyse_n_l_exp(n_l_exp *l_exp){
	if(l_exp){
		analyse_n_exp(l_exp->tete);
		analyse_n_l_exp(l_exp->queue);
	}
}

/*-------------------------------------------------------------------------------------------*/

void analyse_n_l_instr(n_l_instr *l_instr){
	if(l_instr){
		analyse_n_instr(l_instr->tete);
		analyse_n_l_instr(l_instr->queue);
	}
}

/*-------------------------------------------------------------------------------------------*/

void compter_n_param(n_l_exp *n){
	if(n){
		nb_param++;
		compter_n_param(n->queue);
	}
}

/*-------------------------------------------------------------------------------------------*/

void verification_main(){
	if(rechercheExecutable("main") < 0){
		printf("ERROR : impossible de trouver la fonction main\n");
		exit(1);
	}
}
