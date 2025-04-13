#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_perso init_perso(int equipe, int i, int j){
    t_perso b;
    b.x=i, b.y=j;
    b.imgcourante=0;
    b.cptimg=0;
    b.tmpimg=4;
    b.dx=0, b.dy=0;
    b.equipe=equipe;
    b.classe=0;
    for(int k=0; k<NB_IMG_PERSOS; k++) {
        char filename[20];
        sprintf(filename, "Images\\barbare_%d.bmp", k+1);
        b.img[k]=load_bitmap(filename, NULL);
    }
    b.tx=b.img[0]->w;
    b.ty=b.img[0]->h;
    b.xcentre=b.x+b.tx/2;
    b.ycentre=b.y+b.ty/2;
    return b;
}

void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int equipe) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            if (c[i][j].type==NB_CASES+equipe) {
                p[equipe-1]=init_perso(equipe, c[i][j].x, c[i][j].y);
            }
        }
    }
}

void afficher_persos(BITMAP *buffer, t_perso p[NB_PERSOS]) {
    for(int i=0;i<NB_PERSOS;i++){
        draw_sprite(buffer, p[i].img[0], p[i].x, p[i].y);
    }
}