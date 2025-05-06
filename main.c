#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>

void initialisation_allegro() {
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

    t_perso p[NB_PERSOS];
    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    t_potion pot[NB_POTION];
    int tab_map[TAILLE_MAP][TAILLE_MAP];
    int tour_perso=1;

    BITMAP *inventaire = load_bitmap("inventaire.bmp", NULL);
    BITMAP *fond=load_bitmap("fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c, equipe, nb_joueurs);
    creer_obstacles(c, obs);
    placer_persos(c, p, equipe, choix_joueurs);
    creer_potion(pot, nom_potion);
    t_coord chemin[20];
    int nb_etapes = 0;
    int etape_courante = 0;
    while (!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        afficher_map(buffer, c);
        afficher_obstacles_persos(buffer, c, obs, p);

        if (!p[tour_perso].anim_en_cours) {
            souris_tab(c, buffer, &ligne_prec, &colonne_prec, &ligne_actu, &colonne_actu);
            afficher_chemin(c, p, tour_perso, ligne_actu, colonne_actu, buffer);
            deplacement(c, p, tour_perso, ligne_actu, colonne_actu, chemin, &nb_etapes, &etape_courante);
        } else {
            animer(&p[tour_perso], chemin, &etape_courante, nb_etapes);
        }

        blit(inventaire, buffer, 0, 0, 0, SCREEN_H - inventaire->h, SCREEN_W, SCREEN_H);
        point_vie(buffer, pot, degats);
        afficher_inventaire(buffer, pot, degats);
        souris_potion(buffer, pot, c);

        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();
