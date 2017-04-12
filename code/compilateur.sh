#!/bin/bash

if [ "$1" == "-allfile" ]
then
  for file in LFile/*.l
  do
    fichier="$file"
    variable="${fichier##*/}"
    lfile="$variable"

    variable="${variable%%.*l}"
    echo "$lfile $variable"

    ./"src/"analyseurLL1 "LFile/"$lfile > "asm/$variable.asm"
    ./"src/"analyseurLL1_synt "LFile/"$lfile > "synt/$variable.synt"
    ./"src/"analyseurLL1_asynt "LFile/"$lfile > "asynt/$variable.asynt"
  done
  exit 0
fi
if [ $# -lt 2 ]
then
  echo "error usage :
          -c [file] to compile file
          -asynt [file] to get the abstract tree
          -synt [file] to get the syntaxic tree
          -all [file] to get all of the above
          -allfile to compile all the files"
fi
if [ "$1" == "-c" -a -f "$2" ]
then
  fichier="$2"
  variable="${fichier%%.*l}"
  variable="${variable##*/}"
  ./"src/"analyseurLL1 $2 > "asm/$variable.asm"
fi
if [ "$1" == "-asynt" -a -f "$2" ]
then
  fichier="$2"
  variable="${fichier%%.*l}"
  variable="${variable##*/}"
  ./"src/"analyseurLL1_asynt $2 > "asynt/$variable.asynt"
fi
if [ "$1" == "-synt" -a -f "$2" ]
then
  fichier="$2"
  variable="${fichier%%.*l}"
  variable="${variable##*/}"
  ./"src/"analyseurLL1_synt $2 > "synt/$variable.synt"
fi
if [ "$1" == "-all" -a -f "$2" ]
then
  fichier="$2"
  variable="${fichier%%.*l}"
  variable="${variable##*/}"
  ./"src/"analyseurLL1 $2 > "asm/$variable.asm"
  ./"src/"analyseurLL1_synt $2 > "synt/$variable.synt"
  ./"src/"analyseurLL1_asynt $2 > "asynt/$variable.asynt"
fi
