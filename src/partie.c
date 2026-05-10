#include "headers/lib.h"
#include "headers/partie.h"
#include "headers/saisie.h"
#include "headers/affichage.h"

/* CORRECTION : definition unique de fermer (extern dans lib.h, defini ici) */
volatile int fermer = 0;

void fermeture() {
    fermer = 1;
}

void initialisation_Allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGEUR, HAUTEUR, 0, 0) != 0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    set_close_button_callback(fermeture);
}

void initialisation_yoda(BITMAP *img[NIMAGE_YODA]) {
    for (int i = 0; i < NIMAGE_YODA; i++) {
        char path[50];
        sprintf(path, "src/personnages/yoda/run_%d.bmp", i);
        img[i] = load_bitmap(path, NULL);
        if (!img[i]) {
            allegro_message("pas pu trouver %s", path);
            exit(EXIT_FAILURE);
        }
    }
}

void initialisation_vador(BITMAP *img[NIMAGE_VADOR]) {
    for (int i = 0; i < NIMAGE_VADOR; i++) {
        char path[50];
        /* CORRECTION : chemin manquait "src/" */
        sprintf(path, "src/personnages/vador/vador_%d.bmp", i);
        img[i] = load_bitmap(path, NULL);
        if (!img[i]) {
            allegro_message("pas pu trouver %s", path);
            exit(EXIT_FAILURE);
        }
    }
}

void initialisation_vaisseau(BITMAP *img[NIMAGE_VAISSEAU]) {
    for (int i = 0; i < NIMAGE_VAISSEAU; i++) {
        char path[50];
        /* CORRECTION : chemin manquait "src/" */
        sprintf(path, "src/personnages/vaisseau/vaisseau_%d.bmp", i);
        img[i] = load_bitmap(path, NULL);
        if (!img[i]) {
            allegro_message("pas pu trouver %s", path);
            exit(EXIT_FAILURE);
        }
    }
}

