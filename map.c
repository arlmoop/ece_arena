#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_case init_case(int n, int x, int y) {
    t_case c;
    c.x=x, c.y=y;
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

t_obstacle init_obstacle(int n, int x, int y) {
    t_obstacle obs;
    obs.x=x, obs.y=y;
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
            if(c[i][j].type==1 && 1+rand()%100>100-PRCNT_OBS) {
                obs[i][j].e=1;
                c[i][j].o=1;
            }
        }
    }
}

void afficher_obstacles_persos(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS]) {
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (obs[i][j].e == 1) {
                draw_sprite(buffer, obs[i][j].img, obs[i][j].x, obs[i][j].y);
            }

            if (c[i][j].p != 0) {
                int classe = c[i][j].p;

                for (int k = 0; k < NB_PERSOS; k++) {
                    if (p[k].classe == classe) {
                        int frame = p[k].imgcourante % p[k].nb_images;
                        draw_sprite(buffer, p[k].img[frame], p[k].x, p[k].y);
                        break;
                    }
                }//sa
            }
        }
    }
}

int point_dans_losange(t_case c) {
    float dx = abs(mouse_x - c.xcentre);
    float dy = abs(mouse_y - c.ycentre_losange);
    return (dx / (c.tx / 2.0) + dy / (c.ty / 4.0)) <= 1;
}

void remplir_losange(t_case c, BITMAP * buffer, int couleur) {
    for (int i=0; i<=c.tx/4+1; i++) {
        hline(buffer, c.x+13*i/7, c.ycentre_losange-i, c.x+c.tx-13*i/7, couleur);
        hline(buffer, c.x+13*i/7, c.ycentre_losange+i, c.x+c.tx-13*i/7, couleur);
    }
}

void souris_tab(t_case c[TAILLE_MAP][TAILLE_MAP], BITMAP *buffer, int *ligne_prec, int *colonne_prec, int *ligne_actu, int*colonne_actu) {
    *ligne_actu = -1;
    *colonne_actu = -1;
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (point_dans_losange(c[i][j])) {
                *ligne_actu = i;
                *colonne_actu = j;
                break;
            }
        }
        if (*ligne_actu != -1) break;
    }
    if (*ligne_actu != *ligne_prec || *colonne_actu != *colonne_prec) {
        for (int i = 0; i < TAILLE_MAP; i++) {
            for (int j = 0; j < TAILLE_MAP; j++) {
                c[i][j].r = 0;
            }
        }
        if (*ligne_actu != -1 && *colonne_actu != -1) {
            c[*ligne_actu][*colonne_actu].r = 1;
        }
        *ligne_prec = *ligne_actu;
        *colonne_prec = *colonne_actu;
    }
    if (*ligne_prec != -1 && *colonne_prec != -1 && c[*ligne_prec][*colonne_prec].r == 1) {
        remplir_losange(c[*ligne_prec][*colonne_prec], buffer, makecol(255, 0, 0));
    }
}

int comparer_coord(t_perso p, int ligne_actu, int colonne_actu) {
    int r=0;
    for(int i=0; i<=PM; i++) {
        if(abs(p.ligne-ligne_actu) + abs(p.colonne-colonne_actu)<=PM) {
            r=1;
        }
    }
    return r;
}

int chemin_valide(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu) {
    if (c[ligne_actu][colonne_actu].o != 0 || c[ligne_actu][colonne_actu].p != 0)
        return 0;

    if (comparer_coord(p[tour_perso - 1], ligne_actu, colonne_actu) != 1)
        return 0;

    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (c[i][j].p == tour_perso) {
                return 1;
            }
        }
    }

    return 0;
}

void chemin(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu, BITMAP* buffer) {
    if (!chemin_valide(c, p, tour_perso, ligne_actu, colonne_actu)){
            return;
    }
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (tour_perso == c[i][j].p) {
                int ligne_finale = i;
                if (i < ligne_actu) {
                    for (int k = i + 1; k <= ligne_actu; k++) {
                        remplir_losange(c[k][j], buffer, makecol(100, 255, 100));
                    }
                    ligne_finale = ligne_actu;
                } else if (i > ligne_actu) {
                    for (int k = i - 1; k >= ligne_actu; k--) {
                        remplir_losange(c[k][j], buffer, makecol(100, 255, 100));
                    }
                    ligne_finale = ligne_actu;
                }
                if (j < colonne_actu) {
                    for (int l = j + 1; l <= colonne_actu; l++) {
                        remplir_losange(c[ligne_finale][l], buffer, makecol(100, 255, 100));
                    }
                } else if (j > colonne_actu) {
                    for (int l = j - 1; l >= colonne_actu; l--) {
                        remplir_losange(c[ligne_finale][l], buffer, makecol(100, 255, 100));
                    }
                }
            }
        }
    }
}

void afficher_pause(BITMAP *buffer, int *compteur) {

    if (mouse_b&1 && mouse_x<SCREEN_W/18 && mouse_y<SCREEN_H/15 && *compteur==0) {
        *compteur=1;
    }

    if (mouse_b&1 && mouse_x>SCREEN_W-17*SCREEN_W/18 && mouse_x<SCREEN_W-15*SCREEN_W/18 && mouse_y>SCREEN_H-17*SCREEN_H/18 && mouse_y<SCREEN_H-15*SCREEN_H/18 && *compteur==1) {
        *compteur=0;
    }

    if (*compteur==1) {
        rectfill(buffer, 17*SCREEN_W/18, 17*SCREEN_H/18, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, makecol(180, 180, 100));
        rectfill(buffer, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, SCREEN_W-15*SCREEN_W/18, SCREEN_H-15*SCREEN_H/18, makecol(200, 50, 150));
    }
    if (*compteur==0) {
        rectfill(buffer, 0, 0, SCREEN_W/18, SCREEN_H/15, makecol(50, 50, 50));
        rectfill(buffer, 10, 10, SCREEN_W/36-3, SCREEN_H/15-10, makecol(200, 200, 200));
        rectfill(buffer, SCREEN_W/18-10, 10, SCREEN_W/36+3, SCREEN_H/15-10, makecol(200, 200, 200));
    }

}
