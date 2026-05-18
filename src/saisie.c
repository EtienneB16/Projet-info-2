
#include "headers/lib.h"
#include "headers/saisie.h"
#include "headers/affichage.h"
#include "headers/partie.h"

int gestion_menu(BITMAP* bg, BITMAP* page) {
    /* CORRECTION : l'original ne verifiait pas si bg est NULL avant de le
       detruire. Au 1er appel (bg=NULL dans main) c'est un comportement
       indefini. destroy_bitmap(NULL) est safe dans Allegro mais on garde
       la garde explicite pour la clarte. */
    if (bg != NULL) destroy_bitmap(bg);
    bg = charger_bg(0);

    int choix_menu = 0;
    while (!choix_menu) {
        afficher_menu(bg, page);
        if (key[KEY_ESC]) {
            choix_menu = 1;
            destroy_bitmap(bg);
            return 0;
        }
        if (mouse_b & 1) {
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 200 && mouse_y <= 315) {
                choix_menu = 1; destroy_bitmap(bg); return 1;
            }
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 295 && mouse_y <= 410) {
                choix_menu = 1; destroy_bitmap(bg); return 2;
            }
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 390 && mouse_y <= 505) {
                choix_menu = 1; destroy_bitmap(bg); return 3;
            }
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 485 && mouse_y <= 590) {
                choix_menu = 1; destroy_bitmap(bg); return 0;
            }
        }
        rest(100);
    }
    destroy_bitmap(bg);
    return 0;
}

void gestion_menu_pause(int* niv, Niveau niveau[], Joueur* joueur, Yoda yoda, Vador vador,
                     Vaisseau* tete_vaisseau, Projectile* tete_projectile,
                     BITMAP* page, BITMAP* bg,
                     BITMAP* img_yoda[], BITMAP* img_vador[],
                     BITMAP* img_vaisseau[]) {
    if(bg != NULL) destroy_bitmap(bg);
    bg = charger_bg(4);
    int choix_menu = 0;
    while (!choix_menu) {
        afficher_niveau(niv, niveau, yoda, vador, tete_vaisseau, tete_projectile,
                         page, bg, img_yoda, img_vador, img_vaisseau);
        afficher_menu_pause(page, bg);
        if (key[KEY_ESC]) {
            choix_menu = 1;
            return;
        }
        if (mouse_b & 1) {
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 200 && mouse_y <= 315) {
                choix_menu = 1; return;
            }
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 295 && mouse_y <= 410) {
                sauvegarde(niv, niveau, joueur);
            }
            if (mouse_x >= 200 && mouse_x <= 990 && mouse_y >= 390 && mouse_y <= 505) {
                choix_menu = 1; fermer = 1; return;
            }
        }
        rest(100);
    }
}

void gestion_menu_regles(BITMAP* page, BITMAP* bg) {
    int choix_menu = 0;
    while (!choix_menu) {
        afficher_regles(page, bg);
        if (key[KEY_ESC]) {
            choix_menu = 1;
            return;
        }
        rest(100);
    }
}

void deplacement_a_droite(Yoda* yoda) {
    yoda->x += yoda->vitesse;
    yoda->dx = 1;
    defiler_img_yoda(yoda);
}

void deplacement_a_gauche(Yoda* yoda) {
    yoda->x -= yoda->vitesse;
    yoda->dx = -1;
    defiler_img_yoda(yoda);
}

/* CORRECTION :
   1. Retournait char -> ne peut pas contenir une chaine. On passe char* out.
   2. key[KEY_ENTER] et key[KEY_BACKSPACE] retournent 1/0 (touche enfoncee),
      pas un code ASCII. Il faut comparer avec '\r' et '\b'.
   3. return *pseudo ne retournait que le 1er caractere. */
void pseudo(char* out) {
    int i = 0;
    out[0] = '\0';

    /* Afficher une invite */
    clear_to_color(screen, makecol(0, 0, 0));
    textout_centre_ex(screen, font, "Entrez votre pseudo :",
                      LARGEUR / 2, HAUTEUR / 2 - 20,
                      makecol(255, 255, 255), -1);

    while (i < 19) {   /* on garde la place pour le '\0' final */
        if (keypressed()) {
            int raw = readkey();
            char c  = raw & 0xFF;   /* octet bas = code ASCII */

            if (c == '\r') {        /* CORRECTION : Entree = '\r', pas key[KEY_ENTER] */
                break;
            }
            if (c == '\b' && i > 0) { /* CORRECTION : Backspace = '\b' */
                i--;
                out[i] = '\0';
            } else if (c >= ' ' && c <= '~') {
                out[i] = c;
                i++;
                out[i] = '\0';
            }
            /* Rafraichir l'affichage du pseudo en cours de saisie */
            rectfill(screen, LARGEUR / 2 - 100, HAUTEUR / 2,
                     LARGEUR / 2 + 100, HAUTEUR / 2 + 16,
                     makecol(0, 0, 0));
            textout_centre_ex(screen, font, out,
                              LARGEUR / 2, HAUTEUR / 2,
                              makecol(255, 255, 0), -1);
        }
        rest(30);
    }
}
