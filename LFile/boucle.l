max( entier $a, entier $b )
{
    si $a < $b alors {
        ecrire($b);
    }
    sinon
    {
      ecrire($a);
    }
}

main()
entier $v_1, entier $v_2;
{
    $v_1 = lire();
    $v_2 = lire();
    max($v_1,$v_2);

}
