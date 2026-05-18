#ifndef PARTIE_H
#define PARTIE_H

void fermeture();
void initialisation_Allegro();
void initialisation_yoda(BITMAP *img[NIMAGE_YODA]);
void initialisation_vador(BITMAP *img[NIMAGE_VADOR]);
void initialisation_vaisseau(BITMAP *img[NIMAGE_VAISSEAU]);
BITMAP* charger_bg(int i);

/* CORRECTION : Vaisseau** pour que la tete de liste soit modifiable depuis l'appelant */
void creation_vaisseau(Vaisseau** tete_vaisseau, int nb_vaisseau);
void liberer_vaisseau(Vaisseau* tete_vaisseau);
void liberer_projectile(Projectile* tete_projectile);

/* CORRECTION : Joueur*, Yoda*, Vador*, Vaisseau**, Projectile** pour que
   les initialisations soient visibles dans main() */
void initialisation_partie(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                            Vador* vador, Vaisseau** tete_vaisseau,
                            Projectile** tete_projectile);

void initialisation_niveau_suivant(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                                Vador* vador, Vaisseau** tete_vaisseau,
                                Projectile** tete_projectile);

void fin_de_partie(int* niv, Niveau niveau[], Joueur* joueur,
                   Vaisseau** tete_vaisseau, Projectile** tete_projectile,
                   BITMAP* page, BITMAP* bg);

void liberer_bg_cache(void);

void sauvegarde(int* niv, Niveau niveau[], Joueur* joueur);
void charger_sauvegarde(int* niv, Niveau niveau[], Joueur* joueur);

#endif
