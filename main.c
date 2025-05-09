#include "header.h"

#include <time.h>
#include <stdlib.h>  
#include <stdio.h>
#include <allegro.h>


int main() {//sa
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
    int compteur=0;

    t_perso p[NB_PERSOS];
    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    t_potion pot[NB_POTION];
    int tab_map[TAILLE_MAP][TAILLE_MAP];
    int tour_perso=1;

    BITMAP *inventaire = load_bitmap("Images\\inventaire.bmp", NULL);
    BITMAP *fond=load_bitmap("Images\\fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    BITMAP* imagesss = load_bitmap("Images\\serpent3.bmp", NULL);
    magenta(imagesss, 200, 50, 200);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c, equipe, nb_joueurs);
    creer_obstacles(c, obs);
    placer_persos(c, p, equipe, choix_joueurs);
    equiper_potion(p, nom_potion);

    while (!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        afficher_map(buffer, c);
        blit(inventaire, buffer, 0, 0, 0, SCREEN_H - inventaire->h, SCREEN_W, SCREEN_H);
        afficher_inventaire(buffer, degats, p, 1);
        affichage_potions(buffer, p, c, 1);
        point_vie(buffer, degats);
        souris_tab(c, buffer, &ligne_prec, &colonne_prec, &ligne_actu, &colonne_actu);
        chemin(c, p, tour_perso, ligne_actu, colonne_actu, buffer);
        deplacement(c, p, tour_perso, ligne_actu, colonne_actu);
        if (p[tour_perso - 1].anim_en_cours) {
            animer(&p[tour_perso - 1]);
        }
        afficher_obstacles_persos(buffer, c, obs, p);
        afficher_pause(buffer, &compteur);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();
