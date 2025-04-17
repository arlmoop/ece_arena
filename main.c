#include "header.h"

#include <time.h>
#include <stdlib.h>  
#include <stdio.h>
#include <allegro.h>


void initialisation_allegro(){
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0)!=0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen);
}

int main() {
    srand(time(NULL));
    initialisation_allegro();

    int degats = 100;
    char nom_potion[20];

    t_perso p[TAILLE_MAP][TAILLE_MAP];
    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    t_potion pot[NB_POTION];
    int tab_map[TAILLE_MAP][TAILLE_MAP];
    int equipe=0;// a enlever quand menu // 0 non 1 oui

    BITMAP *inventaire = load_bitmap("Images\\inventaire.bmp", NULL);
    BITMAP *fond=load_bitmap("Images\\fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c, equipe);
    creer_obstacles(c, obs);
    placer_persos(c, p, equipe);
    creer_potion(pot, nom_potion);

    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        afficher_map(buffer, c);
        blit(inventaire, buffer, 0, 0, 0, SCREEN_H-inventaire->h, SCREEN_W,SCREEN_H);
        point_vie(buffer, pot, degats);
        afficher_obstacles_persos(buffer, obs, p);
        afficher_inventaire(buffer, pot, degats);
        souris_potion(buffer, pot);
        souris_tab(c,buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();
