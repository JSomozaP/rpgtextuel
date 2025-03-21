/*******************************************************************************
 * Ce programme est un jeu de rôle textuel (RPG) où tu incarnes un aventurier   *
 * qui doit faire des choix pour progresser dans son histoire. Chaque choix     *
 * aura des conséquences sur tes points de vie, ton endurance et ton or !      *
 *******************************************************************************/

// Ces lignes incluent des "boîtes à outils" dont notre programme a besoin
#include <stdio.h>  // Pour afficher du texte et lire ce que tape le joueur
#include <stdlib.h> // Pour plein d'outils pratiques comme convertir du texte en nombres
#include <string.h> // Pour manipuler du texte (copier, comparer, etc.)
#include <ctype.h>  // Pour vérifier si un caractère est un chiffre

// On définit la taille maximum d'un texte qu'on peut lire
// C'est comme définir la taille d'une feuille de papier !
#define BUF_SIZE 255 // Taille du buffer pour la lecture des chaînes

/*
 * Cette structure représente ton personnage, comme une fiche de héros !
 * Elle contient toutes les informations importantes sur ton aventurier :
 * - Ses points de vie (pv) : s'ils tombent à 0, c'est game over !
 * - Son endurance : pour résister aux épreuves
 * - Son nom : pour que le jeu s'adresse à toi personnellement
 * - Son or : pour acheter des objets ou payer ton chemin
 * - L'événement en cours : pour savoir où tu en es dans l'histoire
 */
struct player  // Structure pour représenter le joueur
{
    int pv;             // Points de vie (0 = mort)
    int endurance;      // Energie pour les actions difficiles
    char nomjoueur[20]; // Ton nom d'aventurier (maximum 19 lettres)
    int or;             // Ton argent pour acheter des objets
    int event;          // Pour suivre ta progression dans l'histoire
};

/*
 * Cette fonction nettoie le texte que tu écris
 * Quand tu appuies sur "Entrée", ça ajoute un retour à la ligne
 * Cette fonction enlève ce retour à la ligne pour que ce soit plus propre
 * C'est comme effacer les petits débordements sur ta feuille !
 */
void noretour(char *str) // Fonction pour supprimer le retour à la ligne d'une chaîne
{
    size_t len = strlen(str); // On compte la longueur du texte
    if (len > 0 && str[len - 1] == '\n')
    {                        // Si le dernier caractère est un retour à la ligne
        str[len - 1] = '\0'; // On le remplace par un marqueur de fin de texte
    }
}

/*
 * Cette fonction sauvegarde ta partie
 * C'est comme si tu prenais une photo de ton aventure pour la continuer plus tard !
 * Elle écrit toutes les informations de ton personnage dans un fichier magique
 * appelé "saverpg" (comme "save RPG" = "sauvegarder le jeu de rôle")
 */
void sauvegarder(struct player *joueur) // Fonction pour sauvegarder la progression du joueur
{
    // On ouvre le fichier magique en mode écriture ("w" pour "write" = écrire)
    FILE *save = fopen("saverpg", "w"); 
    if (save)
    { // Si le fichier s'est bien ouvert
        // On écrit toutes les informations de ton personnage
        fprintf(save, "%s %d %d %d %d\n",
                joueur->nomjoueur, // Ton nom
                joueur->pv,        // Tes points de vie
                joueur->endurance, // Ton endurance
                joueur->or,        // Ton or
                joueur->event);    // Où tu en es dans l'histoire
        fclose(save);              // On ferme le fichier magique
        printf("\nTon périple est gravé dans la pierre mémorielle\n\n");
    }
    else
    { // Si on n'arrive pas à ouvrir le fichier
        perror("Erreur d'ouverture du fichier");
        printf("\nImpossible de sauvegarder la partie\n\n");
    }
}

/*
 * Cette fonction charge une partie sauvegardée
 * C'est comme si tu regardais la photo de ton ancienne partie pour reprendre où tu en étais !
 * Elle lit toutes les informations de ton personnage depuis le fichier magique "saverpg"
 * et les remet dans le jeu comme si tu n'avais jamais arrêté de jouer
 */
void charger(struct player *joueur) // Fonction pour charger une sauvegarde
{
    // On ouvre le fichier magique en mode lecture ("r" pour "read" = lire)
    FILE *save = fopen("saverpg", "r");
    if (save)
    { // Si le fichier s'est bien ouvert
        // On lit toutes les informations de ton personnage dans l'ordre :
        // nom, points de vie, endurance, or, et événement en cours
        fscanf(save, "%s %d %d %d %d",
               joueur->nomjoueur, &joueur->pv, &joueur->endurance, &joueur->or, &joueur->event);
        fclose(save); // On referme le fichier magique
        printf("\nMatérialisation de ton périple par pierre mémorielle\n");
    }
}

