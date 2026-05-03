#include "headers/lib.h"
#include "headers/affichage.h"
#include "headers/partie.h"

void afficherMenu(BITMAP* menu){
    stretch_blit(menu, screen, 0, 0, menu->w, menu->h, 0, 0, LARGEUR, HAUTEUR);
}

// typedef struct {
    int x;
    int y;
    int vies;
    int score;
    char pseudo[30];
    BITMAP *sprite;
} Joueur;

int initialiserAffichageGalactique();
void fermerAffichage();
void AfficherMenu();
void AfficherInterfaceJeu(Joueur j1, Joueur j2);
void afficherJoueur(Joueur j1, Joueur j2);  //

#define LARGEUR 1280
#define HAUTEUR 720

BITMAP *buffer = NULL;
BITMAP *fond = NULL;

int initialiserAffichageGalactique() {
    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(32);

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGEUR, HAUTEUR, 0, 0) != 0) {
        allegro_message("Erreur mode graphique : %s", allegro_error);
        return 0;
    }

    buffer = create_bitmap(LARGEUR, HAUTEUR);

    if (buffer == NULL) {
        allegro_message("Erreur creation buffer");
        return 0;
    }

    fond = load_bitmap("images/fond.bmp", NULL);

    return 1;
}

void fermerAffichage() {
    if (buffer != NULL) {
        destroy_bitmap(buffer);
    }

    if (fond != NULL) {
        destroy_bitmap(fond);
    }

    allegro_exit();
}

void AfficherMenu() {
    clear_to_color(buffer, makecol(0, 0, 0));

    textout_centre_ex(buffer, font, "CELESTE", LARGEUR / 2, 120, makecol(255, 255, 0), -1);

    textout_centre_ex(buffer, font, "1 - Nouvelle partie", LARGEUR / 2, 250, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "2 - Reprendre une partie", LARGEUR / 2, 300, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "3 - Regles", LARGEUR / 2, 350, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "4 - Quitter", LARGEUR / 2, 400, makecol(255, 255, 255), -1);

    blit(buffer, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
}

void AfficherInterfaceJeu(Joueur j1, Joueur j2) {
    clear_to_color(buffer, makecol(0, 0, 0));

    if (fond != NULL) {
        stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, 1000, HAUTEUR);
    } else {
        rectfill(buffer, 0, 0, 1000, HAUTEUR, makecol(10, 10, 30));
    }

    rectfill(buffer, 1000, 0, LARGEUR, HAUTEUR, makecol(25, 25, 25));
    line(buffer, 1000, 0, 1000, HAUTEUR, makecol(255, 255, 255));

    textprintf_ex(buffer, font, 1030, 50, makecol(255, 255, 255), -1,
                  "Joueur 1 : %s", j1.pseudo);

    textprintf_ex(buffer, font, 1030, 80, makecol(255, 255, 255), -1,
                  "Score : %d", j1.score);

    textprintf_ex(buffer, font, 1030, 110, makecol(255, 255, 255), -1,
                  "Vies : %d", j1.vies);

    if (j2.vies > 0) {
        textprintf_ex(buffer, font, 1030, 170, makecol(255, 255, 255), -1,
                      "Joueur 2 : %s", j2.pseudo);

        textprintf_ex(buffer, font, 1030, 200, makecol(255, 255, 255), -1,
                      "Score : %d", j2.score);

        textprintf_ex(buffer, font, 1030, 230, makecol(255, 255, 255), -1,
                      "Vies : %d", j2.vies);
    }
}

void afficherJoueur(Joueur j1, Joueur j2) {
    if (j1.sprite != NULL) {
        draw_sprite(buffer, j1.sprite, j1.x, j1.y);
    } else {
        rectfill(buffer, j1.x, j1.y, j1.x + 40, j1.y + 40, makecol(0, 255, 0));
    }

    if (j2.vies > 0) {
        if (j2.sprite != NULL) {
            draw_sprite(buffer, j2.sprite, j2.x, j2.y);
        } else {
            rectfill(buffer, j2.x, j2.y, j2.x + 40, j2.y + 40, makecol(0, 100, 255));
        }
    }

    blit(buffer, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
