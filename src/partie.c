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

BITMAP* charger_bg(int i){
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

void initialisation_partie(Joueur joueur, Yoda yoda, Vador vador, Vaisseau vaisseau){
    joueur.score = 0;
    joueur.niveau =1;
    strcpy(joueur.nom, pseudo());

    yoda.tx = 0;//POUR CHAQUE STRUCT, IL FAUT ENTRER LES TAILLES DES IMAGES (trouvables en bas a droite dans les .bmp)
    yoda.ty = 0;
    yoda.x = LARGEUR-yoda.tx/2;
    yoda.y = HAUTEUR-yoda.ty;
    yoda.dx = 0;
    yoda.vitesse = 5;
    yoda.imgcourante = 0;
    yoda.cptimg = 0;
    yoda.tmpimg = 5;

    vador.tx = 0;
    vador.ty =0;
    vador.x = LARGEUR-vador.tx/2;
    vador.y = HAUTEUR-vador.ty;
    vador.dx = 0;
    vador.vitesse = 5;
    vador.taille = 3;

    vaisseau.tx = 0;
    vaisseau.ty = 0;
    vaisseau.x = LARGEUR-vaisseau.tx/2;
    vaisseau.y = HAUTEUR-vaisseau.ty;
    vaisseau.dx = 0;
    vaisseau.vitesse = 5;
    vaisseau.taille = 3;
}
