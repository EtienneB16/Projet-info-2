#include <headers/lib.h>
#include <headers/partie.h>
#include <headers/affichage.h>

int main() {
    initialisation_Allegro();
    afficherMenu();
    while (!fermer) {
        if (key[KEY_ESC]) {
            fermeture();
        }
    }
    allegro_exit();
    return 0;
}