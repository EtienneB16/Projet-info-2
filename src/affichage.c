#include "headers/lib.h"
#include "headers/affichage.h"
#include "headers/partie.h"

void afficher_menu(BITMAP* bg, BITMAP* page){
    stretch_blit(bg, page , 0, 0, bg->w, bg->h, 0, 0, LARGEUR, HAUTEUR);
    rectfill(page, 300, 150, 500, 200, makecol(255, 255, 255));
    rectfill(page, 300, 225, 500, 275, makecol(255, 255, 255));
    rectfill(page, 300, 300, 500, 350, makecol(255, 255, 255));
    rectfill(page, 300, 375, 500, 425, makecol(255, 255, 255));
    textout_centre_ex(page, font, "Jouer", 400, 175, makecol(0, 0, 0), -1);
    textout_centre_ex(page, font, "Reprendre une partie", 400, 250, makecol(0, 0,0), -1);
    textout_centre_ex(page, font, "Régles", 400, 325, makecol(0, 0,0), -1);
    textout_centre_ex(page, font, "Quitter", 400, 400, makecol(0, 0,0), -1);
    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    show_mouse(screen);
}

