#include "headers/lib.h"
#include "headers/partie.h"

void fermeture() {
    fermer = 1;
}

void initialisation_Allegro(){
    allegro_init();
    install_keyboard();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,LARGEUR,HAUTEUR,0,0)!=0){
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    set_close_button_callback(fermeture);
}

BITMAP* initialisation_bg(int i){
    char bg[255];
    sprintf(bg, "src/backgrounds/bg_%d.bmp",i);
    BITMAP* p = load_bitmap(bg, NULL);
    if(!p) {
        allegro_message("impossible de charger %s", bg);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return p;
}

void initialisation_yoda(BITMAP *img[NIMAGE]){
    for (int i=0;i<NIMAGE;i++){
        char yoda[40];
        sprintf(yoda, "src/personnages/yoda/run_%d.bmp",i);
        img[i] = load_bitmap(yoda,NULL);
        if (!img[i]){
            allegro_message("pas pu trouver %s",yoda);
            exit(EXIT_FAILURE);
        }
    }
}

void initialisation_vador(BITMAP *img[NIMAGE]){
    for (int i=0;i<NIMAGE;i++){
        char vador[40];
        sprintf(vador, "personnages/vador/vador_%d.bmp",i);
        img[i] = load_bitmap(vador,NULL);
        if (!img[i]){
            allegro_message("pas pu trouver %s",vador);
            exit(EXIT_FAILURE);
        }
    }
}

void initialisation_vaisseau(BITMAP *img[NIMAGE]){
    for (int i=0;i<NIMAGE;i++){
        char vaisseau[40];
        sprintf(vaisseau, "personnages/vaisseau/vaisseau_%d.bmp",i);
        img[i] = load_bitmap(vaisseau,NULL);
        if (!img[i]){
            allegro_message("pas pu trouver %s",vaisseau);
            exit(EXIT_FAILURE);
        }
    }
}