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
void empile(int reg);
void depile(int reg);

// int ajouteIdentificateur(char *identif,  int portee, int type, int adresse, int complement);

int portee;
int adresseLocaleCourante;
int adresseArgumentCourant;
int adresseGlobaleCourante;
int nb_param;
int trace = 1;
int buffer[80];
int nu_registre = 1;
int nu_etiquette = 0;
int etiquetteCourante = 0;
int flag_etiquette = 0;
int nu_etiquette_si = 0;
int nu_etiquette_tq = 0;
int nb_var_locale = 0;


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
	affiche_texte("\tjal f_main",trace);
	affiche_texte("\tli $v0, 10",trace);
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
		nb_var_locale++;
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
	nb_var_locale = 0;
	if(rechercheDeclarative(n->nom) >= 0){
		printf("ERROR: redeclaration de la fonction %s\n",n->nom);
		exit(1);
	}
	sprintf(buffer,"f_%s:",n->nom);
	affiche_texte(buffer,trace);

	sprintf(buffer,"\tsubi	$sp, $sp, 4\t#empile registre");
	affiche_texte(buffer,trace);

	sprintf(buffer,"\tsw	$fp, 0($sp)");
	affiche_texte(buffer, trace);

	sprintf(buffer,"\tmove	$fp, $sp\t#nouvelle valeur de $fp");
	affiche_texte(buffer, trace);

	sprintf(buffer,"\tsubi	$sp, $sp, 4\t# empile registre");
	affiche_texte(buffer, trace);

	sprintf(buffer,"\tsw	$ra, 0($sp)");
	affiche_texte(buffer, trace);

	compteParametres(n->u.foncDec_.param);



	ajouteIdentificateur(n->nom,P_VARIABLE_GLOBALE,T_FONCTION,0,nb_param); // ici on connait le nombre d'args de la fonction
	entreeFonction();
	portee = P_ARGUMENT;
	ajoute_l_dec(n->u.foncDec_.param);
	portee = P_VARIABLE_LOCALE;
	//entreeFonction();
	ajoute_l_dec(n->u.foncDec_.variables);

	if(nb_var_locale > 0){
		sprintf(buffer,"\tsubi $sp $sp %d\t#on alloue les variables locales",4*nb_var_locale);
		affiche_texte(buffer,trace);
	}
	// on ne fait pas encore le check du corps de la fonction

	analyse_n_instr(n->u.foncDec_.corps);

	//afficheTabsymboles();

	if(nb_var_locale > 0){
		sprintf(buffer,"\taddi $sp $sp %d\t#on désalloue les variables locales",4*nb_var_locale);
		affiche_texte(buffer,trace);
	}

	sprintf(buffer,"\tlw	$ra, 0($sp)\t# depile vers registre");
	affiche_texte(buffer,trace);

	sprintf(buffer,"\taddi	$sp, $sp, 4");
	affiche_texte(buffer,trace);

	sprintf(buffer,"\tlw	$fp, 0($sp)\t# depile vers registre");
	affiche_texte(buffer,trace);

	sprintf(buffer,"\taddi	$sp, $sp, 4");
	affiche_texte(buffer,trace);

	sprintf(buffer,"\tjr	$ra");
	affiche_texte(buffer,trace);

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
	int etiquette_si,etiquette_fin_si, reg_1, etiquette_test, etiquette_fin_tq;
	if(corps == NULL) return;
	int current_register = 1;
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
			//current_register = analyse_n_exp(corps->u.affecte_.exp);

			analyse_n_exp(corps->u.affecte_.exp);
			depile(current_register);
			if(tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].portee == P_VARIABLE_GLOBALE){
				sprintf(buffer,"\tsw $t%d, %s \t#sauve la variable",current_register,corps->u.affecte_.var->nom);
				affiche_texte(buffer,trace);
			}
			if(tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].portee == P_VARIABLE_LOCALE){
				sprintf(buffer,"\tsw $t%d, -%d($fp) \t#sauve la variable",current_register,8+tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].adresse);
				affiche_texte(buffer,trace);
			}
			if(tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].portee == P_ARGUMENT){
				sprintf(buffer,"\tsw $t%d, %d($fp) \t#sauve la variable",current_register,4+tabsymboles.tab[rechercheExecutable(corps->u.affecte_.var->nom)].adresse);
				affiche_texte(buffer,trace);
			}
			break;
		case siInst:
			etiquette_si = nu_etiquette_si;
			nu_etiquette_si++;
			etiquette_fin_si = nu_etiquette_si;
			nu_etiquette_si++;

			//reg_1 = analyse_n_exp(corps->u.si_.test);
			analyse_n_exp(corps->u.si_.test);
			depile(reg_1);
			sprintf(buffer,"\tbeq $t%d, $0, si%d",reg_1,etiquette_si);
			affiche_texte(buffer,trace);
			analyse_n_instr(corps->u.si_.alors);
			/* si pas de sinon pas besoin de jump par dessus la section sinon */
			if(corps->u.si_.sinon != NULL){
				sprintf(buffer,"\tj si%d",etiquette_fin_si);
				affiche_texte(buffer,trace);
			}
			sprintf(buffer,"si%d:",etiquette_si);
			affiche_texte(buffer,trace);
			analyse_n_instr(corps->u.si_.sinon);
			if(corps->u.si_.sinon != NULL){
				sprintf(buffer,"si%d:",etiquette_fin_si);
				affiche_texte(buffer,trace);
			}
			break;
		case faireInst:
			analyse_n_exp(corps->u.faire_.test);
			analyse_n_instr(corps->u.faire_.faire);
			break;
		case tantqueInst:
			/* on reserve les etiquettes */
			etiquette_test = nu_etiquette_tq;
			nu_etiquette_tq++;
			etiquette_fin_tq = nu_etiquette_tq;
			nu_etiquette_tq++;
			/* on place l'etiquette de test */
			sprintf(buffer,"tq%d:",etiquette_test);
			affiche_texte(buffer,trace);
			//reg_1 = analyse_n_exp(corps->u.tantque_.test);
			analyse_n_exp(corps->u.tantque_.test);
			depile(reg_1);
			/* si c'est faux on branche à la sortie */
			sprintf(buffer,"\tbeq $t%d $0 tq%d",reg_1,etiquette_fin_tq);
			affiche_texte(buffer,trace);
			analyse_n_instr(corps->u.tantque_.faire);
			sprintf(buffer,"\tj tq%d",etiquette_test);
			affiche_texte(buffer,trace);
			sprintf(buffer,"tq%d:",etiquette_fin_tq);
			affiche_texte(buffer,trace);
			break;
		case appelInst:
			analyser_n_appel(corps->u.appel);
			break;
		case retourInst:
			analyse_n_exp(corps->u.retour_.expression);
			break;
		case ecrireInst:
			//current_register = analyse_n_exp(corps->u.retour_.expression);
			analyse_n_exp(corps->u.retour_.expression);
			depile(current_register);
			sprintf(buffer,"\tli $v0, 1");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tmove $a0, $t%d",current_register);
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
	int reg_1, reg_2, reg_return,tmp, flag=0, etiquette;
	if(!flag_etiquette){
		 flag = 1;
		 flag_etiquette = 1;
	 }
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
			if(tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].portee == P_VARIABLE_GLOBALE){
				sprintf(buffer,"\tlw $t%d %s",nu_registre,exp->u.var->nom);
				affiche_texte(buffer,trace);
				empile(1);
			}
			if(tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].portee == P_VARIABLE_LOCALE){
				sprintf(buffer,"\tlw $t%d -%d($fp)",1,8+tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].adresse);
				affiche_texte(buffer,trace);
				empile(1);
			}
			if(tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].portee == P_ARGUMENT){
				sprintf(buffer,"\tlw $t%d, %d($fp) \t#sauve la variable",1,4+tabsymboles.tab[rechercheExecutable(exp->u.var->nom)].adresse);
				affiche_texte(buffer,trace);
				empile(1);
			}
			return nu_registre-1;

		case opExp:

			switch (exp->u.opExp_.op) {
				case non:
					analyse_n_exp(exp->u.opExp_.op1);
					depile(1);
					sprintf(buffer,"\tseq $t%d, $t%d, $0",1,1);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case plus:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					sprintf(buffer,"\tadd $t%d, $t%d, $t%d",1,1,2);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case moins:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					sprintf(buffer,"\tsub $t%d, $t%d, $t%d",1,2,1);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case fois:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					sprintf(buffer,"\tmult $t%d, $t%d",1,2);
					affiche_texte(buffer,trace);
					sprintf(buffer,"\tmflo $t%d",1);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case divise:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					sprintf(buffer,"\tdiv $t%d, $t%d",2,1);
					affiche_texte(buffer,trace);
					sprintf(buffer,"\tmflo $t%d",1);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case modulo:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					sprintf(buffer,"\tdiv $t%d, $t%d",2,1);
					affiche_texte(buffer,trace);
					sprintf(buffer,"\tmfhi $t%d",1);
					affiche_texte(buffer,trace);
					empile(1);
					// reg_return = nu_registre;
					// nu_registre++;
					break;
				case et:
					tmp = nu_etiquette;
					nu_etiquette++;
					etiquetteCourante = nu_etiquette;
					analyse_n_exp(exp->u.opExp_.op1);
					depile(1);

					sprintf(buffer,"\tbeq $t%d, $0 e%d",1,tmp);

					affiche_texte(buffer,trace);
					etiquetteCourante = tmp;
					nu_etiquette++;
					analyse_n_exp(exp->u.opExp_.op2);
					depile(2);
					sprintf(buffer,"e%d:",tmp);
					affiche_texte(buffer,trace);

					empile(0);
					break;
				case ou:
					tmp = nu_etiquette;
					nu_etiquette++;
					etiquetteCourante = nu_etiquette;
					analyse_n_exp(exp->u.opExp_.op1);
					depile(1);
					sprintf(buffer,"\tbne $t%d, $0 e%d",1,tmp);

					affiche_texte(buffer,trace);
					etiquetteCourante = tmp;
					nu_etiquette++;
					analyse_n_exp(exp->u.opExp_.op2);
					depile(2);
					sprintf(buffer,"e%d:",tmp);
					affiche_texte(buffer,trace);
					empile(0);
					break;
				case inf:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					// sprintf(buffer,"\tli $t0, -1");
					// affiche_texte(buffer,trace);
					// sprintf(buffer,"\tblt $t%d, $t%d, e%d",2,1,etiquetteCourante);
					// affiche_texte(buffer,trace);
					// sprintf(buffer,"\tli $t0, 0");
					// affiche_texte(buffer,trace);
					// if(nu_etiquette == etiquetteCourante){
					// 	sprintf(buffer,"e%d:",etiquetteCourante);
					// 	affiche_texte(buffer,trace);
					// }
					sprintf(buffer,"\tslt $t%d, $t%d, $t%d",0,2,1);
					affiche_texte(buffer,trace);
					empile(0);
					break;
				case egal:
					analyse_n_exp(exp->u.opExp_.op1);
					analyse_n_exp(exp->u.opExp_.op2);
					depile(1);
					depile(2);
					// sprintf(buffer,"\tli $t0, -1");
					// affiche_texte(buffer,trace);
					// sprintf(buffer,"\tbeq $t%d, $t%d, e%d",1,2,etiquetteCourante);
					// affiche_texte(buffer,trace);
					// sprintf(buffer,"\tli $t0, 0");
					// affiche_texte(buffer,trace);
					// if(nu_etiquette == etiquetteCourante){
					// 	sprintf(buffer,"e%d:",etiquetteCourante);
					// 	affiche_texte(buffer,trace);
					// }
					sprintf(buffer,"\tseq $t%d, $t%d, $t%d",0,2,1);
					affiche_texte(buffer,trace);
					empile(0);
					break;


			}

			return reg_return;
		case intExp: // attention !!!!!!!!!!!
			sprintf(buffer,"\tli $t%d, %d",1,exp->u.entier);
			affiche_texte(buffer,trace);
			empile(1);
			return nu_registre-1;

		case appelExp:
			analyser_n_appel(exp->u.appel);
			break;

		case lireExp:
			sprintf(buffer,"\tli $v0, 5");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tsyscall");
			affiche_texte(buffer,trace);
			sprintf(buffer,"\tmove $t%d, $v0",1);
			affiche_texte(buffer,trace);
			empile(1);
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

	sprintf(buffer,"\tjal f_%s",appel->fonction);
	affiche_texte(buffer,trace);
	sprintf(buffer,"\taddi $sp $sp %d",4*tabsymboles.tab[rechercheExecutable(appel->fonction)].complement);
	affiche_texte(buffer,trace);
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

/*-------------------------------------------------------------------------------------------*/

void empile( int reg ) {
	sprintf(buffer,"\tsubu $sp, $sp, 4\t# alloue un mot sur la pile" );
	affiche_texte(buffer,trace);
	sprintf(buffer,"\tsw $t%d, 0($sp)\t# copie reg vers sommet de pile", reg );
	affiche_texte(buffer,trace);
}

/*-------------------------------------------------------------------------------------------*/

void depile( int reg ) {
	sprintf(buffer,"\tlw $t%d, 0($sp)\t# copie sommet de pile vers reg", reg );
	affiche_texte(buffer,trace);
	sprintf(buffer,"\taddu $sp, $sp, 4\t# desalloue un mot sur la pile" );
	affiche_texte(buffer,trace);
}
