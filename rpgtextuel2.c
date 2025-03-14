#include <stdio.h>  // Inclut la bibliothèque standard d'entrée/sortie pour utiliser printf, scanf, etc.
#include <stdlib.h> // Inclut la bibliothèque standard pour utiliser des fonctions comme exit, atoi, etc.
#include <string.h> // Inclut la bibliothèque de manipulation de chaînes de caractères pour utiliser strlen, strcat, etc.

#define BUF_SIZE 255 // Définit la taille du buffer pour la lecture de chaînes de caractères

// Structure représentant le joueur et ses statistiques
struct player {
    int pv;              // Points de vie du joueur
    int endurance;       // Endurance du joueur
    char nomjoueur[20];  // Nom du joueur (20 caractères maximum)
    int or;              // Quantité d'or possédée par le joueur
    int event;           // Événement en cours (par exemple, une quête spécifique)
};

// Fonction pour supprimer le retour à la ligne d'une chaîne de caractères
void noretour(char *str) {
    size_t len = strlen(str); // Calcule la longueur de la chaîne de caractères
    if (len > 0 && str[len - 1] == '\n') { // Vérifie si le dernier caractère est un retour à la ligne
        str[len - 1] = '\0'; // Remplace le retour à la ligne par un caractère nul pour terminer la chaîne
    }
}

// Fonction pour sauvegarder la progression du joueur dans un fichier
void sauvegarder(struct player *joueur) {
    FILE *save = fopen("saverpg", "w"); // Ouvre le fichier "saverpg" en mode écriture ("w")
    if (save) { // Vérifie si le fichier a été ouvert avec succès
        // Écrit les informations du joueur dans le fichier
        fprintf(save, "%s %d %d %d %d\n", joueur->nomjoueur, joueur->pv, joueur->endurance, joueur->or, joueur->event);
        fclose(save); // Ferme le fichier
        printf("\nTon périple est gravé dans la pierre mémorielle\n\n"); // Affiche un message de confirmation
    } else {
        perror("Erreur d'ouverture du fichier"); // Affiche un message d'erreur si le fichier n'a pas pu être ouvert
        printf("\nImpossible de sauvegarder la partie\n\n"); // Affiche un message d'erreur
    }
}

// Fonction pour charger une sauvegarde si elle existe
void charger(struct player *joueur) {
    FILE *save = fopen("saverpg", "r"); // Ouvre le fichier "saverpg" en mode lecture ("r")
    if (save) { // Vérifie si le fichier a été ouvert avec succès
        // Lit les informations du joueur depuis le fichier
        fscanf(save, "%s %d %d %d %d", joueur->nomjoueur, &joueur->pv, &joueur->endurance, &joueur->or, &joueur->event);
        fclose(save); // Ferme le fichier
        printf("\nMatérialisation de ton périple par pierre mémorielle\n"); // Affiche un message de confirmation
    }
}

// Fonction pour afficher les statistiques du joueur
void stats1(struct player *joueur) {
    printf("\n---------Statistiques de %s---------\n", joueur->nomjoueur); // Affiche le nom du joueur
    printf("PV: %d\nEndurance: %d\nOr: %d\nEvent: %d\n", joueur->pv, joueur->endurance, joueur->or, joueur->event); // Affiche les stats
    printf("-------------------------------------\n"); // Affiche une ligne de séparation
}

