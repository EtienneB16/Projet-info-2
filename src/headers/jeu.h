#ifndef JEU_H
#define JEU_H

int bord_touche(Vaisseau* vai);
void deplacement_vaisseau(Vaisseau* tete_vaisseau);

/* CORRECTION : Projectile** pour pouvoir supprimer les noeuds de la liste */
void deplacement_projectile(Projectile** tete_projectile);

/* CORRECTION : retourne le projectile touche (NULL si pas de collision)
   pour pouvoir le supprimer immediatement apres la collision */
Projectile* collision_projectile_vaisseau(Projectile* tete_projectile,
                                           Vaisseau* vai);
int collision_yoda_vaisseau(Yoda yoda, Vaisseau* vai);
void rebond(Vaisseau* vai);

/* CORRECTION : Vaisseau** pour modifier la tete si on supprime le 1er noeud */
void division_vaisseau(Vaisseau** tete_vaisseau, Vaisseau* vai);

/* CORRECTION : Projectile** pour que le nouveau projectile soit visible depuis l'appelant */
void tirer(Projectile** tete_projectile, Yoda yoda);

/* CORRECTION : tout en pointeurs pour que les modifications soient propagees */
void boucle_de_jeu(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                   Vador* vador, Vaisseau** tete_vaisseau,
                   Projectile** tete_projectile, BITMAP* page, BITMAP* bg,
                   BITMAP* img_yoda[], BITMAP* img_vador[],
                   BITMAP* img_vaisseau[]);

#endif
