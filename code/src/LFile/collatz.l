
collatz(entier $n){
  ecrire($n);
  si($n = 1)
  alors
  {
    retour 1;
  }
  si($n/2*2 = $n)
  alors
  {
    retour collatz($n/2);
  }
  sinon
  {
    retour collatz((3*$n)+1);
  }
}

main(){
  ecrire(collatz(42));
}
