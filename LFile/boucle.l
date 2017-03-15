entier $i, entier $carre[10], entier $tab, entier $x;
main()
{
    $tab[0] = 0;
    $i = 0;
    tantque $i < 10 faire
    {
        $carre = $i * $i;
        ecrire( $carre );
        $i = $i + 1;
    }
}
