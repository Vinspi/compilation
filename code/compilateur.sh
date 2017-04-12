#!/bin/bash

if [ $# -lt 2 ]
then
  echo "error usage :
          -c [file] to compile file
          -asynt [file] to get the abstract tree
          -synt [file] to get the syntaxic tree
          -all [file] to get all of the above"
fi
if [ "$1" == "-c" -a -f "$2" ]
then
  fichier="$2"
  variable="${fichier%%.*l}"
  ./analyseurLL1 $2 > "$variable.asm"
fi
