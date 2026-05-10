#include "headers/lib.h"
#include "headers/jeu.h"
#include "headers/affichage.h"
#include "headers/saisie.h"
#include "headers/partie.h"
#include <math.h>

#define GRAVITE 0.4f 

Vaisseau* vaisseau_i(Vaisseau* tete_vaisseau, int i) {
    Vaisseau* temp = tete_vaisseau;
    for (int j = 0; j < i && temp != NULL; j++) {
        temp = temp->suivant;
    }
    return temp;
}

int bord_touche(Vaisseau* tete_vaisseau, int i) {
    Vaisseau* vai = vaisseau_i(tete_vaisseau, i);
    if (vai == NULL) return 0;
    if (vai->x_f <= 0 || vai->x_f + vai->tx >= LARGEUR) return 1;
    if (vai->y_f <= 0 || vai->y_f + vai->ty >= HAUTEUR) return 1;
    return 0;
}

/* Vitesse de rebond verticale selon la taille de la bulle.
   C'est une vitesse FIXE appliquee a chaque rebond au sol,
   ce qui garantit une hauteur de rebond constante et identique
   pour toutes les bulles de meme taille.
   taille 1 = grande bulle  -> rebond tres haut
   taille 2 = bulle moyenne -> rebond moyen
   taille 3 = petite bulle  -> rebond bas                          */
static float vitesse_rebond(int taille) {
    switch (taille) {
        case 1:  return -19.0f;   /* grande  : rebond haut  */
        case 2:  return -18.0f;   /* moyenne : rebond moyen */
        default: return  -17.0f;   /* petite  : rebond bas   */
    }
}

void deplacement_vaisseau(Vaisseau* tete_vaisseau) {
    Vaisseau* vai = tete_vaisseau;
    while (vai != NULL) {
        /* Gravite : acceleration vers le bas a chaque frame */
        vai->vy_f += GRAVITE;

        /* Mise a jour de la position en float */
        vai->x_f  += vai->vx_f;
        vai->y_f  += vai->vy_f;

        /* Synchronisation entiers pour l'affichage et les collisions */
        vai->x = (int)vai->x_f;
        vai->y = (int)vai->y_f;

        vai = vai->suivant;
    }
}

/* CORRECTION : fonction manquante. Deplace les projectiles vers le haut
   et supprime ceux qui sortent de l'ecran. */
void deplacement_projectile(Projectile** tete_projectile) {
    Projectile* proj = *tete_projectile;
    while (proj != NULL) {
        Projectile* suivant = proj->suivant; /* sauvegarder avant suppression eventuelle */
        proj->y += proj->dy;

        if (proj->y + proj->ty < 0) {
            /* Supprimer le projectile sorti par le haut */
            if (proj->precedent != NULL) proj->precedent->suivant = proj->suivant;
            else                         *tete_projectile         = proj->suivant;
            if (proj->suivant  != NULL)  proj->suivant->precedent = proj->precedent;
            free(proj);
        }
        proj = suivant;
    }
}

/* CORRECTION : verification sur X ET Y (l'original ne verifiait que X) */
int collision_projectile_vaisseau(Projectile* tete_projectile,
                                           Vaisseau* tete_vaisseau, int i) {
    if (tete_projectile == NULL) return 0;
    Vaisseau* vai = vaisseau_i(tete_vaisseau, i);
    if (vai == NULL) return 0;
    Projectile* proj = tete_projectile;
    while (proj != NULL) {
        if (proj->x >= vai->x && proj->x <= vai->x + vai->tx &&
            proj->y >= vai->y && proj->y <= vai->y + vai->ty) {
            return 1;   /* retourne 1 si un projectile touche le vaisseau */
        }
        proj = proj->suivant;
    }
    return 0;
}

int collision_yoda_vaisseau(Yoda yoda, Vaisseau* tete_vaisseau, int i) {
    Vaisseau* vai = vaisseau_i(tete_vaisseau, i);
    if (vai == NULL) return 0;
    if (yoda.x + yoda.tx > vai->x && yoda.x < vai->x + vai->tx &&
        yoda.y + yoda.ty > vai->y && yoda.y < vai->y + vai->ty) return 1;
    return 0;
}

void rebond(Vaisseau* tete_vaisseau, int i) {
    Vaisseau* vai = vaisseau_i(tete_vaisseau, i);
    if (vai == NULL) return;

    /* --- Sol : on repart toujours avec la meme vitesse fixe vers le haut ---
       La hauteur du prochain arc est determinee uniquement par vitesse_rebond(),
       independamment de la vitesse d'arrivee -> rebond constant a chaque fois. */
    if (vai->y_f + vai->ty >= HAUTEUR) {
        vai->y_f  = (float)(HAUTEUR - vai->ty); /* coller au sol */
        vai->vy_f = vitesse_rebond(vai->taille); /* vitesse fixe vers le haut */
        vai->y    = (int)vai->y_f;
    }

    /* --- Plafond --- */
    if (vai->y_f <= 0) {
        vai->y_f  = 0.0f;
        vai->vy_f = (float)fabs(vai->vy_f); /* repartir vers le bas */
        vai->y    = 0;
    }

    /* --- Murs lateraux : simple inversion de vx_f --- */
    if (vai->x_f <= 0) {
        vai->x_f  = 0.0f;
        vai->vx_f = (float)fabs(vai->vx_f);
        vai->x    = 0;
    }
    if (vai->x_f + vai->tx >= LARGEUR) {
        vai->x_f  = (float)(LARGEUR - vai->tx);
        vai->vx_f = -(float)fabs(vai->vx_f);
        vai->x    = (int)vai->x_f;
    }
}