// Fonction pour appliquer les malus/bonus en fonction des stats du joueur
void malusbonus(struct player *joueur) {
    if (joueur->pv <= 0) { // Si les points de vie sont à 0 ou moins
        printf("Vous êtes mort, votre aventure se termine ici. Les légendes ne parleront jamais de vous.\n");
        exit(0); // Termine le programme
    }
    if (joueur->or >= 1000) { // Si le joueur a plus de 1000 pièces d'or
        printf("Vous êtes riche ! Votre péril touche enfin à sa fin et vos poches sont remplies d'or.\n");
        exit(0); // Termine le programme
    }
    if (joueur->or <= 0) { // Si le joueur n'a plus d'or
        printf("Votre bourse est désespérément vide ! Votre estomac crie famine, et vous perdez 5 points de vie et d'endurance par tour.\n");
        joueur->endurance -= 5; // Réduit l'endurance de 5
        joueur->pv -= 5; // Réduit les points de vie de 5
    }
    if (joueur->endurance <= 0) { // Si l'endurance est à 0 ou moins
        printf("Vous n'avez plus d'endurance ! Vous dépérissez à vue d'œil et perdez 5 points de vie par tour.\n");
        joueur->pv -= 5; // Réduit les points de vie de 5
    }
}

// Fonction pour lire et exécuter une quête depuis un fichier
void executer_quete(struct player *joueur, FILE *fichier) {
    char ligne[BUF_SIZE]; // Buffer pour stocker une ligne lue depuis le fichier
    char texte_quete[BUF_SIZE * 10] = ""; // Buffer pour stocker le texte de la quête
    char choix[3][BUF_SIZE]; // Tableau pour stocker les 3 choix possibles
    char consequence[3][BUF_SIZE]; // Tableau pour stocker les 3 conséquences possibles

    // Lire le texte de la quête
    while (fgets(ligne, BUF_SIZE, fichier) && strstr(ligne, "===") == NULL) { // Lit jusqu'à trouver "==="
        strcat(texte_quete, ligne); // Concatène la ligne lue au texte de la quête
    }

    // Lire les 3 choix
    for (int i = 0; i < 3; i++) { // Boucle pour lire les 3 choix
        if (!fgets(choix[i], BUF_SIZE, fichier)) { // Si la lecture échoue
            printf("Erreur de lecture des choix.\n");
            return; // Quitte la fonction
        }
        noretour(choix[i]); // Supprime le retour à la ligne du choix
    }

    // Lire les 3 conséquences
    for (int i = 0; i < 3; i++) { // Boucle pour lire les 3 conséquences
        if (!fgets(consequence[i], BUF_SIZE, fichier)) { // Si la lecture échoue
            printf("Erreur de lecture des conséquences.\n");
            return; // Quitte la fonction
        }
        noretour(consequence[i]); // Supprime le retour à la ligne de la conséquence
    }

    // Afficher la quête et les choix
    printf("\n%s\n", texte_quete); // Affiche le texte de la quête
    for (int i = 0; i < 3; i++) { // Boucle pour afficher les 3 choix
        printf("%d. %s\n", i + 1, choix[i]); // Affiche chaque choix avec un numéro
    }

    // Gérer le choix du joueur
    int choix_joueur;
    do {
        printf("\nVotre choix : "); // Demande au joueur de faire un choix
        if (scanf("%d", &choix_joueur) != 1) { // Si l'entrée n'est pas un nombre
            printf("\nEntrée invalide\n");
            while (getchar() != '\n'); // Vide le buffer d'entrée
            continue; // Recommence la boucle
        }

        if (choix_joueur < 1 || choix_joueur > 3) { // Si le choix est invalide
            printf("\nChoix invalide, essayez encore.\n");
        }
    } while (choix_joueur < 1 || choix_joueur > 3); // Répète jusqu'à ce que le choix soit valide

    // Appliquer les conséquences du choix
    char *desc = strtok(consequence[choix_joueur - 1], "|"); // Récupère la description de la conséquence
    printf("\n%s\n", desc); // Affiche la description de la conséquence

    // Appliquer les modifications des stats
    char *modif;
    while ((modif = strtok(NULL, "|")) != NULL) { // Parcourt les modifications de stats
        if (strstr(modif, "pv+") != NULL) { // Si la modification concerne les points de vie (augmentation)
            joueur->pv += atoi(modif + 3); // Ajoute les points de vie (exemple : "pv+10")
        } else if (strstr(modif, "pv-") != NULL) { // Si la modification concerne les points de vie (réduction)
            joueur->pv -= atoi(modif + 3); // Retire les points de vie (exemple : "pv-15")
        } else if (strstr(modif, "endurance+") != NULL) { // Si la modification concerne l'endurance (augmentation)
            joueur->endurance += atoi(modif + 10); // Ajoute de l'endurance (exemple : "endurance+20")
        } else if (strstr(modif, "endurance-") != NULL) { // Si la modification concerne l'endurance (réduction)
            joueur->endurance -= atoi(modif + 10); // Retire de l'endurance (exemple : "endurance-15")
        } else if (strstr(modif, "or+") != NULL) { // Si la modification concerne l'or (augmentation)
            joueur->or += atoi(modif + 3); // Ajoute de l'or (exemple : "or+100")
        } else if (strstr(modif, "or-") != NULL) { // Si la modification concerne l'or (réduction)
            joueur->or -= atoi(modif + 3); // Retire de l'or (exemple : "or-15")
        } else if (strstr(modif, "event+") != NULL) { // Si la modification concerne l'événement (augmentation)
            joueur->event += atoi(modif + 6); // Ajoute à l'événement (exemple : "event+1")
        }
    }

    // Afficher les stats mises à jour
    stats1(joueur); // Affiche les nouvelles stats du joueur
    malusbonus(joueur); // Applique les malus/bonus en fonction des nouvelles stats
}

