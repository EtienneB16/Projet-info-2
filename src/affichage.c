#include "headers/lib.h"
#include "headers/affichage.h"
#include "headers/partie.h"

void afficherMenu(BITMAP* menu){
    stretch_blit(menu, screen, 0, 0, menu->w, menu->h, 0, 0, LARGEUR, HAUTEUR);
}