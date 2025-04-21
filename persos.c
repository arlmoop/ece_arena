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
    b.tmpimg=4;
    b.dx=0, b.dy=0;
    b.equipe=0;
    b.classe=n;
    if (n==1) {
        b.img=load_bitmap("Images\\barbare_1.bmp", NULL);
    }
    else if (n==2) {
        b.img=load_bitmap("Images\\squelette_1.bmp", NULL);
    }
    else if (n==3) {
        b.img=load_bitmap("Images\\archere_1.bmp", NULL);
    }
    else if (n==4) {
        b.img=load_bitmap("Images\\geant_1.bmp", NULL);
    }
    else {
        b.img=load_bitmap("Images\\poubelle.bmp", NULL);// on l'affiche pas
    }
    b.tx=b.img->w;
    b.ty=b.img->h;
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

void deplacement (t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu) {
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (tour_perso == c[i][j].p && comparer_coord(p[tour_perso-1], ligne_actu, colonne_actu) == 1
                && mouse_b&1 && c[ligne_actu][colonne_actu].o==0 && c[ligne_actu][colonne_actu].p==0) {

                p[tour_perso-1].x=c[ligne_actu][colonne_actu].x;
                p[tour_perso-1].y=c[ligne_actu][colonne_actu].y-35;
                // eventuelement changer xcentre et ycentre
                p[tour_perso-1].ligne=ligne_actu;
                p[tour_perso-1].colonne=colonne_actu;

                c[ligne_actu][colonne_actu].p=tour_perso;
                c[i][j].p=0;
            }
        }
    }
}