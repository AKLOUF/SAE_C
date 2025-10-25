#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include<assert.h>
enum 
{   
    MAX_ETUDIANTS = 100,
    J_MIN = 1, J_MAX = 40,
    MAX_NOM = 31,
    MAX_JUSTIFICATIF = 51,
    MAX_ABS = 500, 
    PERIODE = 3
};

/* Cette structure de données définie le type étudiant   
 contenant toutes les informations de l'étudiant */
typedef struct 
{
    char nom [MAX_NOM];         // Le nom de l'étudiant
    int groupe;                 // Le groupe de l'étudiant
    int id_etu;                 // L'idantifiant de l'étudiant
    int absence;                // Le nombre d'absences de l'étudiant    
} Etudiant;

/* Cette structure de données définie le type Absence 
  contenant toutes les informations sur l'absence d'un étudiant */ 
typedef struct 
{
    int id_abs;                                 // L'idantifiant de l'absence
    int id_etu;                                 // L'idantifiant de l'étudiant
    int j_abs;                                  // Le jour de l'absence
    int j_justif;                               // Le jour du dépôt du justificatif
    char dj [PERIODE];                          // La demi-journée de l'absence
    char justificatif[MAX_JUSTIFICATIF];        // Le texte decrivant le motif de l'absence
    int statut;                                 // 0: en attente de justificatif , 1: en attente de validation , 2: non justifiee , 3: justifiee   
} Absence;

/* Cette structure de données définie le type BUT qui contient 
un tableau d'étudiants et le nombre des étudiants réellement inscrits
*/
typedef struct 
{
    int Nb_Etudiants;                           // Le nombre effectif des étudiants inscrits
    Etudiant Etudiants[MAX_ETUDIANTS];          // Le tableau des étudiants inscrits
} But;

//La variable qui contient le tableau des étudiants et leur nombre 
But Promotion;

/* Cette structure de données définie le type Abs_BUT contenant 
le nombre total des absences de toute la promotion ainsi qu'un tableau decrivant toutes les absences
*/
typedef struct 
{
    int Nb_absence;                             // La taille du tableau des absences 
    Absence Absences[MAX_ABS];                  // Le tableau des absences de toute la promotion     
} Abs_But;
Abs_But Promotion_Abs;

// Fonction qui permet la sortie du programme 
void exit_programme () 
{
    exit(0);
}

/* Cette fonction permet de trier le tableau d'étudiants donné en entrée 
par groupe et par ordre alphabétique à l'interieur du même groupe */
void tri_etudiants (Etudiant etu[], int NB) 
{
    Etudiant temp;          // Variable tempooraire de type Etudiant pour l'utiliser dans la permeutation
    for (int i = 0; i < NB-1; i++) {
        for (int j = i + 1; j < NB; j++) {
            if (etu[i].groupe > etu[j].groupe ||                                   // on permute si le groupe est differents sinon
            (etu[i].groupe == etu[j].groupe && strcmp(etu[i].nom,etu[j].nom) > 0)) // on permute par ordre alphabétique si même groupe           
            {
                temp = etu[i];
                etu[i] = etu[j];
                etu[j] = temp;
            }
        }
    }
}

/* Cette fonction permet de trier le tableau des absences donné en entrée 
par étudiant et par ordre chronologique des absences pour le même étudiant*/
void tri_absences (Absence abs[] , int NB_abs)
{
    Absence temp;                                           // Variable tempooraire de type Absence pour l'utiliser dans la permeutation
    for (int i = 0; i < NB_abs-1; i++) {
        for (int j = i + 1; j < NB_abs; j++) {
            if (abs[i].id_etu > abs[j].id_etu ||                                // On permute si étudiant différent, sinon
            (abs[i].id_etu == abs[j].id_etu && abs[i].j_abs > abs[j].j_abs))    // on permute par jour d'absence pour le même étudiant
            {
                temp = abs[i];
                abs[i] = abs[j];
                abs[j] = temp;
            }
        }
    }
}

