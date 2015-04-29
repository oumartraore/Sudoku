#include "Regle3.h"

/* Remarque : un groupe peut etre une ligne, une colonne ou une region */


/* Fonction retournant le nombre d'occurence de nbre dans les candidats d'une ligne */
/* le but c'est de verifier apres que le nombre d'occurence est egale 1 pour savoir que ce nombre ce repete 1x */

int NbRepetitionLigne(int nbre,Candidats_case cand,int ligne)
{
    int num_case=ligne*9; /* on rajoute pas la colonne pour se mettre o debut */
    int i;
    int res=0;

    for(i=num_case;i<COLONNES_MAX+num_case;i++)
    {
        if(EstDansTab(nbre,cand[i])) res++;
    }
    return res;
}

/* Fonction retournant le nombre d'occurence de nbre dans les candidats d'une colonne */

int NbRepetitionColonne(int nbre,Candidats_case cand,int colonne)
{
    int num_case=colonne;
    int i;
    int res=0;

    for(i=num_case;i<81;i+=9)
    {
        if(EstDansTab(nbre,cand[i])) res++;
    }
    return res;
}

/* Fonction retournant le nombre d'occurence de nbre dans les candidats d'une region */

int NbRepetitionRegion(int nbre,Candidats_case cand,int reg)
{
    int lig=3*(reg/3);
    int col=3*(reg%3);
    int num_case=lig*9+col;

    int i,j;
    int res=0;

    for(i=num_case;i<num_case+18+1;i=i+9) /* le 18+1 c'est pour parcourir 3 ligne apres ce qui est le nombre de ligne maximum pour une region */
    {
        for(j=0;j<3;j++)
        {
            if(EstDansTab(nbre,cand[i+j])) res++;
        }
    }
    return res;
}

/* Fonction retournant le candidat unique d'une ligne */

int CandidatUniqueLigne(Candidats_case cand,int ligne)
{
    T_Tab rep={0};
    int i;

    for(i=1;i<=9;i++)
    {
        if((NbRepetitionLigne(i,cand,ligne)==1)) rep[i-1]=i;
    }
    Trier(rep);
    if(NombreDeCazNonVides(rep)==1)
    {
        return rep[0];
    }
    else
    {
        return 0;
    }
}

/* Fonction retournant le candidat unique sur une colonne */

int CandidatUniqueColonne(Candidats_case cand,int colonne)
{
    T_Tab rep={0};
    int i;

    for(i=1;i<=9;i++)
    {
        if((NbRepetitionColonne(i,cand,colonne)==1)) rep[i-1]=i;
    }
    Trier(rep);
    if(NombreDeCazNonVides(rep)==1)
    {
        return rep[0];
    }
    else
    {
        return 0;
    }
}

/* Fonction retournant le candidat unique dans une region */

int CandidatUniqueRegion(Candidats_case cand,int reg)
{
    T_Tab rep={0};
    int i;

    for(i=1;i<=9;i++)
    {
        if((NbRepetitionRegion(i,cand,reg)==1)) rep[i-1]=i;
    }
    Trier(rep);
    if(NombreDeCazNonVides(rep)==1)
    {
        return rep[0];
    }
    else
    {
        return 0;
    }
}

/* Regle 3: Candidats Uniques */

