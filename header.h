#ifndef HEADER_H
#define HEADER_H

#include <allegro.h>

#define TAILLE_MAP 19 //Impair pr bien placer les spawns d'equipe
#define NB_IMG_PERSOS 10 //Nb de sprite par perso (mieux si tt pareil)
#define NB_CASES 10 //Nb de cases hors spawns
#define NB_OBS 8
#define Y_DEPART 60
#define PRCNT_OBS 8


typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, e;
    // E 1:occupee 0:pas occupee
    // TYPE 1:terre 2:autre
    BITMAP *img;
} t_case;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, e;
    // E 1:affiche 0:pas affiche
    // TYPE 1:arbre 2:autre
    BITMAP *img;
} t_obstacle;

typedef struct {
    int x, y, dx, dy, tx, ty, xcentre, ycentre, imgcourante, cptimg, tmpimg;
    BITMAP *img[NB_IMG_PERSOS];
}t_perso;


void initialisation_allegro();
t_spriteimmo initspriteimmo();
t_perso init_perso();
t_case init_case(int n, int i, int j);
t_obstacle init_obstacle(int n, int i, int j);
void creer_fichier();
void charger_fichier(int tab_map[TAILLE_MAP][TAILLE_MAP]);
void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP]);
void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]);
void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);
void afficher_obstacles(BITMAP *buffer, t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);

#endif //HEADER_H
