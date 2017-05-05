ceci est le fichier readme du dossier source du projet de compilation L3

compilation du projet : make all

suppression des fichiers objets ainsi que des executables : make clean

les fichiers : analyseurLL1, analyseurLL1_synt, analyseurLL1_asynt sont des doublons de code, seul leurs paramètrages diffères.

./analyseurLL1 "nom-de-fichier-source" va produire la sortie assembleur de celui-ci.
./analyseurLL1_synt "nom-de-fichier-source" va produire l'arbre syntaxique de celui-ci.
./analyseurLL1_asynt "nom-de-fichier-source" va produire l'arbre abstrait de celui-ci.
