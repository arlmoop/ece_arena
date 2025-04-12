//
// Created by aurel on 12/04/2025.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <allegro.h>
#include <time.h>
#include <stdlib.h>

#define TAILLE_MAP 19
#define NB_IMG_PERSOS 10
#define NB_CASES 10
#define NB_OBS 5
#define NB_IMG_EAU 5
#define NB_IMG_TERRE 3
#define Y_DEPART 60

typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type;
    // TYPE : 1->terre 2->le reste
    BITMAP *img;
} t_case;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, e;
    // E : 1->affiche 2->pas affiche
    // TYPE : 1->arbre 2->mur
    BITMAP *img;
} t_obstacle;

typedef struct {
    int x, y, dx, dy, tx, ty, xcentre, ycentre, imgcourante, cptimg, tmpimg;
    BITMAP *img[NB_IMG_PERSOS];
}t_perso;

void initialisation_allegro();
t_spriteimmo initspriteimmo();
t_case init_case(int n, int i, int j);
t_obstacle init_obstacle(int n, int i, int j, int a);
void creer_fichier();
void charger_fichier(int tab_map[TAILLE_MAP][TAILLE_MAP]);
void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP]);
void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]);
void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);
void afficher_obstacles(BITMAP *buffer, t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);

#endif //HEADER_H