// Cette fonction trie les absences par identifiant, c'est à dire par ordre chronologique d'enregistrement 
void tri_absences_id (Absence abs[] , int NB_abs)
{
    Absence temp;
    int pos_min= 0;
    for (int i = 0; i < NB_abs-1; i++) 
    {
        pos_min=abs[i].id_abs;          // Initialiser la position du minimum
        for (int j = i + 1; j < NB_abs; j++) 
            if (abs[j].id_abs<abs[pos_min].id_abs) 
                pos_min=j;                // Chercher la position de l'identifiant minimal
            
        temp = abs[i];
        abs[i] = abs[pos_min];
        abs[pos_min] = temp;  
    }
}
/* Cette fonction permet l'inscription d'un étudiant et le remplissage d'une case du tableau 
 étudiant avec les informations données en entrée: le nom de l'étudiant et son groupe      */
void inscription (char *nom , int groupe)
{
    int i = 0;
    
    while (i < Promotion.Nb_Etudiants)      // Cherche si l'étudioant est déjà inscrit dans le même groupe
    {
        if (Promotion.Etudiants[i].groupe == groupe && strcmp(Promotion.Etudiants[i].nom , nom ) == 0)
        {
            printf ("Nom incorrect\n");
            return;
        }
        else 
            i++;
    }
    if (i == Promotion.Nb_Etudiants)    // le compteur a atteint la fin du tableau, donc, l'étudiant n'existe pas, on va l'incrire
    {
        Promotion.Etudiants[i].groupe = groupe ;
        Promotion.Etudiants[i].id_etu = i+1;
        strcpy(Promotion.Etudiants[i].nom , nom);   
        Promotion.Etudiants[i].absence = 0;
        Promotion.Nb_Etudiants++;
        printf("Inscription enregistree (%d)\n", Promotion.Nb_Etudiants);    
    }   
}

/* Cette fonction permet l'enregistrement d'une absence d'un étudiant et le remplissage 
d'une case du tableau Absence avec les informations données en entrée: l'identifiant de 
l'étudiant, le jour de l'absence et la demi-journée concernée      */
void absence (int id_etu ,int j , char *dj)
{
    if (id_etu > Promotion.Nb_Etudiants || id_etu < 1)  // test si l'identifiant saisi est incorrecte 
    {
        printf("Identifiant incorrect\n");
        return;
    }
    if(j <  J_MIN|| j >  J_MAX)                         // test si le jour saisi est incorrecte
    {
        printf("Date incorrecte\n");
        return;
    }    
    if(strcmp (dj,"am")!= 0 && strcmp (dj , "pm")!= 0)  // test si la demi-journée saisie est incorrecte
    {
        printf("Demi-journee incorrecte\n ");
        return;
    }
    for (int i = 0 ; i < Promotion_Abs.Nb_absence ; i++)    // Cherche si l'absence est déjà saisie auparavant
        if (Promotion_Abs.Absences[i].id_etu==id_etu && Promotion_Abs.Absences[i].j_abs == j && strcmp (Promotion_Abs.Absences[i].dj, dj) == 0)
        {
            printf("Absence deja connue\n");
            return;
        } 
    // Saisi des détails de l'absence car pas d'erreur dans les informations communiquées et l'absence n'est pas connue.
    Promotion_Abs.Absences [Promotion_Abs.Nb_absence].id_abs = Promotion_Abs.Nb_absence+1;    
    Promotion_Abs.Absences [Promotion_Abs.Nb_absence].id_etu = id_etu;
    Promotion_Abs.Absences [Promotion_Abs.Nb_absence].j_abs = j;
    Promotion_Abs.Absences [Promotion_Abs.Nb_absence].j_justif = 0;
    strcpy (Promotion_Abs.Absences[Promotion_Abs.Nb_absence].dj , dj);
    Promotion_Abs.Absences[Promotion_Abs.Nb_absence].statut = 0;
    Promotion.Etudiants[id_etu-1].absence++;
    printf("Absence enregistree [%d]\n", Promotion_Abs.Absences[Promotion_Abs.Nb_absence].id_abs);
    Promotion_Abs.Nb_absence++;   // incrémenter le nombre total d'absence
}

/* Cette fonction affiche pour chaque étudiant son nom, son groupe et le nombre total de ses absences
   l'affichage des étudiants devrait être par ordre croissant des groupes et par ordre alphabétique
   dans le même groupe   */
