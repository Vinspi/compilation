entier $a, entier $b, entier $tmp;


main(){
  $a = lire();
  $b = 0;
  $tmp = 0;
  tantque ($b < $a)
  faire{
    
    $b = $b + 1;
    $tmp = $tmp + $b;

  }
  ecrire($tmp);
}
