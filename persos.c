#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_perso init_perso(int n, int x, int y) {
    t_perso b;
    b.x=x, b.y=y;
    b.ligne=0, b.colonne=0;
    b.imgcourante=0;
    b.cptimg=0;
    b.tmpimg=4;
    b.dx=0, b.dy=0;
    b.classe=n;
    b.cl=-1;
    b.frames_restantes=0;
    b.nb_images=5;
    b.etape_courante=0;
    sprintf(b.nom, "NULL");
    if (n==1) {
        for(int i=0;i<b.nb_images;i++){
            char filename[30];
            sprintf(filename,"Images\\nita_%d.bmp",i+1);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else if (n==2) {
        for(int i=0;i<b.nb_images;i++){
            char filename[30];
            sprintf(filename,"Images\\leon_%d.bmp",i+1);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else if (n==3) {
        for(int i=0;i<b.nb_images;i++){
            char filename[30];
            sprintf(filename,"Images\\byron_%d.bmp",i+1);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else if (n==4) {
        for(int i=0;i<b.nb_images;i++){
            char filename[30];
            sprintf(filename,"Images\\bilie_%d.bmp",i+1);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else {
        for(int i=0;i<b.nb_images;i++){
            char filename[30];
            sprintf(filename,"Images\\bilie_%d.bmp",i+1);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    b.tx=b.img[0]->w;
    b.ty=b.img[0]->h;
    b.xcentre=b.x+b.tx/2;
    b.ycentre=b.y+b.ty/2;
    b.anim_en_cours=0;
    b.pa=5, b.pm=PM, b.pv=100;
    b.num=0;
    b.mort=0;
    b.equipe=-1;
    return b;
}

void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int choix_joueurs[]) {
    int b=0;
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++) {
            if (c[i][j].type>NB_CASES) {
                p[b]=init_perso(choix_joueurs[b], c[i][j].x, c[i][j].y-35);
                c[i][j].p=choix_joueurs[b];
                c[i][j].num_joueur=b+1;
                p[b].ligne=i, p[b].colonne=j;
                p[b].num=b+1;
                sprintf(p[b].nom, "Joueur %d", b+1);
                if (p[b].num==1 || p[b].num==2) {
                    p[b].equipe=1;
                } else if (p[b].num==3 || p[b].num==4) {
                    p[b].equipe=2;
                }
                b++;
            }
        }
    }
}

void animer_statique(t_perso perso[NB_PERSOS], int nb_joueurs){
    for(int i=0;i<nb_joueurs;i++){
        if (!perso[i].anim_en_cours){
            perso[i].cptimg++;
            if (perso[i].cptimg >= perso[i].tmpimg) {
                perso[i].cptimg = 0;
                perso[i].imgcourante = (perso[i].imgcourante + 1) % perso[i].nb_images;
            }
        }
    }
}

void animer(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso *perso, t_coord chemin[], bool *valider_pm, int *distance) {
    if (perso->anim_en_cours && perso->etape_courante < *distance) {
            if(perso->etape_courante!=0){
                c[chemin[perso->etape_courante-1].ligne][chemin[perso->etape_courante-1].colonne].num_joueur = 0;
            }
            if (perso->ligne < chemin[perso->etape_courante].ligne) {
                perso->x -= 21;
                perso->y += 12;
                perso->ligne=perso->ligne+1;
            }
            if (perso->ligne > chemin[perso->etape_courante].ligne) {
                perso->x += 21;
                perso->y -= 12;
                perso->ligne=perso->ligne-1;
            }


            if (perso->colonne < chemin[perso->etape_courante].colonne) {
                perso->x += 21;
                perso->y += 12;
                perso->colonne=perso->colonne+1;
            }
            if (perso->colonne > chemin[perso->etape_courante].colonne) {
                perso->x -= 21;
                perso->y -= 12;
                perso->colonne=perso->colonne-1;
            }

            perso->etape_courante++;
            c[chemin[perso->etape_courante].ligne][chemin[perso->etape_courante].colonne].num_joueur = perso->num;
        if (perso->etape_courante >= *distance) {
            perso->anim_en_cours = 0;
            *valider_pm = 1;
            perso->pm -= *distance;
            perso->etape_courante = 0;

            perso->xcentre = perso->x + perso->tx / 2;
            perso->ycentre = perso->y + perso->ty / 2;
        }
        /*perso->cptimg++;
        if (perso->cptimg >= perso->tmpimg) {
            perso->cptimg = 0;
            perso->imgcourante = (perso->imgcourante + 1) % perso->nb_images;
        }*/

    }
    rest(100);
}


void deplacement(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], t_coord chemin[], int tour_perso, int ligne_actu, int colonne_actu, int *distance, int *deplacement_valide) {
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (tour_perso == c[i][j].num_joueur && mouse_b & 1 && *deplacement_valide)
            {
                int nb_frames = 10;
                //p[tour_perso - 1].dx =(chemin[*distance-1].ligne - p[tour_perso-1].ligne);
                //p[tour_perso - 1].dy =(chemin[*distance-1].colonne - p[tour_perso-1].colonne);
                c[chemin[*distance-1].ligne][chemin[*distance-1].colonne].p=c[i][j].p;
                p[tour_perso - 1].frames_restantes = nb_frames;
                p[tour_perso - 1].anim_en_cours = 1;
                c[i][j].num_joueur = 0;
                c[i][j].p=0;
            }
        }
    }
}

void gerer_tours(int *tour_perso, t_perso *p, bool *valider_pm, bool *valider_pa, bool *passer_tour,
    int nb_joueurs, double *secondes, clock_t *depart, clock_t *tps_pause, int *ca) {
    if ((*valider_pm && *valider_pa) || *passer_tour || *secondes>=TEMPS_TOUR) {
        if(*tour_perso<nb_joueurs)
            (*tour_perso)++;
        else if(*tour_perso==nb_joueurs)
            *tour_perso=1;

        *valider_pm=0;
        *valider_pa=0;
        *passer_tour=0;
        *ca=0;

        p->pm=PM;
        p->pa=PA;

        *depart=clock();
        *tps_pause=0;
    }
}

void passer(bool *passer_tour, BITMAP *buffer) {
    rectfill(buffer, 600, 400, 738, 420, makecol(20, 20, 20));
    rect(buffer, 600, 400, 738, 420, makecol(200, 150, 60));
    textout_ex(buffer, font, "TERMINER LE TOUR", 605, 407, makecol(200, 150, 60), -1);
    if(clic_gauche(600, 400, 738, 420)) {
        *passer_tour=1;
        while(mouse_b & 1);
    }
}

void val_pa(bool *valider_pa, t_perso p) {
    if(p.pa==0)
        *valider_pa=1;
}

void barre_pv(t_perso p, BITMAP *buffer) {
    char t[30];
    rectfill(buffer, p.xcentre-25, p.y-15, p.xcentre+25, p.y, makecol(250, 250, 250));
    rectfill(buffer, p.xcentre-25, p.y-15, p.xcentre-25+p.pv/2, p.y, makecol(200, 50, 50));
    rect(buffer, p.xcentre-25, p.y-15, p.xcentre+25, p.y, makecol(50, 50, 50));
    sprintf(t, "%d", p.pv);
    textout_centre_ex(buffer, font, t, p.xcentre, p.y-10, makecol(128, 255, 128), -1);
}

void barres (int nb_persos, t_perso p[NB_PERSOS], BITMAP *buffer) {
    for (int i=0; i<nb_persos; i++) {
        if(p[i].mort==0)
            barre_pv(p[i], buffer);
    }
}

void gerer_mort(t_perso p[NB_PERSOS], int nb_joueurs, int *nb_morts) {
    for(int i=0; i<nb_joueurs; i++) {
        if(p[i].mort==0) {
            if(p[i].pv<=0) {
                p[i].cl=nb_joueurs-*nb_morts;
                (*nb_morts)++;
                p[i].mort=1;
            }
        }
    }
}

void aff_morts(t_perso *p, bool *passer_tour) {
    if(p->mort==1) {
        *passer_tour=1;
    }
}

void fin(int nb_joueurs, int nb_morts, int *compteur, t_perso p[NB_PERSOS]) {
    if(nb_morts==nb_joueurs-1)
        *compteur=3;
    for(int i=0; i<nb_joueurs; i++) {
        if(p[i].mort==0) {
            p[i].cl=1;
        }
    }
}
