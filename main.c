#include "header.h"

#include <time.h>
#include <stdlib.h>  
#include <stdio.h>
#include <allegro.h>


int main() {
    srand(time(NULL));
    initialisation_allegro();
    int aleatoire=0;
    int theme=-1;
    int nb_joueurs=2;
    int choix_joueurs[4];
    int equipe=0;
    menu(&aleatoire,&theme,&nb_joueurs,choix_joueurs,&equipe);
    int degats = 100;
    char nom_potion[20];
    int ligne_prec=-1;
    int colonne_prec=-1;
    int ligne_actu = -1;
    int colonne_actu = -1;

    t_perso p[TAILLE_MAP][TAILLE_MAP];
    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    t_potion pot[NB_POTION];
    int tab_map[TAILLE_MAP][TAILLE_MAP];

    BITMAP *inventaire = load_bitmap("Images\\inventaire.bmp", NULL);
    BITMAP *fond=load_bitmap("Images\\fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c, equipe, nb_joueurs);
    creer_obstacles(c, obs);
    placer_persos(c, p, equipe, choix_joueurs);
    creer_potion(pot, nom_potion);

    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        afficher_map(buffer, c);
        blit(inventaire, buffer, 0, 0, 0, SCREEN_H-inventaire->h, SCREEN_W,SCREEN_H);
        point_vie(buffer, pot, degats);
        souris_tab(c,buffer,&ligne_prec,&colonne_prec, &ligne_actu, &colonne_actu);
        chemin(c, p, 1, ligne_actu, colonne_actu, buffer);
        afficher_obstacles_persos(buffer, obs, p);
        afficher_inventaire(buffer, pot, degats);
        souris_potion(buffer, pot);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();
