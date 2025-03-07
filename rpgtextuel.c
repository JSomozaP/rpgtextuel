#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 255  //taille du buffer pour la lecture de chaînes de caractères

//structure representant le joueur 1 et ses stats
struct player {
    int pv;
    int endurance;
    char nomjoueur[20];
    int or;
    int event;
};

//fonction pour supprimer le retour à la ligne d'une chaîne de caractères quand obtenu avec fgets
void noretour (char *str) {
        size_t len=strlen(str);
        if(len>0 && str[len - 1]== '\n'){
                str[len - 1]= '\0';
        }
    }

    //fonction pour suvegarder la progression du joueur dans un fichier
    void sauvegarder(struct player *joueur) {
        FILE *save = fopen("saverpg", "w"); //ouvre le fichier en mode ecriture
        if (save) {
            fprintf (save, "%s %d %d %d %d", joueur->nomjoueur, joueur->pv, joueur->endurance, joueur->or, joueur->event);
            fclose(save);
            printf("\nTon périple est gravé dans la pierre mémorielle\n\n");
        }
    }

    //fonction pour charger une sauvegarde si elle existe
    void charger (struct player *joueur) {
        FILE *save =fopen ("saverpg", "r"); //ouvre le fichier en mode lecture
        if (save) {
            fscanf (save,"%s %d %d %d %d", joueur->nomjoueur, &joueur->pv, &joueur->endurance, &joueur->or, &joueur->event);
            fclose(save);
            printf("\nMatérialisation de ton périple par pierre mémorielle\n");
        }
    }

    //fonction pour afficher les stats du joueur1
    void stats1(struct player *joueur){
        printf("\n---Statistique de %s ---\n",joueur->nomjoueur);
        printf("PV: %d\nEndurance: %d\nOr: %d\nEvent: %d\n", joueur->pv, joueur->endurance, joueur->or, joueur->event);
    }

    // Fonction pour gérer un événement interactif
void evenement(struct player *joueur) {
    printf("\nVous arrivez à l'orée d'une forêt mystérieuse, trois chemins s'offrent à vous. Que faites-vous ?\n");
    printf("1. Prendre le sentier de gauche.\n");
    printf("2. Avancer tout droit.\n");
    printf("3. Prendre le sentier de droite.\n");
    
    int choix;
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            printf("\nVous vous enfoncez dans les bois et trouvez un coffre abandonné.\n");
            printf("Vous obtenez 10 pièces d'or !\n");
            joueur->or += 10;
            break;
        case 2:
            printf("\nUn loup surgit et vous attaque ! Vous perdez 15 PV.\n");
            joueur->pv -= 15;
            break;
        case 3:
            printf("\nVous tombez dans un piège et perdez 10 points d'endurance.\n");
            joueur->endurance -= 10;
            break;
        default:
            printf("\nChoix invalide, rien ne se passe.\n");
            break;
    }
    stats1(joueur);
}

// Fonction pour enchaîner plusieurs événements
void suite_evenements(struct player *joueur) {
    printf("\nVous arrivez devant une auberge.\n");
    printf("Que faites-vous ?\n");
    printf("1. Entrer et prendre un repas.\n");
    printf("2. Continuer votre route.\n");
    printf("3. Voler un client distrait.\n");
    
    int choix;
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            printf("\nLe repas vous redonne des forces, mais coûte 5 pièces d'or.\n");
            joueur->pv += 5;
            joueur->or -= 5;
            break;
        case 2:
            printf("\nVous continuez votre chemin sans encombre.\n");
            break;
        case 3:
            printf("\nVous volez 10 pièces d'or mais le propriétaire vous poursuit et vous lance une pierre, vous perdez 5pv.\n");
            joueur->or += 10;
            joueur->pv -= 5;
            break;
        default:
            printf("\nChoix invalide, rien ne se passe.\n");
            break;
    }
    stats1(joueur);
}

    int main(){

        struct player joueur;
        joueur.pv=100;  //valeur initiale des points de vie
        joueur.endurance=100; //valeur initiale de l'endurance
        joueur.or=100; //valeur initiale de l'or
        joueur.event=0; //valeur initiale des évènements
        
        printf("\n|---------------------------------------------|");
        printf("\n|--------------------RPG Pouet----------------|\n\n");

        //Demande au joueur s'il veut charger sa sauvegarde
        printf("Charger une sauvegarde ? (o/n) : ");
        char choix;
        scanf("%c", &choix);
        getchar(); //consomme le retour à la ligne laissé par scanf

        if (choix == 'o' || choix == 'O') {
            charger(&joueur); //charge la progression
        }
        else {
            printf("\nAventurier, quel est ton nom ?\n\n");
            fgets( joueur.nomjoueur, sizeof(joueur.nomjoueur), stdin ); //récupère le nom du joueur 1
            noretour(joueur.nomjoueur); //supprime le retour à la ligne
            
        }
        printf("\nBienvenue aventurier %s\n\n", joueur.nomjoueur);
        stats1(&joueur); //affiche stats du joueur1

        evenement(&joueur);
        suite_evenements(&joueur);

        //demande au joueur s'il veut sauvegarder sa partie
        printf("\nsauvegarder la progression ? (o/n) : \n\n");
        scanf("%c",&choix);
        if (choix == 'o' || choix =='O') {
            sauvegarder(&joueur); //sauvegarde la progression
        }
    }