#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>

t_attaque init_attaque(char nom[MAX_NOM]) {
    t_attaque a;
    sprintf(a.nom, nom);
    a.degats=100;
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
    textout_centre_ex(buffer, font, "ATTAQUE CAC", 53+p.att.tx/2, 350, makecol(255, 128, 0), -1);
    textprintf_centre_ex(buffer, font, 53+p.att.tx/2, 370, makecol(255, 0, 128), -1,
        "Degats : %d", p.att.degats);
    textprintf_centre_ex(buffer, font, 53+p.att.tx/2, 380, makecol(255, 0, 128), -1,
        "Chances d'echec : %.0f/10", p.att.chance*10);
}

int gerer_attaque(BITMAP *buffer, t_perso p) {
    int r=0;
    if(mouse_x>50 && mouse_x<55+p.att.tx && mouse_y>400 && mouse_y<405+p.att.ty) {
        //textout_ex(buffer, font, "ATTAQUE CAC survolee", 100, 120, makecol(255, 255, 255), -1);
        if (mouse_b&1)
            r=1;
    }
    return r;
}

void attaques(BITMAP *buffer, t_perso p[NB_PERSOS], int nb_joueurs, int tour_perso, int *ca, bool equipe) {

    if(gerer_attaque(buffer, p[tour_perso-1]) && *ca==0) {
        *ca=1;
    }
    if(*ca==1) {
        textout_ex(buffer, font, "Choisissez un joueur a attquer", 100, 140, makecol(255, 255, 255), -1);
        rectfill(buffer, 50, 320, 55+p[tour_perso-1].att.tx, 340, makecol(20, 20, 20));
        rect(buffer, 50, 320, 55+p[tour_perso-1].att.tx, 340, makecol(200, 150, 60));
        textout_centre_ex(buffer, font, "ANNULER", 53+p[tour_perso-1].att.tx/2, 327, makecol(200, 150, 60), -1);
    }

    if(clic_gauche(50, 320, 55+p[tour_perso-1].att.tx, 340) && *ca==1) {
        *ca=0;
    }
    for(int i=0; i<nb_joueurs; i++) {
        if(abs(p[tour_perso-1].ligne-p[i].ligne)+abs(p[tour_perso-1].colonne-p[i].colonne)==1 && *ca==1) {
            if(clic_gauche(p[i].x, p[i].y, p[i].x+p[i].tx, p[i].y+p[i].ty)
                && p[tour_perso-1].pa>=p[tour_perso-1].att.cout) {
                if(equipe==0 || (equipe==1 && p[tour_perso-1].equipe!=p[i].equipe)) {
                    if(1+rand()%10>p[tour_perso-1].att.chance*10) {
                        //PV
                        if(p[i].pv>=p[tour_perso-1].att.degats) {
                            p[i].pv-=p[tour_perso-1].att.degats;
                        }
                        else {
                            p[i].pv=0;
                        }

                        *ca=0;
                    }
                    else {
                        *ca=2;
                    }
                    //PA
                    p[tour_perso-1].pa-=p[tour_perso-1].att.cout;

                    while(mouse_b & 1);
                }
                else {
                    *ca=3;
                }
            }
        }
    }
    if(*ca==2) {
        textout_ex(buffer, font, "ATTAQUE MANQUEE", 100, 140, makecol(255, 255, 255), -1);
    }
    if(*ca==3) {
        textout_ex(buffer, font, "ATTAQUE IMPOSSIBLE", 100, 140, makecol(255, 255, 255), -1);
    }
}
