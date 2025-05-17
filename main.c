#include "header.h"

#include <time.h>
#include <stdlib.h>  
#include <stdio.h>
#include <allegro.h>


int main() {//sa
    srand(time(NULL));
    initialisation_allegro();

    while(!key[KEY_ESC]) {

        int aleatoire=0;
        int theme=-1;
        int nb_joueurs=2;
        int choix_joueurs[NB_PERSOS];
        int equipe=0;

        int m=menu(&aleatoire,&theme,&nb_joueurs,choix_joueurs,&equipe);

        bool quitter=0;
        double secondes;
        int degats = 100;
        char nom_potion[20];
        int ligne_prec=-1;
        int colonne_prec=-1;
        int ligne_actu = -1;
        int colonne_actu = -1;
        int compteur=0;
        bool valider_pm=0;
        bool valider_pa=0;
        bool passer_tour=0;
        int tour_perso=1+rand()%nb_joueurs;
        int tour_depart=tour_perso;
        int distance=0;
        int numero_potion=0;
        int n=5;

        t_perso p[NB_PERSOS];
        t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP];
        t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
        t_case c[TAILLE_MAP][TAILLE_MAP];
        int tab_map[TAILLE_MAP][TAILLE_MAP];
        int tab_attaque[TAILLE_MAP][TAILLE_MAP];
        int tab_aleatoire[TAILLE_MAP][TAILLE_MAP];

        BITMAP *inventaire = load_bitmap("Images\\inventaire.bmp", NULL);
        BITMAP *fond=load_bitmap("Images\\fond2.bmp", NULL);
        BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
        BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
        stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

        if(m==1) {
            nouvelle_partie(buffer, tab_map, tab_obs, c, equipe, obs, p, nb_joueurs, choix_joueurs, nom_potion);
        }
        else if(m==3) {
            sauvegarde(tab_map, tab_obs, c, equipe, obs, p, nb_joueurs, choix_joueurs, nom_potion);
        }

        clock_t depart=clock();
        clock_t pause;
        clock_t tps_pause=0;

        int changement_tour = tour_perso;
        tableau_aleatoire(tab_aleatoire, n);

        while (quitter==0) {
            if(compteur==0) {
                clear_bitmap(buffer);
                blit(decor, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                afficher_map(buffer, c);
                blit(inventaire, buffer, 0, 0, 0, SCREEN_H - inventaire->h, SCREEN_W, SCREEN_H);

                if (tour_perso != changement_tour) { //Pour avoir des attaques aleatoires
                    tableau_aleatoire(tab_aleatoire, n);
                    changement_tour = tour_perso;
                }

                affichage_potions(buffer, p, c, tab_attaque, tab_aleatoire, tour_perso, numero_potion);
                souris_tab(c, buffer, &ligne_prec, &colonne_prec, &ligne_actu, &colonne_actu);
                afficher_chemin(c, p, tour_perso, ligne_actu, colonne_actu, &distance, buffer);
                deplacement(c, p, tour_perso, ligne_actu, colonne_actu, &distance);

                if (p[tour_perso - 1].anim_en_cours) {
                    animer(&p[tour_perso - 1], &valider_pm, &distance);
                }

                afficher_obstacles_persos(buffer, c, obs, p);
                passer(&passer_tour, buffer);
                val_pa(&valider_pa, buffer);
                gerer_tours(&tour_perso, &p[tour_perso-1], &valider_pm, &valider_pa, &passer_tour, nb_joueurs, &secondes, &depart, &tps_pause);
                afficher_infos(&secondes, depart, tps_pause, p, buffer, tour_perso, nb_joueurs);
                barres(nb_joueurs, p, buffer);
            }

            afficher_pause(tab_obs, buffer, &compteur, &degats, nom_potion, &ligne_prec, &ligne_actu,
                &colonne_prec, &colonne_actu, &valider_pm, &valider_pa, &passer_tour,
                &tour_perso, &nb_joueurs, &distance, tab_map, c, &equipe, obs, p,
                choix_joueurs, &tour_depart, &depart, &pause, &tps_pause, &quitter);
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();