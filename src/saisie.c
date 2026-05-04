#include "headers/lib.h"
#include "headers/saisie.h"
#include "headers/affichage.h"

int gestion_menu(BITMAP* bg, BITMAP* page){
    bg = charger_bg(0);
    while (!fermer) {
        afficher_menu(bg, page);
        if (key[KEY_ESC]) {
            fermer = 1;
            return 0;
        }
        if (mouse_b & 1) {
            if (mouse_x >=300 && mouse_x <= 500 && mouse_y >=150 && mouse_y <=200){
                fermer = 1;
                return 1;
            }
            if (mouse_x >=300 && mouse_x <= 500 && mouse_y >=225 && mouse_y <=275){
                fermer = 1;
                return 2;
            }
            if (mouse_x >=300 && mouse_x <= 500 && mouse_y >=300 && mouse_y <=350){
                fermer = 1;
                return 3;
            }
            if (mouse_x >=300 && mouse_x <= 500 && mouse_y >=375 && mouse_y <=425){
                fermer = 1;
                return 0;
            }
        }
        rest(100);
    }
    return 0;
}
