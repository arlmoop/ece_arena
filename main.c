#include "header.h"

#include <time.h>
#include <stdlib.h>  
#include <stdio.h>
#include <allegro.h>


int main() {
    srand(time(NULL));
    initialisation_allegro();

    int degats = 100;

    t_perso p[NB_PERSOS];
    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    int tab_map[TAILLE_MAP][TAILLE_MAP];

    BITMAP *fond=load_bitmap("Images\\fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c, 1);
    creer_obstacles(c, obs);
    placer_persos(c, p);


    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        afficher_map(buffer, c);
        afficher_inventaire(buffer,p, degats);
        souris(buffer,p);
        afficher_obstacles_persos(buffer, c, obs, p);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();
