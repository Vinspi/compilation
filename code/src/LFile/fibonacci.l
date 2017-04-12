
fibonacci(entier $n)
entier $a, entier $b, entier $c, entier $d;
{
  $a = 1;
  $b = 0;
  $c = 1;
  si($n = 0)
  alors
  {
    ecrire(0);
    retour 0;
  }
  si($n = 1)
  alors
  {
    ecrire(1);
    retour 0;
  }
  tantque ($a < $n) faire
  {
    $a = $a +1;
    $d = $b + $c;
    $b = $c;
    $c = $d;
    ecrire($d);
  }
}

main(){
  fibonacci(42);
}
