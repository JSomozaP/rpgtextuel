#include <stdio.h>  // Inclut la bibliothèque standard d'entrée/sortie
#include <stdlib.h> // Inclut la bibliothèque standard pour diverses fonctions
#include <string.h> // Inclut la bibliothèque de manipulation de chaînes
#include <ctype.h>  // Inclut la bibliothèque pour la fonction isdigit

#define BUF_SIZE 255 // Taille du buffer pour la lecture des chaînes

// Structure représentant le joueur
struct player {
    int pv;             // Points de vie
    int endurance;      // Endurance
    char nomjoueur[20]; // Nom du joueur
    int or;             // Or possédé
    int event;          // Événement en cours
};

// Fonction pour supprimer le retour à la ligne d'une chaîne
void noretour(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Fonction pour sauvegarder la progression du joueur
void sauvegarder(struct player *joueur) {
    FILE *save = fopen("saverpg", "w");
    if (save) {
        fprintf(save, "%s %d %d %d %d\n", joueur->nomjoueur, joueur->pv, joueur->endurance, joueur->or, joueur->event);
        fclose(save);
        printf("\nTon périple est gravé dans la pierre mémorielle\n\n");
    } else {
        perror("Erreur d'ouverture du fichier");
        printf("\nImpossible de sauvegarder la partie\n\n");
    }
}

// Fonction pour charger une sauvegarde
void charger(struct player *joueur) {
    FILE *save = fopen("saverpg", "r");
    if (save) {
        fscanf(save, "%s %d %d %d %d", joueur->nomjoueur, &joueur->pv, &joueur->endurance, &joueur->or, &joueur->event);
        fclose(save);
        printf("\nMatérialisation de ton périple par pierre mémorielle\n");
    }
}

// Fonction pour afficher les statistiques du joueur
void stats1(struct player *joueur) {
    printf("\n---------Statistiques de %s---------\n", joueur->nomjoueur);
    printf("PV: %d\nEndurance: %d\nOr: %d\nEvent: %d\n", joueur->pv, joueur->endurance, joueur->or, joueur->event);
    printf("-------------------------------------\n");
}

// Fonction pour appliquer les malus/bonus en fonction des stats du joueur
void malusbonus(struct player *joueur) {
    if (joueur->pv <= 0) {
        printf("Vous êtes mort, votre aventure se termine ici. Les légendes ne parleront jamais de vous.\n");
        exit(0);
    }
    if (joueur->or >= 1000) {
        printf("Vous êtes riche ! Votre péril touche enfin à sa fin et vos poches sont remplies d'or.\n");
        exit(0);
    }
    if (joueur->or <= 0) {
        printf("Votre bourse est désespérément vide ! Votre estomac cri famine, et vous perdez 5 points de vie et d'endurance par tour.\n");
        joueur->endurance -= 5;
        joueur->pv -= 5;
    }
    if (joueur->endurance <= 0) {
        printf("Vous n'avez plus d'endurance ! Vous dépérissez à vue d'œil et perdez 5 points de vie par tour.\n");
        joueur->pv -= 5;
    }
}

// Fonction pour lire et exécuter une quête depuis un fichier
void executer_quete(struct player *joueur, FILE *fichier) {
    char ligne[BUF_SIZE];
    char texte_quete[BUF_SIZE * 10] = "";
    char choix[3][BUF_SIZE * 5] = {"", "", ""}; // Augmenter la taille du buffer pour les choix
    char consequence[3][BUF_SIZE * 5] = {"", "", ""};
    int i = 0;
    int choix_joueur = -1;

    // Lecture du texte de la quête
    if (fgets(ligne, BUF_SIZE, fichier) == NULL) {
        return; // Fin du fichier
    }
    noretour(ligne);
    strcpy(texte_quete, ligne);

    // Lecture des choix et conséquences
    for (i = 0; i < 3; i++) {
        if (fgets(ligne, BUF_SIZE, fichier) == NULL) {
            fprintf(stderr, "Erreur de lecture des choix\n");
            return;
        }
        noretour(ligne);
        char *sep = strchr(ligne, '|');
        if (sep != NULL) {
            *sep = '\0';
            strcpy(choix[i], ligne);
            strcpy(consequence[i], sep + 1);
        } else {
            fprintf(stderr, "Erreur de format du fichier de quêtes\n");
            return;
        }
    }

    // Affichage de la quête
    printf("\n%s\n", texte_quete);

    // Affichage des choix
    for (i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, choix[i]);
    }

    // Demande du choix au joueur
    do {
        printf("Votre choix (1-3): ");
        if (scanf("%d", &choix_joueur) != 1) {
            fprintf(stderr, "Erreur : Entrée invalide.\n");
            while (getchar() != '\n'); // Nettoyer le buffer d'entrée
            choix_joueur = -1;
        } else if (choix_joueur < 1 || choix_joueur > 3) {
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
        }
    } while (choix_joueur < 1 || choix_joueur > 3);

    // Traitement de la conséquence
    char consequence_buffer[BUF_SIZE * 5];
    strncpy(consequence_buffer, consequence[choix_joueur - 1], BUF_SIZE * 5 - 1);
    consequence_buffer[BUF_SIZE * 5 - 1] = '\0';

    // Sépare le texte des modifications de stats (sans modifier la chaîne originale)
    char *texte = consequence_buffer;
    char *sep2 = strchr(texte, '|');
    if (sep2 != NULL) {
        *sep2 = '\0'; // Temporairement terminer la chaîne pour afficher le texte
        printf("\n%s\n", texte);

        // Application des modifications de stats
        char consequence_copy[BUF_SIZE * 5];
        strncpy(consequence_copy, consequence[choix_joueur - 1], BUF_SIZE * 5 - 1);
        consequence_copy[BUF_SIZE * 5 - 1] = '\0';
        char *token = consequence_copy;
        while ((token = strchr(token, '|'))) {
            token++; // Avance après le '|'
            if (strncmp(token, "pv+", 3) == 0) joueur->pv += atoi(token + 3);
            else if (strncmp(token, "pv-", 3) == 0) joueur->pv -= atoi(token + 3);
            else if (strncmp(token, "endurance+", 10) == 0) joueur->endurance += atoi(token + 10);
            else if (strncmp(token, "endurance-", 10) == 0) joueur->endurance -= atoi(token + 10);
            else if (strncmp(token, "or+", 3) == 0) joueur->or += atoi(token + 3);
            else if (strncmp(token, "or-", 3) == 0) joueur->or -= atoi(token + 3);
            else if (strncmp(token, "event+", 6) == 0) joueur->event += atoi(token + 6);
        }
    } else {
        printf("\n%s\n", texte); // Afficher toute la ligne s'il n'y a pas de séparateur
    }

    // Afficher les statistiques mises à jour
    stats1(joueur);
    malusbonus(joueur);
}