void etudiants (int j_crr)
{
    int abs_etu;
    if (j_crr <  J_MIN)                         // Test si le jour donné en entrée est incorrecte 
    {
        printf("Date incorrecte\n");
        return;
    }
    if (Promotion.Nb_Etudiants == 0)            // vérifie si la promotion est vide pas d'inscrit au jour courant
    {
        printf("Aucun inscrit\n");
        return;
    }
    tri_etudiants (Promotion.Etudiants , Promotion.Nb_Etudiants); // Trie les étudiants par groupe et par ordre alphabétique
    for (int i = 0 ; i < Promotion.Nb_Etudiants ; i++)
    {
        abs_etu = 0;    // initialisation du nombre total d'absence au jour courant pour chaque étudiant
        for (int j = 0 ; j < Promotion_Abs.Nb_absence ; j++)
        {
            if (Promotion.Etudiants[i].id_etu == Promotion_Abs.Absences[j].id_etu && 
            Promotion_Abs.Absences[j].j_abs <= j_crr)
                abs_etu++;      //Pour calculer le nombre total d'absence au jour courant
        }   
        printf("(%d) %-13s %-1d %-10d\n", Promotion.Etudiants[i].id_etu , Promotion.Etudiants[i].nom ,
        Promotion.Etudiants[i].groupe, abs_etu);
    }
}
/* Cette fonction permet à un étudiant de déposer un justification pour une absence donnée en précisant 
   l'identifiant de l'absence, la date de la justification et le motif, c'est à dire le texte du justificatif */ 
void justificatif (int id_abs , int j_justif , char *justificatif)
{
    int pos = 0; // variable pour identifier la position de l'absence dans le tableau Absence
    
    if (id_abs < 1 || id_abs > Promotion_Abs.Nb_absence) // Test si l'absence est déjà inscrite (id absence correcte)
    {
        printf("Identifiant incorrect \n");
        return;    
    }
    while (Promotion_Abs.Absences[pos].id_abs != id_abs)
        pos++;      // la position de l'absence dans le tableau  
    
    if (j_justif < Promotion_Abs.Absences[pos].j_abs) // test si la date est incorrecte (date du justificatif inférieur à la date de l'absence
    {
        printf("Date incorrecte\n");
        return; 
    }
    if (Promotion_Abs.Absences[pos].statut != 0 )   // Si l'étudiant a déjà déposé un justificatif pour cette absence
    {
        printf("Justificatif deja connu \n");
        return;
    }
   
    else if(j_justif <= Promotion_Abs.Absences[pos].j_abs + 3) // Mettre à jour le statut de l'absence si justif dans les délais ou non
        Promotion_Abs.Absences[pos].statut = 1;                // Jusitificatif dans les délais 
    else 
        Promotion_Abs.Absences[pos].statut = 2;                // Justificatif hors délai

    strcpy(Promotion_Abs.Absences[pos].justificatif , justificatif);
    Promotion_Abs.Absences[pos].j_justif = j_justif;            // Saisi le text du justificatif
    printf("Justificatif enregistre \n");
}
/*  Cette fonction affiche toutes l absences en attente de validatation pour tous les étudiants
    les absences sont triées par numéro d'étudiant et pour chaque étudiant, par aordre cronologique 
*/
void validations ()
{
    int j ;
    int trouve = 0;     // Je n'ai pas encore trouvé d'absence en attente de validation
    tri_absences (Promotion_Abs.Absences , Promotion_Abs.Nb_absence);   // Trie les absences par étudiant et par ordre chornologique d'absences 
    for(int i = 0 ; i < Promotion_Abs.Nb_absence ; i++)
    {
        if (Promotion_Abs.Absences[i].statut == 1)  // Chercher une absence dans le statut est en attente de validation
        {
            j = 0;
            while (Promotion.Etudiants[j].id_etu != Promotion_Abs.Absences[i].id_etu)
                j++;    // Chercher la position de l'étudiant dans le tableau étudiant
            
            printf("[%d] (%d) %-14s  %-2d  %d/%s  (%4s)\n", Promotion_Abs.Absences[i].id_abs ,
            Promotion_Abs.Absences[i].id_etu ,Promotion.Etudiants[j].nom , 
            Promotion.Etudiants[j].groupe , Promotion_Abs.Absences[i].j_abs ,Promotion_Abs.Absences[i].dj , 
            Promotion_Abs.Absences [i].justificatif);
            trouve = 1; // Au minimum l'étudiant a une absence en attente de validation
        }
    }
    if (trouve == 0)            // pas d'absence en attente de validation trouvée
        printf("Aucune validation en attente \n");
}
/* Cette fonction permet au directeur des études de valider ou d'invalider un 
   justificatif en précisant l'identifiant de l'absence et l'avis OK ou KO */ 
