#ifndef JEU_H
#define JEU_H

Vaisseau* vaisseau_i(Vaisseau* tete_vaisseau, int i);
int bord_touche(Vaisseau* tete_vaisseau, int i);
void deplacement_vaisseau(Vaisseau* tete_vaisseau);

/* CORRECTION : Projectile** pour pouvoir supprimer les noeuds de la liste */
void deplacement_projectile(Projectile** tete_projectile);

/* CORRECTION : retourne le Projectile* touche (NULL si pas de collision)
   pour pouvoir le supprimer immediatement apres la collision */
int collision_projectile_vaisseau(Projectile* tete_projectile,
                                           Vaisseau* tete_vaisseau, int i);
int collision_yoda_vaisseau(Yoda yoda, Vaisseau* tete_vaisseau, int i);
void rebond(Vaisseau* tete_vaisseau, int i);

/* CORRECTION : Vaisseau** pour modifier la tete si on supprime le 1er noeud */
void division_vaisseau(Vaisseau** tete_vaisseau, int i);

/* CORRECTION : Projectile** pour que le nouveau projectile soit visible depuis l'appelant */
void tirer(Projectile** tete_projectile, Yoda yoda);

/* CORRECTION : tout en pointeurs pour que les modifications soient propagees */
void boucle_de_jeu(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                   Vador* vador, Vaisseau** tete_vaisseau,
                   Projectile** tete_projectile, BITMAP* page, BITMAP* bg,
                   BITMAP* img_yoda[], BITMAP* img_vador[],
                   BITMAP* img_vaisseau[]);

#endif
