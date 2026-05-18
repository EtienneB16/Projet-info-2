#include "headers/lib.h"
#include "headers/jeu.h"
#include "headers/partie.h"
#include "headers/saisie.h"
#include "headers/affichage.h"

int main() {
    initialisation_Allegro();

    BITMAP* page = create_bitmap(LARGEUR, HAUTEUR);
    clear_bitmap(page);

    /* bg commence a NULL ; gestion_menu verifie NULL avant destroy_bitmap */
    BITMAP* bg = NULL;

    BITMAP* img_yoda[NIMAGE_YODA];
    BITMAP* img_vador[NIMAGE_VADOR];
    BITMAP* img_vaisseau[NIMAGE_VAISSEAU];

    initialisation_yoda(img_yoda);
    /* CORRECTION : les deux lignes etaient commentees mais img_vador et
       img_vaisseau sont utilises dans boucle_de_jeu -> crash garanti. */
    initialisation_vador(img_vador);
    initialisation_vaisseau(img_vaisseau);

    /* CORRECTION : int niv (valeur) + passage par adresse &niv.
       L'original declarait int* niv sans jamais l'allouer ni lui affecter
       une adresse -> dereferencement de pointeur invalide au 1er *niv. */
    int niv;
    Niveau niveau[3];
    Joueur joueur;
    Yoda yoda;
    Vador vador;
    Vaisseau*    tete_vaisseau    = NULL;
    Projectile*  tete_projectile  = NULL;

    /* Precharger les backgrounds pour eviter des acces disque pendant le jeu. */
    charger_bg(0);
    charger_bg(1);
    charger_bg(3);
    charger_bg(4);

    while (!fermer) {
        switch (gestion_menu(bg, page)) {
            case 1:
                initialisation_partie(&niv, niveau, &joueur, &yoda, &vador,
                                      &tete_vaisseau, &tete_projectile);
                boucle_de_jeu(&niv, niveau, &joueur, &yoda, &vador,
                              &tete_vaisseau, &tete_projectile,
                              page, bg,
                              img_yoda, img_vador, img_vaisseau);
                /* Apres une partie, reinitialiser fermer pour pouvoir
                   retourner au menu si l'utilisateur a ferme via ESC
                   (et non via la croix de la fenetre). */
                fermer = 0;
                break;
            case 2:
                /* TODO : chargement_partie() */
                break;
            case 3:
                gestion_menu_regles(page);
                fermer = 0;
                break;
            case 0:
                fermer = 1;
                break;
            default:
                break;
        }
    }

    /* Nettoyage final */
    destroy_bitmap(page);
    liberer_bg_cache();
    for (int i = 0; i < NIMAGE_YODA; i++) destroy_bitmap(img_yoda[i]);
    for (int i = 0; i < NIMAGE_VADOR; i++) destroy_bitmap(img_vador[i]);
    for (int i = 0; i < NIMAGE_VAISSEAU; i++) destroy_bitmap(img_vaisseau[i]);
    if (tete_vaisseau != NULL) free(tete_vaisseau);
    if (tete_projectile != NULL) free(tete_projectile);
    allegro_exit();
    return 0;
}
END_OF_MAIN()

