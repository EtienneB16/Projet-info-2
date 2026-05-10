#ifndef AFFICHAGE_H
#define AFFICHAGE_H

void afficher_menu(BITMAP* bg, BITMAP* page);
void afficher_niveau(int* niv, Niveau niveau[], Yoda yoda, Vador vador,
                     Vaisseau* tete_vaisseau, Projectile* tete_projectile,
                     BITMAP* page, BITMAP* bg,
                     BITMAP* img_yoda[], BITMAP* img_vador[],
                     BITMAP* img_vaisseau[]);

/* CORRECTION : Joueur* au lieu de Joueur pour eviter une copie inutile
   et rester coherent avec le reste du code */
void afficher_fin_de_partie(int* niv, Niveau niveau[], Joueur* joueur,
                             BITMAP* page, BITMAP* bg);
void defiler_img_yoda(Yoda* yoda);

#endif
