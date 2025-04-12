#include <stdio.h>
#include <allegro.h>
#include <time.h>
#include <stdlib.h>

#include "map.h"
#include "map.c"

void menu(BITMAP *buffer) {
    t_spriteimmo logo=initspriteimmo();
    stretch_sprite(buffer, logo.img, 0, 0, buffer->w, buffer->h);
}


int main(void) {
    srand(time(NULL));
    initialisation_allegro();

    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    int tab_map[TAILLE_MAP][TAILLE_MAP];

    BITMAP *fond=load_bitmap("fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c);
    creer_obstacles(c, obs);
    /*
    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        menu(buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    rest(100);
    */
    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        afficher_map(buffer, c);
        //afficher_obstacles(buffer, obs);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();