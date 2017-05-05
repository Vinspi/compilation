entier $a;

asso1()
entier $a, entier $b, entier $c;
{
  $a = 8-4*2;
  ecrire($a);
  retour $a;
}

asso2()
entier $a;
{
  $a = 5-4+2;
  ecrire($a);
}

main()
{
  $a = 42;
  asso1();
  asso2();
  ecrire($a);
}