BITMAP* charger_bg(int i) {
    char path[255];
    sprintf(path, "src/backgrounds/bg_%d.bmp", i);
    BITMAP* p = load_bitmap(path, NULL);
    if (!p) {
        allegro_message("impossible de charger %s", path);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return p;
}

/* CORRECTION : Vaisseau** pour que la tete soit modifiee chez l'appelant */
void creation_vaisseau(Vaisseau** tete_vaisseau, int nb_vaisseau) {
    for (int i = 0; i < nb_vaisseau; i++) {
        Vaisseau* nouveau = (Vaisseau*)malloc(sizeof(Vaisseau));
        if (nouveau == NULL) {
            allegro_message("erreur d'allocation memoire");
            exit(EXIT_FAILURE);
        }
        nouveau->x         = (LARGEUR - 50) * i / nb_vaisseau;
        nouveau->y         = 150;
        nouveau->dx        = (rand() % 3) + 1;
        nouveau->dy        = (rand() % 3) + 1;
        if (rand() % 2) nouveau->dx = -nouveau->dx;
        if (rand() % 2) nouveau->dy = -nouveau->dy;
        nouveau->tx        = 119;
        nouveau->ty        = 116;
        nouveau->x_f       = (float)nouveau->x;
        nouveau->y_f       = (float)nouveau->y;
        nouveau->vx_f      = (float)nouveau->dx;
        nouveau->vy_f      = -10.0f; /* vitesse initiale vers le haut */
        nouveau->vitesse   = 1;
        nouveau->taille    = 1;
        nouveau->imgcourante = 0;
        nouveau->cptimg    = 0;
        nouveau->tmpimg    = 10;
        nouveau->suivant   = *tete_vaisseau;
        nouveau->precedent = NULL;
        if (*tete_vaisseau != NULL) (*tete_vaisseau)->precedent = nouveau;
        *tete_vaisseau = nouveau;
    }
}

void liberer_vaisseau(Vaisseau* tete_vaisseau) {
    Vaisseau* courant = tete_vaisseau;
    while (courant != NULL) {
        Vaisseau* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
}

void liberer_projectile(Projectile* tete_projectile) {
    Projectile* courant = tete_projectile;
    while (courant != NULL) {
        Projectile* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
}

/* CORRECTION : tous les parametres modifiables passent par pointeur */
void initialisation_partie(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                            Vador* vador, Vaisseau** tete_vaisseau,
                            Projectile** tete_projectile) {
    srand(time(NULL));

    /* CORRECTION : *niv=0 pour partir du 1er niveau (tableau 0-base).
       L'original avait *niv=1 ce qui sautait directement au niveau 2. */
    *niv = 0;

    for (int i = 0; i < 3; i++) {
        niveau[i].reussite          = 0;
        niveau[i].nb_vaisseaux      = 0;
        niveau[i].vaisseaux_detruits = 0;
        niveau[i].temps_ecoule      = 0;
        niveau[i].temps_limite      = 60;
    }
    niveau[0].nb_vaisseaux = 1;
    niveau[1].nb_vaisseaux = 2;
    niveau[2].nb_vaisseaux = 3;

    joueur->score = 0;
    /* CORRECTION : pseudo() remplit le tableau, on passe joueur->pseudo
       L'original faisait joueur.pseudo[20]=pseudo() : acces hors-limites
       (indice max = 19) ET stockait un char au lieu d'une chaine. */
    pseudo(joueur->pseudo);

    yoda->tx         = 23;
    yoda->ty         = 91;
    yoda->x          = (LARGEUR - yoda->tx) / 2;
    yoda->y          = HAUTEUR - yoda->ty - 20;
    yoda->dx         = 0;
    yoda->vitesse    = 5;
    yoda->imgcourante = 0;
    yoda->cptimg     = 0;
    yoda->tmpimg     = 20;

    /* Dimensions a affiner selon la taille reelle de l'image vador */
    vador->tx         = 164;
    vador->ty         = 200;
    vador->x          = (LARGEUR - vador->tx) / 2;
    vador->y          = 150;
    vador->dx         = 0;
    vador->vitesse    = 0;
    vador->taille     = 1;
    vador->imgcourante = 0;
    vador->cptimg     = 0;
    vador->tmpimg     = 10;

    /* CORRECTION : on remet les listes a NULL avant de les remplir,
       et on passe bien Vaisseau** pour que creation_vaisseau
       mette a jour le pointeur de l'appelant. */
    *tete_vaisseau   = NULL;
    *tete_projectile = NULL;
    creation_vaisseau(tete_vaisseau, niveau[*niv].nb_vaisseaux);
}

void initialisation_niveau_suivant(int* niv, Niveau niveau[], Joueur* joueur, Yoda* yoda,
                                Vador* vador, Vaisseau** tete_vaisseau,
                                Projectile** tete_projectile) {
    /* Reinitialiser les listes de vaisseaux et projectiles */
    liberer_vaisseau(*tete_vaisseau);
    *tete_vaisseau = NULL;
    liberer_projectile(*tete_projectile);
    *tete_projectile = NULL;

    creation_vaisseau(tete_vaisseau, niveau[*niv].nb_vaisseaux);

    /* Recentrer Yoda et Vador */
    yoda->x = (LARGEUR - yoda->tx) / 2;
    yoda->y = HAUTEUR - yoda->ty - 20;
    vador->x = (LARGEUR - vador->tx) / 2;
    vador->y = 150;
}

void fin_de_partie(int* niv, Niveau niveau[], Joueur* joueur,
                   Vaisseau** tete_vaisseau, Projectile** tete_projectile,
                   BITMAP* page, BITMAP* bg) {
    liberer_vaisseau(*tete_vaisseau);
    *tete_vaisseau = NULL;
    liberer_projectile(*tete_projectile);
    *tete_projectile = NULL;

    /* On charge un fond dedie a l'ecran de fin */
    BITMAP* fin_bg = charger_bg(3);
    int retour = 0;
    while (!retour) {
        afficher_fin_de_partie(niv, niveau, joueur, page, fin_bg);
        if (key[KEY_ESC]) {
            retour = 1;
        }
        rest(100);
    }

    destroy_bitmap(fin_bg);
}


void sauvegarde(int* niv, Niveau niveau[], Joueur* joueur) {
    FILE* f = fopen(joueur->pseudo, "w");
    if (f == NULL) {
        allegro_message("erreur d'ouverture du fichier de sauvegarde");
        return;
    }
    fprintf(f, "%d\n", *niv);
    fprintf(f, "%d %d %d %d %d\n", niveau[*niv].reussite, niveau[*niv].nb_vaisseaux,
            niveau[*niv].vaisseaux_detruits, niveau[*niv].temps_limite,
            niveau[*niv].temps_ecoule);
    fprintf(f, "%s %d\n", joueur->pseudo, joueur->score);
    fclose(f);
}

void charger_sauvegarde(int* niv, Niveau niveau[], Joueur* joueur) {
    FILE* f = fopen(joueur->pseudo, "r");
    if (f == NULL) {
        allegro_message("pas de sauvegarde trouvee");
        return;
    }
    fscanf(f, "%d\n", niv);
    fscanf(f, "%d %d %d %d %d\n", &niveau[*niv].reussite, &niveau[*niv].nb_vaisseaux,
           &niveau[*niv].vaisseaux_detruits, &niveau[*niv].temps_limite,
           &niveau[*niv].temps_ecoule);
    fscanf(f, "%s %d\n", joueur->pseudo, &joueur->score);
    fclose(f);
}