void validation (int id_abs , char *reponse)
{
    int pos = 0;  
    
    
    if (id_abs < 1 || id_abs > Promotion_Abs.Nb_absence)   // Vérifier si l'identifiant de l'absence est incorrecte ou non
    {
        printf("Identifiant incorrect \n");
        return;
    }
    while (Promotion_Abs.Absences[pos].id_abs != id_abs)
        pos++;   // Chercher la position de l'absence dans le tableau Absences
    
    if (Promotion_Abs.Absences[pos].statut == 3 )  // vérifier le statut de l'absence si Absence déjà valider
    {
        printf("Validation deja connue\n");
        return;
    }
    else if (strcmp (reponse , "ok") != 0 && strcmp (reponse , "ko") != 0) // Vérifier si l'avis saisi est incorrecte
    {
        printf("Code incorrect\n");
        return;
    }
    else if (strcmp (reponse ,"ok") == 0)  // Justificatif accepté
    {    
        printf("Validation enregistree\n");   
        Promotion_Abs.Absences[pos].statut = 3;  // Mettre à jour le statut de l'absence : Absence justifiée
        return;
    }
    else if (strcmp (reponse , "ko") == 0)// Justificatif refusé
    {
        printf("Validation enregistree\n");   
        Promotion_Abs.Absences[pos].statut = 2;  // Mettre à jour le statutde l'absence: Absence non justifiée
        return;
    }
}
/* Cette fonction affiche pour un itudiant donné et pour un jour donné 
   les informations de cet étudiant, ses absences classées par statut */ 
