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
/* CORRECTION : Vaisseau** pour pouvoir mettre a jour la tete si on supprime
   le premier noeud de la liste. */
void division_vaisseau(Vaisseau** tete_vaisseau, int i) {
    Vaisseau* vai = vaisseau_i(*tete_vaisseau, i);
    if (vai == NULL) return;

    if (vai->taille >= 3) {
        /* Vaisseau de taille max : le supprimer */
        if (vai->suivant  != NULL) vai->suivant->precedent  = vai->precedent;
        if (vai->precedent != NULL) vai->precedent->suivant = vai->suivant;
        if (vai == *tete_vaisseau)  *tete_vaisseau           = vai->suivant;
        free(vai);
    } else {
        /* Diviser en deux : augmenter la taille et creer un clone en miroir */
        vai->taille++;
        Vaisseau* nouveau = (Vaisseau*)malloc(sizeof(Vaisseau));
        if (nouveau == NULL) {
            allegro_message("erreur d'allocation memoire");
            exit(EXIT_FAILURE);
        }
        *nouveau           = *vai;        /* copie tous les champs (x_f, y_f, vx_f, vy_f inclus) */
        nouveau->dx        = -vai->dx;    /* direction entiere opposee */
        nouveau->vx_f      = -vai->vx_f;  /* direction flottante opposee */
        /* Les deux bulles repartent du sol avec la vitesse de rebond de leur nouvelle taille */
        vai->vy_f          = vitesse_rebond(vai->taille);
        nouveau->vy_f      = vitesse_rebond(nouveau->taille);
        if(vai->taille == 2){
            vai->tx= 60;
            vai->ty= 58;
        }else{
            vai->tx= 30;
            vai->ty= 29;
        }
        nouveau->precedent = vai;
        nouveau->suivant   = vai->suivant;
        vai->suivant       = nouveau;
        if (nouveau->suivant != NULL) nouveau->suivant->precedent = nouveau;
    }
}

/* CORRECTION : Projectile** pour que le nouveau noeud soit visible dans main.
   Ajout des champs y, ty, dy qui manquaient. */
void tirer(Projectile** tete_projectile, Yoda yoda) {
    Projectile* nouveau = (Projectile*)malloc(sizeof(Projectile));
    if (nouveau == NULL) {
        allegro_message("erreur d'allocation memoire");
        exit(EXIT_FAILURE);
    }
    nouveau->x         = yoda.x + yoda.tx / 2;
    nouveau->y         = yoda.y;
    nouveau->tx        = 6;
    nouveau->ty        = 10;
    nouveau->dy        = -8;             /* monte vers le haut */
    nouveau->suivant   = *tete_projectile;
    nouveau->precedent = NULL;
    if (*tete_projectile != NULL) (*tete_projectile)->precedent = nouveau;
    *tete_projectile = nouveau;
}

void boucle_de_jeu(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                   Vador* vador, Vaisseau** tete_vaisseau,
                   Projectile** tete_projectile, BITMAP* page, BITMAP* bg,
                   BITMAP* img_yoda[], BITMAP* img_vador[],
                   BITMAP* img_vaisseau[]) {

    /* CORRECTION : l'original detruisait page ici sans le recreer, puis
       continuait a l'utiliser -> crash. On detruit seulement bg et on le recharge. */
    if (bg != NULL) destroy_bitmap(bg);
    bg = charger_bg(1);
    afficher_niveau(niv, niveau, *yoda, *vador, *tete_vaisseau,
                    *tete_projectile, page, bg,
                    img_yoda, img_vador, img_vaisseau);
    afficher_decompte(page, bg);

    while (!fermer) {

        /* Deplacements */
        deplacement_vaisseau(*tete_vaisseau);
        deplacement_projectile(tete_projectile);

        /* CORRECTION : on parcourt la liste chainee directement plutot que
           d'utiliser niveau[*niv].nb_vaisseaux qui ne reflete pas les divisions.
           On sauvegarde le suivant avant toute modification de la liste. */
        Vaisseau* vai = *tete_vaisseau;
        while (vai != NULL) {
            Vaisseau* suivant_vai = vai->suivant;
            int idx = 0;
            /* Trouver l'indice courant pour les fonctions qui en ont besoin */
            Vaisseau* tmp = *tete_vaisseau;
            while (tmp != NULL && tmp != vai) { idx++; tmp = tmp->suivant; }

            if (bord_touche(*tete_vaisseau, idx)) {
                rebond(*tete_vaisseau, idx);
            }

            if (collision_projectile_vaisseau(*tete_projectile, *tete_vaisseau, idx) != 0) {
                liberer_projectile(*tete_projectile); /* CORRECTION : supprimer le projectile touche */
                *tete_projectile = NULL;
                division_vaisseau(tete_vaisseau, idx);
                niveau[*niv].vaisseaux_detruits++;
                joueur->score += 10;
                break;
            }

            if (collision_yoda_vaisseau(*yoda, *tete_vaisseau, idx)) {
                fin_de_partie(niv, niveau, joueur, tete_vaisseau,
                              tete_projectile, page, bg);
                return;
            }

            vai = suivant_vai;
        }

        if(niveau[*niv].vaisseaux_detruits/7 >= niveau[*niv].nb_vaisseaux) {
            niveau[*niv].reussite = 1;
            (*niv)++;
            if (*niv >= 3) {
                /* Fin du jeu : le joueur a reussi tous les niveaux */
                fin_de_partie(niv, niveau, joueur, tete_vaisseau,
                              tete_projectile, page, bg);
                return;
            }
            initialisation_niveau_suivant(niv, niveau, joueur, yoda, vador, tete_vaisseau,
                                        tete_projectile);
            afficher_niveau(niv, niveau, *yoda, *vador, *tete_vaisseau,
                            *tete_projectile, page, bg,
                            img_yoda, img_vador, img_vaisseau);
            afficher_decompte(page, bg);
        }

        /* Saisie clavier */
        if (key[KEY_ESC]) {
            fermer = 1;
            break;
        }
        if (key[KEY_SPACE]) {
            tirer(tete_projectile, *yoda);
        }
        if (key[KEY_RIGHT] && yoda->x + yoda->tx < LARGEUR) {
            deplacement_a_droite(yoda);
        }
        if (key[KEY_LEFT] && yoda->x > 0) {
            deplacement_a_gauche(yoda);
        }
        if (key[KEY_P]){
            afficher_menu_pause(page, bg);
            gestion_menu_pause(page, bg);
        }

        /* Affichage */
        afficher_niveau(niv, niveau, *yoda, *vador, *tete_vaisseau,
                        *tete_projectile, page, bg,
                        img_yoda, img_vador, img_vaisseau);

        rest(16); /* ~60 fps */
        fflush(stdout);
    }

    if (bg != NULL) destroy_bitmap(bg);
}

