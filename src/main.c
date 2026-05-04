#include "headers/lib.h"
#include "headers/jeu.h"
#include "headers/partie.h"
#include "headers/saisie.h"
#include "headers/affichage.h"

int main() {
    initialisation_Allegro();

    BITMAP* page = create_bitmap(LARGEUR, HAUTEUR);
    clear_bitmap(page);
    BITMAP* bg = NULL;
    BITMAP* img_yoda[NIMAGE_YODA];
    BITMAP* img_vador[NIMAGE_VADOR];
    BITMAP* img_vaisseau[NIMAGE_VAISSEAU];
    initialisation_yoda(img_yoda);
    initialisation_vador(img_vador);
    initialisation_vaisseau(img_vaisseau);

    Joueur joueur;
    Yoda yoda;
    Vador vador;
    Vaisseau vaisseau;

    while (!fermer) {
        switch(gestion_menu(bg, page)){
            case 1:
                initialisation_partie(joueur, yoda, vador, vaisseau);
                break;
            case 2:
                chargement_partie();
                break;
            case 3:
                afficher_regles(bg, page);
                break;
            case 0:
                fermer = 1;
                break;
            default:
                break;
        }
    }

    allegro_exit();
    return 0;
}END_OF_MAIN();