void etudiant (int id_etu, int j_crr) 
{   int nb_abs = 0; // nombre d'absence de l'étudiant
    int pos = 0;    // position de l'étudiant
    
    if (id_etu < 1 || id_etu > Promotion.Nb_Etudiants)  // Vérifier si l'identifiant de l'étudiant est incorrecte
    {
        printf("Identifiant incorrect\n");
        return;
    }
    if (j_crr <  J_MIN)                                 // Vérifier si le jour courant est incorrecte
    {
        printf("Date incorrecte\n");
        return;
    }
    
    tri_absences (Promotion_Abs.Absences , Promotion_Abs.Nb_absence); // Trie le tableau Absence par étudiant et jour d'absence
    while (Promotion.Etudiants[pos].id_etu != id_etu)
        pos++;          // Récupère la position de l'étudiant dans le tableau Etudiant
    
    for (int i = 0 ; i < Promotion_Abs.Nb_absence ; i++)
        if (Promotion_Abs.Absences[i].id_etu == id_etu && j_crr >= Promotion_Abs.Absences[i].j_abs) // Vérifie si pour cet étudiant, le jour de l'absence est avant le jour courant
            nb_abs++;        // Calcul le nombre total d'absences de l'étudiant au jour courant
    printf("(%d) %s %d %d\n", Promotion.Etudiants[pos].id_etu, Promotion.Etudiants[pos].nom, 
    Promotion.Etudiants[pos].groupe, nb_abs);

    int section_aff = 0;  // pour eviter d'afficher plusieurs fois la même section

    for (int i = 0; i < Promotion_Abs.Nb_absence; i++)
    {
        if (Promotion_Abs.Absences[i].id_etu == id_etu && Promotion_Abs.Absences[i].j_abs <= j_crr) // Vérifie si pour cet étudiant, le jour de l'absence est avant le jour courant
        {
            if  ((Promotion_Abs.Absences[i].statut == 0  && Promotion_Abs.Absences[i].j_abs+3 >= j_crr) || (j_crr < Promotion_Abs.Absences[i].j_justif))
            {       /*Vérifier si l'absence est en attente de justif (statut==0) et elle est toujours dans le délais (03 jours) par rapport au jour courant 
                      ou ne prendre que les absences dont la date de justif est inférieur au jour courant  */
                if (section_aff == 0) // Section pas encore affichée
                {
                    printf("- En attente justificatif\n");
                    section_aff++;
                }    
                printf("  [%d] %d/%s\n", Promotion_Abs.Absences[i].id_abs, Promotion_Abs.Absences[i].j_abs, Promotion_Abs.Absences[i].dj);
            }
        }
    }
    section_aff = 0;    // Réinitialiser la variable pour eviter d'afficher plusieurs fois la même section
    for (int i = 0; i < Promotion_Abs.Nb_absence; i++)
    {    
        if (Promotion_Abs.Absences[i].id_etu == id_etu && Promotion_Abs.Absences[i].j_abs <= j_crr) // Vérifie si pour cet étudiant, le jour de l'absence est avant le jour courant
        {    
            if (Promotion_Abs.Absences[i].j_justif <= j_crr && Promotion_Abs.Absences[i].statut == 1 && Promotion_Abs.Absences[i].j_justif != 0)
            {   // Vérifier si l'absence est en attente de validation (statut ==1) et la date de justificatif n'est pas null et elle est <= au jour courant
                if (section_aff == 0)    // Section pas encore affichée
                {       
                    printf("- En attente validation\n");
                    section_aff++ ;
                }
                printf("  [%d] %d/%s (%s)\n", Promotion_Abs.Absences[i].id_abs, Promotion_Abs.Absences[i].j_abs, 
                Promotion_Abs.Absences[i].dj , Promotion_Abs.Absences[i].justificatif);
            }
        }    
    }
    section_aff = 0; // Réinitialiser la variable pour eviter d'afficher plusieurs fois la même section
    for (int i = 0; i < Promotion_Abs.Nb_absence; i++)
    {    
        if (Promotion_Abs.Absences[i].id_etu == id_etu && Promotion_Abs.Absences[i].j_abs <= j_crr) // Vérifie si pour cet étudiant, le jour de l'absence est avant le jour courant
        {
            if (Promotion_Abs.Absences[i].j_abs <= j_crr && Promotion_Abs.Absences[i].statut == 3)
            {   // Vérifier si le jour d'absence < au jour courant et son statut est justifiée statut==3
                if (section_aff == 0)   // Section pas encore affichée
                {
                    printf("- Justifiees\n");
                    section_aff++;
                }          
                printf("  [%d] %d/%s (%s)\n", Promotion_Abs.Absences[i].id_abs, Promotion_Abs.Absences[i].j_abs, 
                Promotion_Abs.Absences[i].dj , Promotion_Abs.Absences[i].justificatif);
            }
        }
    }
    section_aff = 0;    // Réinitialiser la variable pour eviter d'afficher plusieurs fois la même section
    for (int i = 0 ; i < Promotion_Abs.Nb_absence ; i++)
    {         
        if ( Promotion_Abs.Absences[i].id_etu == id_etu && Promotion_Abs.Absences[i].j_abs <= j_crr)  // Vérifie si pour cet étudiant, le jour de l'absence est avant le jour courant
        {
            if ( (Promotion_Abs.Absences[i].j_abs+3 < j_crr && Promotion_Abs.Absences[i].j_justif == 0) || 
               (Promotion_Abs.Absences[i].statut == 2 && j_crr >= Promotion_Abs.Absences[i].j_justif )) 
            {   /* Vérifier si le statut de l'absence est non justifiée (statut==2) et la date de justificatif <= au jour courant
                   ou l'absence n'a pas été justifiée dans les délais au jour courant   */
                if (section_aff == 0)   // Section pas encore affichée
                {
                    printf("- Non-justifiees\n");
                    section_aff++;
                }       
                if(strcmp(Promotion_Abs.Absences[i].justificatif ,"") == 0) //
                    printf("  [%d] %d/%s \n", Promotion_Abs.Absences[i].id_abs, Promotion_Abs.Absences[i].j_abs, Promotion_Abs.Absences[i].dj);
                else 
                    printf("  [%d] %d/%s (%s) \n", Promotion_Abs.Absences[i].id_abs, Promotion_Abs.Absences[i].j_abs, Promotion_Abs.Absences[i].dj , Promotion_Abs.Absences[i].justificatif);
            }
        }
    }
}
//  Cette fonction affiche les étudiants dont les absences non justifiées sont supérieur ou égale à 05 au jour courant
void defaillants(int j_crr) 
{
    int trouve = 0;
    int nb_abs_nj;    
    int id_abs_jcrr = 0;
    int j = 0;

    if (j_crr <  J_MIN)         // Vérifier si le jour courant est incorrecte
    {
        printf("Date incorrecte\n");
        return;
    }    
    tri_absences_id (Promotion_Abs.Absences,  Promotion_Abs.Nb_absence);  // Trie le tableau absence par ordre chronologique des absences
    
    while (j < Promotion_Abs.Nb_absence && Promotion_Abs.Absences[j].j_abs <= j_crr)// Rechercher la dernière absence qui a été déclaré jusqu'a le jour courant  
    {
        id_abs_jcrr = Promotion_Abs.Absences[j].id_abs;
        j++;
    }
    
    for (int i = 0; i < Promotion.Nb_Etudiants; i++) 
    {
        if (Promotion.Etudiants[i].absence >= 5 )       // Prendre uniquement les étudiants dont le nombre d'absence dépasse 5
        {
            nb_abs_nj = 0;
            for (int j = 0 ; j < Promotion_Abs.Nb_absence ; j++)
            {
                if (Promotion_Abs.Absences[j].id_etu == Promotion.Etudiants[i].id_etu && Promotion_Abs.Absences[j].id_abs <= id_abs_jcrr)  // Vérifier les absences d'un étudiant déjà identifié 
                    if (Promotion_Abs.Absences[j].statut == 2 || (Promotion_Abs.Absences[j].j_justif == 0 && j_crr > Promotion_Abs.Absences[j].j_abs+3))
                    /* Comptabilise uniquement les absences dont le statut ==2 ou bien l'absence n'est pas encore justifiée au jour courant
                    et elle est enregistrée de plus de trois jour avant le jour courant */             
                    nb_abs_nj++;        // Calcul le nombre d'absences non jusitifées au jour courant
            }    
            if(nb_abs_nj >= 5)  // Affiche uniquement si nombre d'absence dépasse 5
            {
                printf("(%d) %s %d %d\n", Promotion.Etudiants[i].id_etu, Promotion.Etudiants[i].nom, Promotion.Etudiants[i].groupe,Promotion.Etudiants[i].absence);
                trouve = 1;
            }
        }
    }
    if (trouve == 0)
    { 
        printf("Aucun defaillant\n");
        return;
    }
}
// La fonction principale de l'interpreteur de commandes  
int main () 
{
    Promotion.Nb_Etudiants = 0;
    Promotion_Abs.Nb_absence = 0;
    char cde [31] = " ";    // La chaine de caractère qui stocke la commande
    do 
    {
        scanf("%s", cde);
        if (strcmp(cde, "inscription") == 0) 
        {
            char nom[MAX_NOM]; 
            int groupe; 
            scanf ("%s", nom);
            scanf("%d", &groupe);
            inscription (nom , groupe);
        }
        else if (strcmp (cde , "absence") == 0)
        {
            int id_etu , j; 
            char dj[PERIODE];
            scanf("%d", &id_etu );
            scanf ("%d", &j );
            scanf("%s", dj);
            absence ( id_etu , j , dj );
        }
        else if (strcmp (cde , "etudiants") == 0)
        {
            int j_crr;
            scanf("%d", &j_crr);
            etudiants (j_crr);
        }
        else if (strcmp (cde , "justificatif") == 0)
        {
            int id_abs;
            int j_justif;
            char justif [MAX_JUSTIFICATIF];
            scanf ("%d", &id_abs);
            scanf("%d", &j_justif);
            int c = getc (stdin);
            while (c == ' ' || c == '\t')
                c = getc (stdin);
            ungetc (c , stdin);
            fgets (justif , 51 , stdin );    
            justif[strlen (justif) - 1 ] = '\0';
            justificatif (id_abs , j_justif , justif);
        }
        else if (strcmp (cde , "validations") == 0)
            validations ();

        else if (strcmp (cde , "validation") == 0)
        {
            int id_abs;
            char reponse [3];
            scanf("%d", &id_abs );
            scanf("%s", reponse);
            validation (id_abs , reponse);
        }
        else if (strcmp (cde , "etudiant") == 0)
        {
            int id_etu , j_crr;
            scanf("%d", &id_etu);
            scanf("%d",&j_crr);
            etudiant (id_etu , j_crr );
        }
        else if (strcmp (cde , "defaillants") == 0)
        {
            int j_crr;
            scanf("%d",&j_crr);
            defaillants ( j_crr );
        }
    }
    while (strcmp(cde, "exit" ) != 0 );
    return 0;
}

