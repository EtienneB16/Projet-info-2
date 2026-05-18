#include "headers/lib.h"
#include "headers/affichage.h"
#include "headers/partie.h"

void afficher_menu(BITMAP* bg, BITMAP* page) {
    blit(bg, page, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    textout_centre_ex(page, font, "Jouer",
                      LARGEUR/2, 280, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Reprendre une partie",
                      LARGEUR/2, 375, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Regles",
                      LARGEUR/2, 470, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Quitter",
                      LARGEUR/2, 565, makecol(255, 255, 255), -1);
    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    show_mouse(screen);
}

void afficher_menu_pause(BITMAP* page, BITMAP* bg) {
    blit(bg, page, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    textout_centre_ex(page, font, "Reprendre",
                      LARGEUR/2, 280, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Sauvegarder",
                      LARGEUR/2, 375, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Quitter",
                      LARGEUR/2, 470, makecol(255, 255, 255), -1);
    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    show_mouse(screen);
}

void afficher_regles(BITMAP* page, BITMAP* bg) {
    blit(bg, page, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    textout_centre_ex(page, font, "Regles du jeu",
                      LARGEUR/2, 200, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font,
        "Vous incarnez Yoda et devez detruire les bulles ennemis avant la fin du temps imparti.",
        LARGEUR/2, 300, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font,
        "Utilisez les fleches pour vous deplacer et la barre d'espace pour tirer.",
        LARGEUR/2, 400, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font,
        "Appuyez sur Echap pour revenir au menu principal.",
        LARGEUR/2, 500, makecol(255, 255, 255), -1);
    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    show_mouse(screen);
}

void afficher_niveau(int* niv, Niveau niveau[], Yoda yoda, Vador vador,
                     Vaisseau* tete_vaisseau, Projectile* tete_projectile,
                     BITMAP* page, BITMAP* bg,
                     BITMAP* img_yoda[], BITMAP* img_vador[],
                     BITMAP* img_vaisseau[]) {

    /* CORRECTION : l'ordre etait inverse.
       L'original dessinait les textes sur page, PUIS ecrasait page avec blit(bg),
       ce qui effacait tout ce qui venait d'etre dessine.
       Ordre correct : fond -> sprites -> textes -> envoi sur screen. */

    /* 1. Fond */
    blit(bg, page, 0, 0, 0, 0, LARGEUR, HAUTEUR);

    /* 2. Personnages */
    if (yoda.dx >= 0)
        draw_sprite(page, img_yoda[yoda.imgcourante], yoda.x-50, yoda.y);
    else
        draw_sprite_h_flip(page, img_yoda[yoda.imgcourante], yoda.x-50, yoda.y);

    if (*niv == 2)
        draw_sprite(page, img_vador[vador.imgcourante], vador.x, vador.y);

    /* 3. Vaisseaux */
    Vaisseau* courant_vai = tete_vaisseau;
    while (courant_vai != NULL) {
        /* taille 1 = grande -> vaisseau_0, taille 2 = moyenne -> vaisseau_1,
           taille 3 = petite  -> vaisseau_2 */
        draw_sprite(page, img_vaisseau[courant_vai->taille - 1],
                    courant_vai->x, courant_vai->y);
        courant_vai = courant_vai->suivant;
    }

    /* 4. Projectiles (CORRECTION : n'etaient pas affiches du tout) */
    Projectile* proj = tete_projectile;
    while (proj != NULL) {
        rectfill(page, proj->x, proj->y,
                 proj->x + proj->tx, proj->y + proj->ty,
                 makecol(255, 255, 0));
        proj = proj->suivant;
    }

    /* 5. HUD (textes par-dessus tout le reste) */
    char str[30];

    /* Niveau courant (en haut a gauche) */
    sprintf(str, "Niv %d", *niv + 1);
    textout_ex(page, font, str, 10, 10, makecol(255, 255, 255), -1);

    /* Score (en haut a droite) */
    sprintf(str, "Score : %d", niveau[*niv].vaisseaux_detruits * 10);
    textout_ex(page, font, str, LARGEUR - 150, 10, makecol(255, 255, 255), -1);

    /* Vaisseaux detruits / total (a droite du score) */
    sprintf(str, "%d/%d", niveau[*niv].vaisseaux_detruits,
            niveau[*niv].nb_vaisseaux);
    textout_ex(page, font, str, LARGEUR - 60, 25, makecol(255, 255, 255), -1);

    /* Temps restant (centre) */
    sprintf(str, "%d", niveau[*niv].temps_limite - niveau[*niv].temps_ecoule);
    textout_centre_ex(page, font, str, LARGEUR / 2, 10, makecol(255, 255, 255), -1);

    /* 6. Envoi a l'ecran */
    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
}

/* CORRECTION : Joueur* au lieu de Joueur (coherent avec le reste) */
void afficher_fin_de_partie(int* niv, Niveau niveau[], Joueur* joueur,
                             BITMAP* page, BITMAP* bg) {
    blit(bg, page, 0, 0, 0, 0, LARGEUR, HAUTEUR);

    char str[30];
    sprintf(str, "Niveau %d", *niv + 1);
    textout_centre_ex(page, font, str,
                      LARGEUR / 2, 340, makecol(255, 255, 255), -1);
    sprintf(str, "Score : %d", joueur->score);
    textout_centre_ex(page, font, str,
                      LARGEUR / 2, 400, makecol(255, 255, 255), -1);
    sprintf(str, "%d/%d vaisseaux detruits", niveau[*niv].vaisseaux_detruits,
            niveau[*niv].nb_vaisseaux);
    textout_centre_ex(page, font, str,
                      LARGEUR / 2, 430, makecol(255, 255, 255), -1);
    textout_centre_ex(page, font, "Appuyez sur Echap pour revenir au menu",
                      LARGEUR / 2, 500, makecol(255, 255, 255), -1);

    blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
}

void afficher_decompte(int* niv, Niveau niveau[], Yoda yoda, Vador vador,
                     Vaisseau* tete_vaisseau, Projectile* tete_projectile,
                     BITMAP* page, BITMAP* bg,
                     BITMAP* img_yoda[], BITMAP* img_vador[],
                     BITMAP* img_vaisseau[]) {
    for (int i = 3; i > 0; i--) {
        afficher_niveau(niv, niveau, yoda, vador, tete_vaisseau, tete_projectile,
                         page, bg, img_yoda, img_vador, img_vaisseau);
        char str[2];
        sprintf(str, "%d", i);
        textout_centre_ex(page, font, str,
                          LARGEUR / 2, HAUTEUR / 2, makecol(255, 255, 255), -1);
        blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
        rest(1000);
    }
}

void defiler_img_yoda(Yoda* yoda) {
    yoda->cptimg++;
    if (yoda->cptimg >= yoda->tmpimg) {
        yoda->cptimg = 0;
        yoda->imgcourante++;
        if (yoda->imgcourante >= NIMAGE_YODA) yoda->imgcourante = 0;
    }
}
