#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_case init_case(int n, int i, int j) {
    t_case c;
    c.x=i, c.y=j;
    c.o=0;
    c.p=0;
    c.r=0;
    if (n==1) {
        c.img=load_bitmap("Images\\bloc_glace.bmp", NULL);
        c.type=2;
    }
    else if (n>1 && n<=NB_CASES){
        c.img=load_bitmap("Images\\bloc_terre.bmp", NULL);
        c.type=1;
    }
    else {
        char filename[20];
        sprintf(filename, "Images\\spawn_%d.bmp", n-NB_CASES);
        c.img=load_bitmap(filename, NULL);
        c.type=n;
    }
    c.tx=c.img->w;
    c.ty=c.img->h;
    c.xcentre=c.x+c.tx/2;
    c.ycentre=c.y+c.ty/2;
    c.ycentre_losange=c.y+c.ty/4;
    return c;
}

t_obstacle init_obstacle(int n, int i, int j) {
    t_obstacle obs;
    obs.x=i, obs.y=j;
    obs.e=0;
    obs.a=0;
    if (n==1) {
        obs.img=load_bitmap("Images\\tronc.bmp", NULL);
    }
    else if (n==2) {
        obs.img=load_bitmap("Images\\gem_box.bmp", NULL);
    }
    else if (n==3) {
        obs.img=load_bitmap("Images\\pierre.bmp", NULL);
    }
    else {
        obs.img=load_bitmap("Images\\sapin.bmp", NULL);
    }
    obs.tx=obs.img->w;
    obs.ty=obs.img->h;
    obs.xcentre=obs.x+obs.tx/2;
    obs.ycentre=obs.y+obs.ty/2;
    return obs;
}

void creer_fichier() {
    FILE* fichier=fopen("telechargement_map.txt", "w");
    if(fichier==NULL) {
        printf("Erreur\n");
        exit(1);
    }
    for(int i=0;i<TAILLE_MAP;i++) {
        for(int j=0;j<TAILLE_MAP;j++) {
            fprintf(fichier,"%d ", 1+rand()%NB_CASES);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

void charger_fichier(int tab_map[TAILLE_MAP][TAILLE_MAP]) {
    FILE* fic=fopen("telechargement_map.txt", "r");
    if(fic==NULL) {
        printf("Erreur\n");
        exit(1);
    }
    for(int i=0;i<TAILLE_MAP;i++) {
        for(int j=0;j<TAILLE_MAP;j++) {
            fscanf(fic,"%d", &tab_map[i][j]);
        }
    }
}

void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], bool equipe, int nb_joueurs){
    t_case case1=init_case(1,0,0);
    int m;
    // 2V2
    if (equipe==1) {
        for(int i=0;i<TAILLE_MAP;i++){
            for(int j=0;j<TAILLE_MAP;j++){
                if (i==0 && (j==TAILLE_MAP/2+1 || j==TAILLE_MAP/2-1)) {
                    m=NB_CASES+1;
                }
                else if (i==TAILLE_MAP-1 && (j==TAILLE_MAP/2+1 || j==TAILLE_MAP/2-1)) {
                    m=NB_CASES+2;
                }
                else {
                    m=tab_map[i][j];
                }
                c[i][j]=init_case(m,SCREEN_W/2-case1.tx/2+(case1.tx/2)*j-(case1.tx/2)*i,Y_DEPART+(case1.y+12)*j+(case1.y+12)*i);
            }
        }
    }
    // SOLO
    else {
        for(int i=0;i<TAILLE_MAP;i++) {
            for(int j=0;j<TAILLE_MAP;j++) {
                m=tab_map[i][j];
                c[i][j]=init_case(m,SCREEN_W/2-case1.tx/2+(case1.tx/2)*j-(case1.tx/2)*i,Y_DEPART+(case1.y+12)*j+(case1.y+12)*i);
            }
        }
        for(int k=0; k<nb_joueurs; k++) {
            m=NB_CASES+1+k;
            int i=rand()%TAILLE_MAP, j=rand()%TAILLE_MAP;
            if (c[i][j].type<=NB_CASES) {
                c[i][j]=init_case(m,SCREEN_W/2-case1.tx/2+(case1.tx/2)*j-(case1.tx/2)*i,Y_DEPART+(case1.y+12)*j+(case1.y+12)*i);
            }
            else {
                k--;
            }
        }
    }
}

void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            draw_sprite(buffer,c[i][j].img, c[i][j].x, c[i][j].y);
        }
    }
}

void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            obs[i][j]=init_obstacle(1+rand()%NB_OBS, c[i][j].x, c[i][j].y-20);
            if(c[i][j].type==1 && 1+rand()%100>100-PRCNT_OBS && c[i][j].p==0) {
                obs[i][j].e=1;
                c[i][j].o=1;
            }
        }
    }
}

void afficher_obstacles_persos(BITMAP *buffer, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[TAILLE_MAP][TAILLE_MAP]) {
    for (int i=0; i<TAILLE_MAP; i++) {
        for (int j=0; j<TAILLE_MAP; j++) {
            if (obs[i][j].e==1) {
                draw_sprite(buffer, obs[i][j].img, obs[i][j].x, obs[i][j].y);
            }
            if (p[i][j].e==1) {
                draw_sprite(buffer, p[i][j].img, p[i][j].x, p[i][j].y);
            }
        }
    }
}
int point_dans_losange(t_case c) {
    float dx = abs(mouse_x - c.xcentre);
    float dy = abs(mouse_y - c.ycentre_losange);
    return (dx / (c.tx / 2.0) + dy / (c.ty / 4.0)) <= 1;
}

void remplir_losange(t_case c, BITMAP * buffer) {
    for (int i=0; i<=c.tx/4+1; i++) {
        hline(buffer, c.x+13*i/7, c.ycentre_losange-i, c.x+c.tx-13*i/7, makecol(255, 0, 0));
        hline(buffer, c.x+13*i/7, c.ycentre_losange+i, c.x+c.tx-13*i/7, makecol(255, 0, 0));
    }
}

void souris_tab(t_case c[TAILLE_MAP][TAILLE_MAP], BITMAP * buffer) {
    int drapeau = 0;
    for(int i=0; i<TAILLE_MAP && !drapeau; i++) {
        for(int j=0; j<TAILLE_MAP && !drapeau; j++) {
            if (point_dans_losange(c[i][j])) {
                c[i][j].r = 1;
                remplir_losange(c[i][j], buffer);
                drapeau = 1;
            } else {
                c[i][j].r = 0;
            }
        }
    }
}