void Regle3(Grille grille, Candidats_case cand,int pas_a_pas)
{
    int ligne,colonne,reg,num_case;
    int unique; /* variable qui serra tester si elle est differente de 0 */
    int i,j,k,num,cpt,pos;

    /* Pour chaque ligne, on met le candidat unique a la case dont les candidats contiennent le candidat unique */

    for(ligne=0;ligne<LIGNES_MAX;ligne++)
    {
        /* Le numero de la case au debut de la ligne en question */

        num_case=9*ligne; /* d'ou le retrait de la colonne dans la formule num_case = 9*ligne+colonne

        /* On trouve le candidat unique sur la ligne */

        unique=CandidatUniqueLigne(cand,ligne);

        /* Si il est different de zero i.e il existe */

        if(unique != 0)
        {
            for(i=0;i<9;i++)
            {
                /*Si le candidat unique sur la ligne est dans les candidats d'une case de la ligne, on le met a la case correspondante */

                if(EstDansTab(unique, cand[num_case+i])) /* plus i pour se deplacer en colonne a partir d'une ligne et donc atteindre le numero des case suivantes */
                {
                    grille[ligne][i]=unique;
                    /*if(pas_a_pas == 1)
                    {
                        AfficherGrille(grille,ligne,i);
                        printf("Appuyez une touche pour continuer\n");
                        getchar();
                    }*/

                    /* On reduit les candidats de la case a zero puisqu'elle est devoilee */

                    for(j=0;j<9;j++) cand[num_case+i][j]=0;

                    /* On enleve le candidat unique parmi les candidats de la colonne correspondant a sa case */

                    for(num=i;num<81;num+=9)
                    {
                        for(cpt=0;cpt<9;cpt++)
                        {
                            if(cand[num][cpt]== unique)
                            {
                                cand[num][cpt]=0;
                                Trier(cand[num]);
                            }
                        }
                    }

                    /* On enleve le candidat unique parmi les candidats de la region correspondant a sa case */

                    reg=Region(ligne,i);
                    pos=27*(reg/3)+3*(reg%3);

                    for(num=pos;num<pos+19;num+=9)
                    {
                        for(cpt=0;cpt<3;cpt++)
                        {
                            for(j=0;j<9;j++)
                            {
                                if(cand[num+cpt][j] == unique)
                                {
                                    cand[num+cpt][j]=0;
                                    Trier(cand[num+cpt]);
                                }
                            }
                        }
                    }
        /* REMARQUE : On a pas besoin de supprimer le candidat unique parmi les candidats de sa ligne parcequ'on sait deja qu'il est unique sur la ligne */
                }
            }
        }
    }

    /* Pour chaque colonne, on met le candidat unique a la case dont les candidats contiennent le candidat unique */

    for(colonne=0;colonne<COLONNES_MAX;colonne++)
    {
        /* Le numero de la case au debut de la colonne en question */

        num_case=colonne;

        /* On trouve le candidat unique sur la colonne */

        unique=CandidatUniqueColonne(cand, colonne);

        /* Si le candidat unique est different de zero i.e il existe */

        if(unique != 0 )
        {
            for(i=0;i<9;i++)
            {
                /* Si le candidat unique appartient a la liste des candidats d'une case, on met le candidat unique a la case correspondante */

                if(EstDansTab(unique, cand[num_case+(9*i)]))
                {
                    grille[i][colonne]=unique;
                    if(pas_a_pas == 1)
                    {
                        AfficherGrille(grille,i,colonne);
                        printf("Appuyez une touche pour continuer\n");
                        getchar();
                    }

                    /* On reduit les candidats de la case a zero */

                    for(j=0;j<9;j++) cand[num_case+(9*i)][j]=0;

                    /* On supprime le candidat unique parmi les candidats des autres cases de la ligne correspondante */

                    for(num=i*9;num<9*i+9;num++)
                    {
                        for(cpt=0;cpt<9;cpt++)
                        {
                            if(cand[num][cpt] == unique) cand[num][cpt]=0;Trier(cand[num]);
                        }
                    }

                    /* On supprime le candidat unique parmi les candidats des autres cases de la region correspondante */

                    reg=Region(i,colonne);
                    pos=27*(reg/3)+3*(reg%3);

                    for(num=pos;num<pos+19;num+=9)
                    {
                        for(cpt=0;cpt<3;cpt++)
                        {
                            for(j=0;j<9;j++)
                            {
                                if(cand[num+cpt][j] == unique)
                                {
                                    cand[num+cpt][j]=0;Trier(cand[num+cpt]);
                                }
                            }
                        }
                    }

                    /* REMARQUE : On a pas besoin de supprimer le candidat unique parmi les candidats de sa colonne parcequ'on sait deja qu'il est unique sur la colonne */

                }
            }
        }
    }

    /* Pour chaque region, on met le candidat unique a la case dont les candidats contiennent le candidat unique */

    for(reg=0;reg<9;reg++)
    {
        /* Le numero de la case en haut a gauche de la region en question */

        num_case=(27*(reg/3))+(3*(reg%3));

        /* On trouve le candidat unique dans la region */

        unique=CandidatUniqueRegion(cand, reg);

        /* S'il est different de zero i.e il existe */

        if(unique != 0)
        {
            for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                {

                    /* Si le candidat unique sur la region appartient a la liste des candidats d'une case; on le met a la case correspondante */

                    if(EstDansTab(unique, cand[(num_case+9*i+j)]))
                    {
                            ligne=(num_case+9*i+j)/9;
                            colonne=(num_case+9*i+j)%9;

                            grille[ligne][colonne]=unique;
                            if(pas_a_pas == 1)
                            {
                                AfficherGrille(grille,ligne,colonne);
                                printf("Appuyez une touche pour continuer\n");
                                getchar();
                            }

                            /* On reduit les candidats de la case a zero puisqu'elle est devoilee */

                            for(k=0;k<9;k++) cand[(num_case+9*i+j)][k]=0;

                            /* On supprime le candidat unique de la region parmi les candidats de la ligne */

                            for(num=ligne*9;num<9*ligne+9;num++)
                            {
                                for(cpt=0;cpt<9;cpt++)
                                {
                                    if(cand[num][cpt] == unique)
                                    {
                                        cand[num][cpt]=0;
                                        Trier(cand[num]);
                                    }
                                }
                            }

                            /* On supprime le candidat unique de la region parmi les candidats et de la colonne correspondant */

                            for(num=colonne;num<81;num+=9)
                            {
                                for(cpt=0;cpt<9;cpt++)
                                {
                                    if(cand[num][cpt]== unique)
                                    {
                                        cand[num][cpt]=0;
                                        Trier(cand[num]);
                                    }
                                }
                            }

                            /*REMARQUE : On a pas besoin de supprimer le candidat unique parmi les candidats de sa region parcequ'on sait deja qu'il est unique sur la region */
                    }
                }
            }
        }
    }
}


/* On applique la regle 3 pour resoudre la grille */

void AppliquerRegle3(Grille grille,Candidats_case cand,int pas_a_pas)
{
    AppliquerRegle1et2(grille,cand,pas_a_pas); /* on devoile certaines cases � l'aide de la regle 1 et 2 */
    Grille backup;
    /*On applique la regle 3 tant qu'il y'a une case qui contient
    Zero ou que les regles ne modifient plus la grille en entree*/
    do{
        CopierGrille(grille,backup);
        Regle3(grille,cand,pas_a_pas);

    }while((!(GrillesEgales(grille,backup))));
}
