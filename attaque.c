#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>

t_attaque init_attaque(char nom[MAX_NOM]) {
    t_attaque a;
    sprintf(a.nom, nom);
    a.degats=20;
    a.cout=2;
    a.chance=0.1;
    a.img=load_bitmap("Images\\epee.bmp", NULL);
    a.tx=a.img->w;
    a.ty=a.img->h;
    return a;
}

void equiper_attaque(t_perso p[NB_PERSOS], int nb_joueurs) {
    for (int i=0; i<nb_joueurs; i++) {
        char t[MAX_NOM];
        sprintf(t, "ATTAQUE J%d", p[i].num);
        p[i].att=init_attaque(t);
    }
}

void afficher_attaque(BITMAP *buffer, t_perso p) {
    rectfill(buffer, 51, 401, 54+p.att.tx, 404+p.att.ty, makecol(20, 20, 20));
    rect(buffer, 50, 400, 55+p.att.tx, 405+p.att.ty, makecol(200, 150, 60));
    blit(p.att.img, buffer, 0, 0, 53, 403, p.att.tx, p.att.ty);
    textout_centre_ex(buffer, font, "ATTAQUE CAC", 53+p.att.tx/2, 380, makecol(255, 128, 0), -1);
}

int gerer_attaque(BITMAP *buffer, t_perso p) {
    int r=0;
    if(mouse_x>50 && mouse_x<55+p.att.tx && mouse_y>400 && mouse_y<405+p.att.ty) {
        textout_ex(buffer, font, "ATTAQUE CAC survolee", 100, 120, makecol(255, 255, 255), -1);
        if (mouse_b&1)
            r=1;
    }
    return r;
}