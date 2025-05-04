#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_perso init_perso(int n, int x, int y){
    t_perso b;
    b.x=x, b.y=y;
    b.ligne=0, b.colonne=0;
    b.imgcourante=0;
    b.cptimg=0;
    b.tmpimg=2;
    b.dx=0, b.dy=0;
    b.equipe=0;
    b.classe=n;
    b.frames_restantes=0;
    b.nb_images=5;
    if (n==1) {
        for(int i=0;i<b.nb_images;i++){
            char filename[20];
            sprintf(filename,"Images\\barbare_%d.bmp",i);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else if (n==2) {
        b.img[0]=load_bitmap("Images\\squelette_1.bmp", NULL);
    }
    else if (n==3) {
        b.img[0]=load_bitmap("Images\\archere_1.bmp", NULL);
    }
    else if (n==4) {
        b.img[0]=load_bitmap("Images\\geant_1.bmp", NULL);
    }
    else {
        b.img[0]=load_bitmap("Images\\poubelle.bmp", NULL);// on l'affiche pas sa
    }
    b.tx=b.img[0]->w;
    b.ty=b.img[0]->h;
    b.xcentre=b.x+b.tx/2;
    b.ycentre=b.y+b.ty/2;
    return b;
}


void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], bool equipe, int choix_joueurs[]) {
    // SOLO
    if (equipe==0) {
        for(int i=0;i<TAILLE_MAP;i++){
            for(int j=0;j<TAILLE_MAP;j++) {
                if (c[i][j].type==NB_CASES+1) {
                    p[0]=init_perso(1, c[i][j].x, c[i][j].y-35);
                    c[i][j].p=1;
                    p[0].ligne=i, p[0].colonne=j;
                }
                else if (c[i][j].type==NB_CASES+2) {
                    p[1]=init_perso(2, c[i][j].x, c[i][j].y-35);
                    c[i][j].p=2;
                    p[1].ligne=i, p[1].colonne=j;
                }
                else if (c[i][j].type==NB_CASES+3) {
                    p[2]=init_perso(3, c[i][j].x, c[i][j].y-35);
                    c[i][j].p=3;
                    p[2].ligne=i, p[2].colonne=j;
                }
                else if (c[i][j].type==NB_CASES+4) {
                    p[3]=init_perso(4, c[i][j].x, c[i][j].y-35);
                    c[i][j].p=4;
                    p[3].ligne=i, p[3].colonne=j;
                }
            }
        }
    }
    // 2V2
    else {
        int compte=1;
        for(int i=0;i<TAILLE_MAP;i++){
            for(int j=0;j<TAILLE_MAP;j++) {
                if (c[i][j].type==NB_CASES+1) {
                    if (compte==1) {
                        p[0]=init_perso(1, c[i][j].x, c[i][j].y-35);
                        c[i][j].p=1;
                        p[0].ligne=i, p[0].colonne=j;
                        compte++;
                    }
                    else {
                        p[1]=init_perso(2, c[i][j].x, c[i][j].y-35);
                        c[i][j].p=2;
                        p[1].ligne=i, p[1].colonne=j;
                        compte--;
                    }
                }
                else if (c[i][j].type==NB_CASES+2) {
                    if (compte==1) {
                        p[2]=init_perso(3, c[i][j].x, c[i][j].y-35);
                        c[i][j].p=3;
                        p[2].ligne=i, p[2].colonne=j;
                        compte++;
                    }
                    else {
                        p[3]=init_perso(4, c[i][j].x, c[i][j].y-35);
                        c[i][j].p=4;
                        p[3].ligne=i, p[3].colonne=j;
                    }
                }
            }
        }
    }
}

void animer(t_perso *perso) {
    if (perso->anim_en_cours) {
        perso->x += perso->dx;
        perso->y += perso->dy;
        perso->frames_restantes--;

        perso->cptimg++;
        if (perso->cptimg >= perso->tmpimg) {
            perso->cptimg = 0;
            perso->imgcourante++;
            if (perso->imgcourante >= perso->nb_images) {
                perso->imgcourante = 0;
            }
        }

        if (perso->frames_restantes <= 0) {
            perso->anim_en_cours = 0;
            perso->dx = 0;
            perso->dy = 0;

            perso->xcentre = perso->x + perso->tx / 2;
            perso->ycentre = perso->y + perso->ty / 2;
        }
    }
    rest(25);
}

void deplacement(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu) {
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (tour_perso == c[i][j].p && mouse_b & 1 && chemin_valide(c, p, tour_perso, ligne_actu, colonne_actu) && !p[tour_perso - 1].anim_en_cours)
            {
                int x_depart = p[tour_perso - 1].x;
                int y_depart = p[tour_perso - 1].y;
                int x_arrivee = c[ligne_actu][colonne_actu].x;
                int y_arrivee = c[ligne_actu][colonne_actu].y - 35;

                int nb_frames = 10;

                p[tour_perso - 1].dx = (x_arrivee - x_depart) / nb_frames;
                p[tour_perso - 1].dy = (y_arrivee - y_depart) / nb_frames;
                p[tour_perso - 1].frames_restantes = nb_frames;
                p[tour_perso - 1].anim_en_cours = 1;

                p[tour_perso - 1].ligne = ligne_actu;
                p[tour_perso - 1].colonne = colonne_actu;

                c[ligne_actu][colonne_actu].p = tour_perso;
                c[i][j].p = 0;
            }
        }
    }
}
