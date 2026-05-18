#ifndef SAISIE_H
#define SAISIE_H

int gestion_menu(BITMAP* bg, BITMAP* page);
void gestion_menu_pause(int* niv, Niveau niveau[], Joueur* joueur, Yoda yoda,
                     Vador vador, Vaisseau* tete_vaisseau,
                     Projectile* tete_projectile,
                     BITMAP* page, BITMAP* bg,
                     BITMAP* img_yoda[], BITMAP* img_vador[],
                     BITMAP* img_vaisseau[]);
void gestion_menu_regles(BITMAP* page, BITMAP* bg);
void deplacement_a_droite(Yoda* yoda);
void deplacement_a_gauche(Yoda* yoda);

/* CORRECTION : la fonction retournait char alors qu'elle doit remplir une chaine.
   On lui passe le tableau destination en parametre. */
void pseudo(char* out);

#endif
