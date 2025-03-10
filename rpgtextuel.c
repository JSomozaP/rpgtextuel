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
        printf("\n---------Statistiques de %s---------\n",joueur->nomjoueur);
        printf("PV: %d\nEndurance: %d\nOr: %d\nEvent: %d\n", joueur->pv, joueur->endurance, joueur->or, joueur->event);
        printf("-------------------------------------\n");
    }

    // Fonction pour gérer un événement interactif
    //quète numéro 1
void evenement(struct player *joueur) {
    printf("\nVous vous reveillez au beau milieu d'une forêt, en sous-vêtements une bourse contenant 100 pièces d'or accroché à votre taille, sans aucun souvenirs, trois chemins s'offrent à vous. Que faites-vous ?\n");
    printf("1. Prendre le sentier de gauche nimbé de ténèbre qui s'enfonce dans la forêt.\n");
    printf("2. Avancer sur le sentier en face de vous, le chemin semble dégagé et sur.\n");
    printf("3. Prendre le sentier de droite où un épais brouillard est présent.\n");
    
    int choix;
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            printf("\nVous vous enfoncez dans les bois et trouvez un coffre abandonné. Vous obtenez un sous-vêtement blindé et de l'or\n");
            printf("Vous obtenez 100 pièces d'or, c'est beaucoup !\n");
            joueur->or += 100;
            joueur->event+=1;
            break;
        case 2:
            printf("\nUn gorille albinos parlant se dresse devant vous. Il se nomme Brandon et affirme qu'il doit vous tester physiquement ! Vous perdez 15 PV, ce fut violent.\n");
            joueur->pv -= 15;
            joueur->event+=1;
            break;
        case 3:
            printf("\nVous trébuchez lamentablement après vous être cogné le petit orteil sur une souche, vous perdez 15 points d'endurance.\n");
            joueur->endurance -= 15;
            joueur->event+=1;
            break;
        default:
            printf("\nChoix invalide, rien ne se passe.\n");
            break;
    }
    stats1(joueur);
}

// Fonction pour enchaîner plusieurs événements
//quète numero 2
void suite_evenements(struct player *joueur) {
    printf("\nTant bien que mal, vous arrivez devant une auberge perdu au milieu des bois.\n");
    printf("Que faites-vous ?\n");
    printf("1. Entrer pour prendre un repas et glaner quelques informations pour enfin comprendre où vous êtes.\n");
    printf("2. Vous ne tentez pas le diable et decidez de continuer votre route. Cette auberge est trop louche.\n");
    printf("3. Vous apercevez un client éméché affalé au sol, et vous décidez de lui faire les poches pour trouver un objet utile.\n");
    
    int choix;
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            printf("\nLe repas vous réchauffe et vous redonne des forces (+5pv), mais coûte 5 pièces d'or. Le tavernier, ne comprenant pas pourquoi vous êtes en slip décide de vous facturer 10 pièces d'or supplémentaires pour la gène visuelle occasionnée\n");
            joueur->pv += 5;
            joueur->or -= 15;
            joueur->event+=1;
            break;
        case 2:
            printf("\nVous continuez votre chemin en remarquant que vous êtes suivis par un groupe de personnes encapuchonnés, vous vous enfuyez de toutes vos forces et perdez 20 pt d'endurance.\n");
            joueur->endurance -=20;
            joueur->event+=1;
            break;
        case 3:
            printf("\nVous parvenez à lui dérober une belle bourse de 150 pièces d'or, c'est incroyable ! Malheureusement le pochtron se reveil et vous saute dessus. Vous parvenez finalement à le neutraliser, mais vous perdez 30 pv dans la bagarre.\n");
            joueur->or += 150;
            joueur->pv -= 30;
            joueur->event+=1;
            break;
        default:
            printf("\nChoix invalide, rien ne se passe.\n");
            break;
    }
    stats1(joueur);

    //quète numéro 3
    printf("\nVous finissez par arriver devant un autel étrange, où un vieil homme luisant à la peau mauve tout aussi étrange s'adresse à vous.\n");
    printf("J'ai remarqué ton slip majestueux, et j'ai décidé de t'accorder trois souhaits !\n");
    printf("1. Vous décidez de lui demander la santé, car la santé c'est la vie.\n");
    printf("2. Vous décidez de lui demander d'être incroyablement endurant pour pouvoir affronter sereinement les obstacles sur votre route\n");
    printf("3. Vous décidez de lui demander de l'argent, car c'est bien connu, l'argent ne fait pas le bonheur mais y contribu largement.\n");
    
    
    printf("\nVotre choix : ");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            printf("\nLe vieux se met à tourner rapidement sur lui même en hurlant, vous vous sentez très mal, et perdez 70 points de vie !\n");
            printf("La prochaine fois, tu ne fera pas confiance à la première personne que tu croise, merci pour ton énérgie vitale !\n");
            printf("Le vieux disparait dans une ricanement infernal.\n");
            joueur->pv -= 70;
            joueur->event+=1;
            break;
        case 2:
            printf("\nLe vieux s'avance vers vous et vous met un énorme coup de genou dans les parties. Vous tombez au sol et perdez 50 points d'endurance\n");
            printf("Voilà, grâce à moi, tu souffrira moins la prochaine fois qu'on te frappera à cet endroit, tu es devenu plus endurant !\n");
            printf("il disparait en ricanant\n");
            joueur->endurance -=50;
            joueur->event+=1;
            break;
        case 3:
            printf("\nLe vieux s'avance vers vous, subtilise l'entiéreté de votre bourse, puis vous redonne 10 pièces d'or\n");
            printf("Voilà, maintenant tu as gagné 10 pièces d'or, et tu as plus d'or que quelqu'un qui n'en a pas, tu es plus heureux ?\n");
            printf("Le vieux disparait en ricanant\n");
            joueur->or == 0;
            joueur->or += 10;
            joueur->event+=1;
            break;
        default:
            printf("\nChoix invalide, rien ne se passe.\n");
            break;
        }
        stats1(joueur);
}

/*void suite_evenements(struct player *joueur) {
}*/

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
        getchar();
        if (choix == 'o' || choix =='O') {
            sauvegarder(&joueur); //sauvegarde la progression
        }
        return 0;
    }