#ifndef SAISIE_H
#define SAISIE_H

int gestion_menu(BITMAP* bg, BITMAP* page);
void deplacement_a_droite(Yoda* yoda);
void deplacement_a_gauche(Yoda* yoda);

/* CORRECTION : la fonction retournait char alors qu'elle doit remplir une chaine.
   On lui passe le tableau destination en parametre. */
void pseudo(char* out);

#endif
