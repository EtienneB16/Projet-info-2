#include "headers/lib.h"
#include "headers/jeu.h"
#include "headers/affichage.h"
#include "headers/saisie.h"
#include "headers/partie.h"
#include <math.h>

#define GRAVITE 0.4f 

int bord_touche(Vaisseau* vai) {
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
static void supprimer_projectile(Projectile** tete_projectile, Projectile* proj) {
    if (proj == NULL || tete_projectile == NULL) return;
    if (proj->precedent != NULL) proj->precedent->suivant = proj->suivant;
    else *tete_projectile = proj->suivant;
    if (proj->suivant != NULL) proj->suivant->precedent = proj->precedent;
    free(proj);
}

static int circle_collision(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int r = r1 + r2;
    return dx * dx + dy * dy <= r * r;
}

Projectile* collision_projectile_vaisseau(Projectile* tete_projectile,
                                           Vaisseau* vai) {
    if (tete_projectile == NULL || vai == NULL) return NULL;

    int vx = vai->x + vai->tx / 2;
    int vy = vai->y + vai->ty / 2;
    int vr = (vai->tx < vai->ty ? vai->tx : vai->ty) / 2;
    if (vr > 10) vr -= 10;
    if (vr < 4) vr = 4;

    Projectile* proj = tete_projectile;
    while (proj != NULL) {
        int px = proj->x + proj->tx / 2;
        int py = proj->y + proj->ty / 2;
        int pr = (proj->tx < proj->ty ? proj->tx : proj->ty) / 2;
        if (pr < 2) pr = 2;
        if (circle_collision(px, py, pr, vx, vy, vr)) {
            return proj;
        }
        proj = proj->suivant;
    }
    return NULL;
}

int collision_yoda_vaisseau(Yoda yoda, Vaisseau* vai) {
    if (vai == NULL) return 0;

    int yx = yoda.x + yoda.tx / 2;
    int yy = yoda.y + yoda.ty / 2;
    int yr = (yoda.tx < yoda.ty ? yoda.tx : yoda.ty) / 2;
    if (yr > 12) yr -= 12;
    if (yr < 6) yr = 6;

    int vx = vai->x + vai->tx / 2;
    int vy = vai->y + vai->ty / 2;
    int vr = (vai->tx < vai->ty ? vai->tx : vai->ty) / 2;
    if (vr > 10) vr -= 10;
    if (vr < 5) vr = 5;

    return circle_collision(yx, yy, yr, vx, vy, vr);
}

void rebond(Vaisseau* vai) {
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
void division_vaisseau(Vaisseau** tete_vaisseau, Vaisseau* vai) {
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
    afficher_decompte(niv, niveau, *yoda, *vador, *tete_vaisseau,
                      *tete_projectile, page, bg,
                      img_yoda, img_vador, img_vaisseau);

    while (!fermer) {

        /* Deplacements */
        deplacement_vaisseau(*tete_vaisseau);
        deplacement_projectile(tete_projectile);

        /* CORRECTION : on parcourt la liste chainee directement plutot que
           d'utiliser niveau[*niv].nb_vaisseaux qui ne reflete pas les divisions. */
        Vaisseau* vai = *tete_vaisseau;
        while (vai != NULL) {
            Vaisseau* suivant_vai = vai->suivant;

            if (bord_touche(vai)) {
                rebond(vai);
            }

            Projectile* proj = collision_projectile_vaisseau(*tete_projectile, vai);
            if (proj != NULL) {
                supprimer_projectile(tete_projectile, proj);
                division_vaisseau(tete_vaisseau, vai);
                niveau[*niv].vaisseaux_detruits++;
                joueur->score += 10;
                break;
            }

            if (collision_yoda_vaisseau(*yoda, vai)) {
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
            afficher_decompte(niv, niveau, *yoda, *vador, *tete_vaisseau,
                              *tete_projectile, page, bg,
                              img_yoda, img_vador, img_vaisseau);
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
            gestion_menu_pause(niv, niveau, joueur, *yoda, *vador, *tete_vaisseau,
                               *tete_projectile, page, bg,
                               img_yoda, img_vador, img_vaisseau);
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

