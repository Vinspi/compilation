/* symboles non terminaux */

#define EPSILON 0

#define NB_NON_TERMINAUX 41

#define _listeDecVariables_ 100
#define _listeDecFonctions_ 200
#define _declarationVariable_ 300
#define _declarationFonction_ 400
#define _listeParam_ 500
#define _listeInstructions_ 600
#define _listeExpressionsBis_ 700
#define _instruction_ 800
#define _instructionAffect_ 900
#define _instructionBloc_ 1000
#define _instructionSi_ 1100
#define _instructionTantque_ 1200
#define _instructionAppel_ 1300
#define _instructionRetour_ 1400
#define _instructionEcriture_ 1500
#define _instructionVide_ 1600
#define _var_ 1700
#define _expression_ 1800
#define _appelFct_ 1900
#define _conjonction_ 2000
#define _negation_ 2100
#define _comparaison_ 2200
#define _expArith_ 2300
#define _terme_ 2400
#define _facteur_ 2500
#define _argumentsEffectifs_ 2600
#define _listeExpressions_ 2700
#define _programme_ 2800
#define _conjonctionBis_ 2900
#define _optTailleTableau_ 3000
#define _expArithBis_ 3100
#define _optSinon_ 3200
#define _comparaisonBis_ 3300
#define _optDecVariables_ 3400
#define _optIndice_ 3500
#define _listeDecVariablesBis_ 3600
#define _instructionPour_ 3700
#define _termeBis_ 3800
#define _expressionBis_ 3900
#define _instructionFaire_ 4000
#define _optListeDecVariables_ 4100



/* symboles terminaux */
#define NB_TERMINAUX 32

#define POINT_VIRGULE 1
#define PLUS 2
#define MOINS 3
#define FOIS 4
#define DIVISE 5
#define PARENTHESE_OUVRANTE 6
#define PARENTHESE_FERMANTE 7
#define CROCHET_OUVRANT 8
#define CROCHET_FERMANT 9
#define ACCOLADE_OUVRANTE 10
#define ACCOLADE_FERMANTE 11
#define EGAL 12
#define ET 14
#define OU 15
#define NON 16
#define SI 17
#define ALORS 18
#define SINON 19
#define TANTQUE 20
#define FAIRE 21
#define ENTIER 22
#define RETOUR 23
#define LIRE 24
#define ECRIRE 25
#define ID_VAR 26
#define ID_FCT 27
#define NOMBRE 28
#define FIN 29
#define VIRGULE 30

#define SUPERIEUR 60
#define INFERIEUR 61