/*
 * Cette fonction affiche tes statistiques
 * C'est comme regarder ta fiche de personnage dans un vrai jeu de rôle !
 * Elle montre :
 * - Ton nom d'aventurier
 * - Tes points de vie (PV)
 * - Ton endurance
 * - Combien d'or tu possèdes
 * - Où tu en es dans l'histoire (event)
 */
void stats1(struct player *joueur) // Fonction pour afficher les statistiques du joueur
{
    printf("\n---------Statistiques de %s---------\n", joueur->nomjoueur);
    printf("PV: %d\nEndurance: %d\nOr: %d\nEvent: %d\n",
           joueur->pv, joueur->endurance, joueur->or, joueur->event);
    printf("-------------------------------------\n");
}

/*
 * Cette fonction vérifie si tu es en difficulté et applique des bonus ou malus
 * C'est comme les règles d'un jeu de société qui disent ce qui se passe
 * quand tu n'as plus d'argent ou que tu es fatigué !
 *
 * Il y a plusieurs cas possibles :
 * 1. Si tes points de vie tombent à 0 ou moins → Game Over !
 * 2. Si tu as 1000 pièces d'or ou plus → Tu gagnes le jeu !
 * 3. Si tu n'as plus d'or → Tu perds de la vie et de l'endurance (tu as faim !)
 * 4. Si tu n'as plus d'endurance → Tu perds de la vie (tu es épuisé !)
 */
void malusbonus(struct player *joueur) // Fonction pour appliquer les malus/bonus en fonction des stats du joueur
{
    // Vérifie si le joueur est mort (PV ≤ 0)
    if (joueur->pv <= 0)
    {
        printf("Vous êtes mort, votre aventure se termine ici. "
               "Les légendes ne parleront jamais de vous.\n");
        exit(0); // Arrête le jeu
    }

    // Vérifie si le joueur est devenu riche (1000 or ou plus)
    if (joueur->or >= 1000)
    {
        printf("Vous êtes riche ! Votre péril touche enfin à sa fin "
               "et vos poches sont remplies d'or.\n");
        exit(0); // Arrête le jeu (victoire !)
    }

    // Vérifie si le joueur n'a plus d'or
    if (joueur->or <= 0)
    {
        printf("Votre bourse est désespérément vide ! "
               "Votre estomac crie famine, et vous perdez "
               "5 points de vie et d'endurance par tour.\n");
        joueur->endurance -= 5; // Perd 5 points d'endurance
        joueur->pv -= 5;        // Perd 5 points de vie
    }

    // Vérifie si le joueur n'a plus d'endurance
    if (joueur->endurance <= 0)
    {
        printf("Vous n'avez plus d'endurance ! "
               "Vous dépérissez à vue d'œil et perdez "
               "5 points de vie par tour.\n");
        joueur->pv -= 5; // Perd 5 points de vie
    }
}

/*
 * Cette fonction est le cœur du jeu ! C'est elle qui :
 * 1. Lit l'histoire depuis le fichier de quêtes
 * 2. Te montre les choix possibles
 * 3. Applique les conséquences de tes décisions
 *
 * C'est comme un maître du jeu qui lit l'histoire et gère ce qui se passe
 * selon tes choix !
 */
