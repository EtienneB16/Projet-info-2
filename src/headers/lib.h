#ifndef LIB_H
#define LIB_H

#define LARGEUR 1200
#define HAUTEUR 675
#define NIMAGE_YODA 8
#define NIMAGE_VADOR 1
#define NIMAGE_VAISSEAU 3

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>

/* CORRECTION : extern ici, definition unique dans partie.c
   Sans extern, chaque .c qui include lib.h cree sa propre variable
   -> erreur "multiple definition of fermer" a la compilation. */
extern volatile int fermer;

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
} Yoda;

typedef struct {
    int x;
    int y;
    int dx;
    int tx;
    int ty;
    int vitesse;
    int taille;
    int imgcourante;
    int cptimg;
    int tmpimg;
} Vador;

typedef struct {
    int reussite;
    int nb_vaisseaux;
    int vaisseaux_detruits;
    int temps_limite;
    int temps_ecoule;
} Niveau;

typedef struct {
    char pseudo[20];
    int score;
} Joueur;

typedef struct Vaisseau {
    int x;
    int y;
    int dx;
    int dy;
    int tx;
    int ty;
    int vitesse;
    int taille;
    int imgcourante;
    int cptimg;
    int tmpimg;
    struct Vaisseau* suivant;
    struct Vaisseau* precedent;
    /* Physique parabolique */
    float x_f;   /* position X en float */
    float y_f;   /* position Y en float */
    float vx_f;  /* vitesse horizontale en float */
    float vy_f;  /* vitesse verticale  en float (positif = vers le bas) */
} Vaisseau;

typedef struct Projectile {
    int x;
    int y;    /* CORRECTION : champ y manquant, le projectile ne pouvait pas se deplacer */
    int tx;
    int ty;
    int dy;   /* CORRECTION : vitesse verticale manquante */
    struct Projectile* suivant;
    struct Projectile* precedent;
} Projectile;

#endif

