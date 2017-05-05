entier $a;

carre(entier $x){
  retour $x*$x;
}

main(){
  $a = lire();
  $a = carre(carre(carre($a)));
  ecrire($a);
}
