#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_perso init_perso(int n, int i, int j){
    t_perso b;
    b.x=i, b.y=j;
    b.imgcourante=0;
    b.cptimg=0;
    b.tmpimg=4;
    b.dx=0, b.dy=0;
    b.equipe=0;
    b.classe=n;
    b.e=0;
    if (n==1) {
        b.img=load_bitmap("Images\\barbare_1.bmp", NULL);
    }
    else {
        b.img=load_bitmap("Images\\squelette_1.bmp", NULL);
    }
    b.tx=b.img->w;
    b.ty=b.img->h;
    b.xcentre=b.x+b.tx/2;
    b.ycentre=b.y+b.ty/2;
    return b;
}

void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[TAILLE_MAP][TAILLE_MAP], bool equipe) {
    int num=1;
    int paire=1;
        for(int i=0;i<TAILLE_MAP;i++){
            for(int j=0;j<TAILLE_MAP;j++) {
                p[i][j]=init_perso(num, c[i][j].x, c[i][j].y-35);
                if (c[i][j].type==NB_CASES+num) {
                    p[i][j].e=1;
                    c[i][j].p=1;
                    num++;
                    if (paire%2!=0 && equipe==1) {
                        num--;
                    }
                    paire++;
                }
            }
        }
}
