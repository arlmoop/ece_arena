#ifndef HEADER_H
#define HEADER_H

#include <allegro.h>
#include <stdbool.h>

#define TAILLE_MAP 19 //Impair pr bien placer les spawns d'equipe
#define NB_IMG_PERSOS 1 //Nb de sprite par perso (mieux si tt pareil)
#define NB_CASES 10 //Nb de cases hors spawns
#define NB_OBS 8
#define NB_SPAWNS 4
#define Y_DEPART 10
#define PRCNT_OBS 8
#define NB_POTION 4 // nombre de potion par perso
#define MENU_W 400
#define MENU_H 300
#define MENU_CLASSES_W 600
#define MENU_CLASSES_H 480


typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, ycentre_losange;
    bool o, p, r;
    // O 0:pas occupee 1:occupee par obstacle
    // P 0:pas occupee 1:occupee par perso
    // TYPE 1:terre 2:autre
    BITMAP *img;
} t_case;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre;
    bool e, a;
    // E 0:existe pas 1:existe
    // A 0:n'a pas ete affiche 1:a ete affiche
    BITMAP *img;
} t_obstacle;

typedef struct {
    int x, y, dx, dy, tx, ty, xcentre, ycentre, classe, imgcourante, cptimg, tmpimg, equipe;
    bool e;
    // E 0:existe pas 1:existe
    BITMAP *img;
}t_perso;

typedef enum {
    MENU_PRINCIPAL,
    CHOIX_MAP,
    CHOIX_JOUEURS,
    CHOIX_CLASSES
} EtatMenu;

typedef struct {
    int x, y, xf, yf;
    //xf et yf les coordonnees de la fin de la taille des images des potions
    BITMAP* img;
} t_potion;


//OUTILS.C
void initialisation_allegro();
void magenta (BITMAP* image, int r_max, int g_max, int b_max);


// MENU.C
t_spriteimmo initspriteimmo();
int menu(int *aleatoire,int *theme,int *nb_joueurs,int choix_joueurs[], int *equipe);


// PERSOS.C
t_perso init_perso(int n, int i, int j);
void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[TAILLE_MAP][TAILLE_MAP], bool equipe, int choix_joueurs[]);


// MAP.C
t_case init_case(int n, int i, int j);
t_obstacle init_obstacle(int n, int i, int j);
t_potion init_inventaire(char nom_potion[20], int i);
void creer_fichier();
void charger_fichier(int tab_map[TAILLE_MAP][TAILLE_MAP]);
void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], bool equipe, int nb_joueurs);
void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]);
void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);
void afficher_obstacles_persos(BITMAP *buffer, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[TAILLE_MAP][TAILLE_MAP]);
int point_dans_losange(t_case c);
void remplir_losange(t_case c, BITMAP * buffer);
void souris_tab(t_case c[TAILLE_MAP][TAILLE_MAP], BITMAP *buffer, int *ligne_prec, int *colonne_prec);


// SORTS.C
void creer_potion (t_potion p[NB_POTION], char nom_potion[20]);
void point_vie (BITMAP* buffer, t_potion p[NB_POTION], int degats);
void afficher_inventaire (BITMAP* buffer, t_potion p[NB_POTION], int degats);
void souris_potion (BITMAP* buffer, t_potion p[NB_POTION]);


#endif //HEADER_H