int main() {
    struct player joueur; // Crée une instance de la structure player
    joueur.pv = 100; // Initialise les points de vie à 100
    joueur.endurance = 100; // Initialise l'endurance à 100
    joueur.or = 100; // Initialise l'or à 100
    joueur.event = 0; // Initialise l'événement à 0

    printf("\n|---------------------------------------------|");
    printf("\n|--------------------RPG Pouet----------------|\n\n");

    // Demande au joueur s'il veut charger sa sauvegarde
    printf("Charger une sauvegarde ? (o/n) : ");
    char choix;
    scanf(" %c", &choix); // Lit le choix du joueur
    getchar(); // Consomme le retour à la ligne laissé par scanf
    if (choix == 'o' || choix == 'O') { // Si le joueur choisit de charger
        charger(&joueur); // Charge la sauvegarde
    } else {
        printf("\nAventurier, quel est ton nom ?\n\n");
        fgets(joueur.nomjoueur, sizeof(joueur.nomjoueur), stdin); // Lit le nom du joueur
        noretour(joueur.nomjoueur); // Supprime le retour à la ligne du nom
    }

    printf("\nBienvenue aventurier %s\n\n", joueur.nomjoueur); // Affiche un message de bienvenue
    stats1(&joueur); // Affiche les stats initiales du joueur

    // Ouvrir le fichier des quêtes
    FILE *fichier_quetes = fopen("quetes.txt", "r"); // Ouvre le fichier des quêtes en mode lecture
    if (!fichier_quetes) { // Si le fichier n'a pas pu être ouvert
        perror("Erreur d'ouverture du fichier quetes.txt"); // Affiche un message d'erreur
        return 1; // Quitte le programme avec un code d'erreur
    }

    // Exécuter les quêtes
    while (!feof(fichier_quetes)) { // Tant qu'on n'a pas atteint la fin du fichier
        executer_quete(&joueur, fichier_quetes); // Exécute une quête
    }

    fclose(fichier_quetes); // Ferme le fichier des quêtes

    // Demande au joueur s'il veut sauvegarder sa partie
    printf("\nSauvegarder la progression ? (o/n) : ");
    fflush(stdout); // Assure que la sortie est bien affichée
    scanf(" %c", &choix); // Lit le choix du joueur
    if (choix == 'o' || choix == 'O') { // Si le joueur choisit de sauvegarder
        sauvegarder(&joueur); // Sauvegarde la progression
    }

    return 0; // Termine le programme avec succès
}