void executer_quete(struct player *joueur, FILE *fichier) // On lit une quête depuis un fichier
{
    // On prépare des "boîtes" pour stocker le texte qu'on va lire
    char ligne[BUF_SIZE];                             // Pour lire une ligne du fichier (Buffer pour stocker une ligne lue depuis le fichier)
    char texte_quete[BUF_SIZE * 10] = "";             // Pour l'histoire de la quête (Buffer pour stocker le texte de la quête)
    char choix[3][BUF_SIZE * 5] = {"", "", ""};       // Pour les 3 choix possibles (Tableau pour stocker les 3 choix possibles)
    char consequence[3][BUF_SIZE * 5] = {"", "", ""}; // Pour ce qui arrive après chaque choix (Tableau pour stocker les 3 conséquences possibles)
    int i = 0;                                        // Pour compter où on en est
    int choix_joueur = -1;                            // Pour enregistrer ton choix (-1 = pas encore choisi)

    /* ÉTAPE 1 : Lire l'histoire de la quête */
    if (fgets(ligne, BUF_SIZE, fichier) == NULL) // On lit une ligne du fichier
    {
        return; // Si on ne peut pas lire, on arrête
    }
    noretour(ligne);            // On nettoie la ligne
    strcpy(texte_quete, ligne); // On la copie dans notre boîte à histoire

    /* ÉTAPE 2 : Lire les trois choix possibles et leurs conséquences */
    for (i = 0; i < 3; i++) // On lit les 3 choix possibles
    {
        // D'abord, on lit le choix
        if (fgets(ligne, BUF_SIZE, fichier) == NULL) // Si on ne peut pas lire
        {
            fprintf(stderr, "Erreur de lecture des choix\n");
            return;
        }
        noretour(ligne);

        // On cherche le symbole '|' qui sépare le choix de sa conséquence
        char *sep = strchr(ligne, '|'); // On cherche le symbole '|'
        if (sep != NULL)               // Si on a trouvé le symbole '|'
        {
            *sep = '\0';                     // On coupe la ligne au niveau du '|'
            strcpy(choix[i], ligne);         // On copie le choix
            strcpy(consequence[i], sep + 1); // On copie la conséquence
        }
        else
        {
            fprintf(stderr, "Erreur de format du fichier de quêtes\n"); // Si on n'a pas trouvé le '|'
            return;
        }
    }

    /* ÉTAPE 3 : Montrer l'histoire et les choix au joueur */
    printf("\n%s\n", texte_quete); // Affiche le texte de la quête
    for (i = 0; i < 3; i++)
    {                                       // Boucle pour afficher les 3 choix
        printf("%d. %s\n", i + 1, choix[i]);// Affiche chaque choix avec un numéro
    }

    /* ÉTAPE 4 : Attendre que le joueur fasse un choix valide */
    do
    {
        printf("Votre choix (1-3): "); // Demande au joueur de faire un choix
        // On vérifie que le joueur entre bien un nombre
        if (scanf("%d", &choix_joueur) != 1) // Si l'entrée n'est pas un nombre
        {
            fprintf(stderr, "Erreur : Entrée invalide.\n");
            while (getchar() != '\n')
                ; // On nettoie ce qui reste dans l'entrée en vidant le buffer
            choix_joueur = -1;
        }
        // On vérifie que le nombre est entre 1 et 3
        else if (choix_joueur < 1 || choix_joueur > 3)
        {                   //si le choix est invalide
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
        }
    } while (choix_joueur < 1 || choix_joueur > 3); // boucle tant que le choix n'est pas valide

    /* ÉTAPE 5 : Appliquer la conséquence du choix */
    // On prépare un espace pour traiter la conséquence
    char consequence_buffer[BUF_SIZE * 5]; // Pour stocker la conséquence
    strncpy(consequence_buffer, consequence[choix_joueur - 1], BUF_SIZE * 5 - 1); // On copie la conséquence
    consequence_buffer[BUF_SIZE * 5 - 1] = '\0';  // On s'assure que la chaîne est bien terminée

    // On sépare le texte de la conséquence des modifications de stats
    char *texte = consequence_buffer; // On garde une copie du texte
    char *sep2 = strchr(texte, '|'); // On cherche le symbole '|'
    if (sep2 != NULL) // S'il y a un '|'
    {
        *sep2 = '\0';            // On coupe au niveau du '|'
        printf("\n%s\n", texte); // On affiche ce qui se passe

        // On fait une copie pour pouvoir la modifier sans danger
        char consequence_copy[BUF_SIZE * 5]; // On copie la conséquence
        strncpy(consequence_copy, consequence[choix_joueur - 1], BUF_SIZE * 5 - 1); // On copie la conséquence
        consequence_copy[BUF_SIZE * 5 - 1] = '\0';  // On s'assure que la chaîne est bien terminée

        // On cherche toutes les modifications de stats (ce qui commence par '|')
        char *token = consequence_copy; // On garde une copie de la conséquence
        while ((token = strchr(token, '|'))) // Tant qu'on trouve un '|'
        {
            token++; // On avance après le '|'
            // On regarde quel type de modification il faut faire
            if (strncmp(token, "pv+", 3) == 0) // Gagner des points de vie
                joueur->pv += atoi(token + 3); // On ajoute les points de vie
            else if (strncmp(token, "pv-", 3) == 0) // Perdre des points de vie
                joueur->pv -= atoi(token + 3); // On retire les points de vie
            else if (strncmp(token, "endurance+", 10) == 0) // Gagner de l'endurance
                joueur->endurance += atoi(token + 10); // On ajoute de l'endurance
            else if (strncmp(token, "endurance-", 10) == 0) // Perdre de l'endurance
                joueur->endurance -= atoi(token + 10); // On retire de l'endurance
            else if (strncmp(token, "or+", 3) == 0) // Gagner de l'or
                joueur->or += atoi(token + 3); // On ajoute de l'or
            else if (strncmp(token, "or-", 3) == 0) // Perdre de l'or
                joueur->or -= atoi(token + 3); // On retire de l'or
            else if (strncmp(token, "event+", 6) == 0) // Avancer dans l'histoire
                joueur->event += atoi(token + 6); // On avance dans l'histoire
        }
    }
    else
    {
        printf("\n%s\n", texte); // S'il n'y a pas de '|', on affiche juste le texte
    }

    /* ÉTAPE 6 : Mettre à jour et vérifier l'état du joueur */
    stats1(joueur);     // On montre les statistiques actualisées
    malusbonus(joueur); // On vérifie si le joueur est en difficulté
}

