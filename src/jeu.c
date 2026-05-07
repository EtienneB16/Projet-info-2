
#include "headers/lib.h"
#include "headers/jeu.h"

void boucle_de_jeu(BITMAP* menu, BITMAP* page, BITMAP* yoda[]) {
    int x,y;
    int dx;
    int tx,ty;
    int vitesse = 5;

    int imgcourante=0;
    int cptimg=0, tmpimg=5;

    int doucement = 10;

    tx = yoda[0]->w;
    ty = yoda[0]->h;
    x = 0;
    y = HAUTEUR-ty;
    dx = 0;
    
    while (!fermer) {
        stretch_blit(menu,page,0,0,menu->w,menu->h,0,0,LARGEUR,HAUTEUR);
        if (key[KEY_ESC]) {
            fermer = 1;
        }
        if (key[KEY_RIGHT] && x+tx<LARGEUR) {
            x+=vitesse;
            dx = 1;
            cptimg++;
            if (cptimg>=tmpimg){
                cptimg=0;
                imgcourante++;
                if (imgcourante>=NIMAGE)imgcourante=0;
            }

        } 
        if (key[KEY_LEFT] && x>0) {
            x-=vitesse;
            dx = -1;
            cptimg++;
            if (cptimg>=tmpimg){
                cptimg=0;
                imgcourante++;
                if (imgcourante>=NIMAGE)imgcourante=0;
            }
        }
        if (dx>=0)
        //blit(img[imgcourante],page,0,0,x,y,img[imgcourante]->w,img[imgcourante]->h);
           draw_sprite(page,yoda[imgcourante],x,y);
        else draw_sprite_h_flip(page,yoda[imgcourante],x,y);

        blit(page,screen,0,0,0,0,LARGEUR,HAUTEUR);

        rest(doucement);
    }
}
