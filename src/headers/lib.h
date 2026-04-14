#ifndef LIB_H
#define LIB_H

#define LARGEUR 800
#define HAUTEUR 600
#define NIMAGE 6

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>

volatile int fermer;

typedef struct {
    int x;
    int y;
    int vies;
    int score;
}Joueur;

#endif