/*
 * La fonction main est comme le chef d'orchestre de notre jeu !
 * C'est elle qui :
 * 1. Crée ton personnage
 * 2. Te demande si tu veux charger une ancienne partie
 * 3. Lance les quêtes
 * 4. Te propose de sauvegarder à la fin
 *
 * C'est un peu comme le menu principal d'un jeu vidéo !
 */
int main()
{
    /* ÉTAPE 1 : Créer ton personnage avec ses stats de départ */
    struct player joueur;   // On crée un nouveau héros
    joueur.pv = 100;        // Il commence avec 100 points de vie
    joueur.endurance = 100; // 100 points d'endurance
    joueur.or = 100;        // 100 pièces d'or
    joueur.event = 0;       // Et il n'a pas encore vécu d'événements

    /* ÉTAPE 2 : Afficher le titre du jeu */
    printf("\n|---------------------------------------------|");
    printf("\n|--------------------RPG Pouet----------------|\n\n");

    /* ÉTAPE 3 : Proposer de charger une partie existante */
    printf("Charger une sauvegarde ? (o/n) : ");
    char choix;           // On crée une variable pour stocker ta réponse
    scanf(" %c", &choix); // On lit ta réponse
    getchar();            // On nettoie le retour à la ligne

    if (choix == 'o' || choix == 'O')
    {                     // Si tu veux charger ('o' ou 'O' pour "oui")
        charger(&joueur); // On charge ta sauvegarde
    }
    else
    { // Sinon, on crée un nouveau personnage
        printf("\nAventurier, quel est ton nom ?\n\n");
        fgets(joueur.nomjoueur, sizeof(joueur.nomjoueur), stdin); // On lit ton nom
        noretour(joueur.nomjoueur);                               // On nettoie le retour à la ligne
    }

    /* ÉTAPE 4 : Accueillir le joueur et montrer ses stats */
    printf("\nBienvenue aventurier %s\n\n", joueur.nomjoueur);
    stats1(&joueur); // On affiche tes statistiques de départ

    /* ÉTAPE 5 : Ouvrir le livre des quêtes */
    FILE *fichier_quetes = fopen("quetes.txt", "r"); // On ouvre le fichier des quêtes
    if (!fichier_quetes)
    { // Si on n'arrive pas à l'ouvrir
        perror("Erreur d'ouverture du fichier quetes.txt");
        return 1; // On arrête le jeu avec une erreur
    }

    /* ÉTAPE 6 : Lire et jouer chaque quête */
    char ligne[BUF_SIZE]; // On crée un buffer pour lire les lignes
    while (fgets(ligne, BUF_SIZE, fichier_quetes) != NULL) 
    { // Tant qu'il y a des lignes à lire
        if (strncmp(ligne, "===", 3) == 0)  
        {                                            // Si on trouve le début d'une quête
            executer_quete(&joueur, fichier_quetes); // On lance la quête
        }
    }

    fclose(fichier_quetes); // On referme le livre des quêtes

    /* ÉTAPE 7 : Proposer de sauvegarder la partie */
    printf("\nSauvegarder la progression ? (o/n) : ");
    fflush(stdout);       // On s'assure que tout est bien affiché
    scanf(" %c", &choix); // On lit ton choix
    if (choix == 'o' || choix == 'O')
    {                         // Si tu veux sauvegarder
        sauvegarder(&joueur); // On sauvegarde ta partie
    }

    return 0; // Le jeu se termine normalement
}
