#ifndef LIB_H
#define LIB_H

#define LARGEUR 800
#define HAUTEUR 600
#define NIMAGE_YODA 1
#define NIMAGE_VADOR 1
#define NIMAGE_VAISSEAU 1
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>

volatile int fermer;

typedef struct {
    int x;
    int y;
    int dx;
    int tx;
    int ty;
    int vitesse;
    int imgcourante;
    int cptimg;
    int tmpimg;
}Yoda;

typedef struct {
    int x;
    int y;
    int dx;
    int tx;
    int ty;
    int vitesse;
    int taille;
}Vaisseau;

typedef struct {
    int x;
    int y;
    int dx;
    int tx;
    int ty;
    int vitesse;
    int taille;
}Vador;

typedef struct {
    int score;
    int niveau;
    char nom [20];
}Joueur;

#endif