int main() {
    struct player joueur;
    joueur.pv = 100;
    joueur.endurance = 100;
    joueur.or = 100;
    joueur.event = 0;

    printf("\n|---------------------------------------------|");
    printf("\n|--------------------RPG Pouet----------------|\n\n");

    // Demande au joueur s'il veut charger sa sauvegarde
    printf("Charger une sauvegarde ? (o/n) : ");
    char choix;
    scanf(" %c", &choix);
    getchar(); // Consomme le retour à la ligne laissé par scanf
    if (choix == 'o' || choix == 'O') {
        charger(&joueur);
    } else {
        printf("\nAventurier, quel est ton nom ?\n\n");
        fgets(joueur.nomjoueur, sizeof(joueur.nomjoueur), stdin);
        noretour(joueur.nomjoueur);
    }

    printf("\nBienvenue aventurier %s\n\n", joueur.nomjoueur);
    stats1(&joueur);

    // Ouvrir le fichier des quêtes
    FILE *fichier_quetes = fopen("quetes.txt", "r");
    if (!fichier_quetes) {
        perror("Erreur d'ouverture du fichier quetes.txt");
        return 1;
    }

    // Exécuter les quêtes
    char ligne[BUF_SIZE];
    while (fgets(ligne, BUF_SIZE, fichier_quetes) != NULL) {
        if (strncmp(ligne, "===", 3) == 0) {
            executer_quete(&joueur, fichier_quetes);
        }
    }

    fclose(fichier_quetes);

    // Demande au joueur s'il veut sauvegarder sa partie
    printf("\nSauvegarder la progression ? (o/n) : ");
    fflush(stdout);
    scanf(" %c", &choix);
    if (choix == 'o' || choix == 'O') {
        sauvegarder(&joueur);
    }

    return 0;
}