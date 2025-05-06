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
    b.nb_images=20;
    if (n==1) {
        for(int i=0;i<b.nb_images;i++){
            char filename[20];
            sprintf(filename,"barbare_%d.bmp",i);
            b.img[i]=load_bitmap(filename,NULL);
        }
    }
    else if (n==2) {
        b.img[0]=load_bitmap("squelette_1.bmp", NULL);
    }
    else if (n==3) {
        b.img[0]=load_bitmap("archere_1.bmp", NULL);
    }
    else if (n==4) {
        b.img[0]=load_bitmap("geant_1.bmp", NULL);
    }
    else {
        b.img[0]=load_bitmap("poubelle.bmp", NULL);// on l'affiche pas
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

void lancer_animation(t_perso *perso, t_coord chemin[], int *etape_courante, int nb_etapes) {
    *etape_courante = 1;  // on part de l'étape 1, étape 0 = position actuelle

    int dx_total = (chemin[1].colonne - chemin[0].colonne) * TAILLE_CASE_X;
    int dy_total = (chemin[1].ligne - chemin[0].ligne) * TAILLE_CASE_Y;

    perso->dx = dx_total / NB_FRAMES;
    perso->dy = dy_total / NB_FRAMES;
    perso->frames_restantes = NB_FRAMES;
    perso->nb_frames = NB_FRAMES;
    perso->anim_en_cours = 1;
}

void animer(t_perso *perso, t_coord chemin[], int *etape_courante, int nb_etapes) {
    if (!perso->anim_en_cours) return;

    perso->x += perso->dx;
    perso->y += perso->dy;
    perso->frames_restantes--;

    perso->cptimg++;
    if (perso->cptimg >= perso->tmpimg) {
        perso->cptimg = 0;

        if (perso->dy < 0) { // haut
            perso->imgcourante = (perso->imgcourante < 5 || perso->imgcourante >= 10) ? 0 : perso->imgcourante;
            perso->imgcourante++;
            if (perso->imgcourante >= 5) perso->imgcourante = 0;
        } else if (perso->dy > 0) { // bas
            perso->imgcourante = (perso->imgcourante < 5 || perso->imgcourante >= 10) ? 5 : perso->imgcourante;
            perso->imgcourante++;
            if (perso->imgcourante >= 10) perso->imgcourante = 5;
        } else if (perso->dx > 0) { // droite
            perso->imgcourante = (perso->imgcourante < 10 || perso->imgcourante >= 15) ? 10 : perso->imgcourante;
            perso->imgcourante++;
            if (perso->imgcourante >= 15) perso->imgcourante = 10;
        } else if (perso->dx < 0) { // gauche
            perso->imgcourante = (perso->imgcourante < 15) ? 15 : perso->imgcourante;
            perso->imgcourante++;
            if (perso->imgcourante >= 20) perso->imgcourante = 15;
        }
    }

    if (perso->frames_restantes <= 0) {
        perso->x = chemin[*etape_courante].colonne * TAILLE_CASE_X;
        perso->y = chemin[*etape_courante].ligne * TAILLE_CASE_Y;

        (*etape_courante)++;
        if (*etape_courante >= nb_etapes) {
            perso->anim_en_cours = 0;
            perso->dx = 0;
            perso->dy = 0;
        } else {
            int dx_total = (chemin[*etape_courante].colonne - chemin[*etape_courante - 1].colonne) * TAILLE_CASE_X;
            int dy_total = (chemin[*etape_courante].ligne - chemin[*etape_courante - 1].ligne) * TAILLE_CASE_Y;
            perso->dx = dx_total / perso->nb_frames;
            perso->dy = dy_total / perso->nb_frames;
            perso->frames_restantes = perso->nb_frames;
        }
    }

    rest(20); // temporisation
}


void deplacement(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso,
                 int ligne_actu, int colonne_actu, t_coord chemin[], int *nb_etapes, int *etape_courante) {
    t_perso *perso = &p[tour_perso];

    if (perso->anim_en_cours) return;  // On ignore l'appel si une animation est déjà en cours

    int ligne_depart = perso->y / TAILLE_CASE_Y;
    int colonne_depart = perso->x / TAILLE_CASE_X;

    // Vérifier que la destination est différente
    if (ligne_depart == ligne_actu && colonne_depart == colonne_actu)
        return;

    // Ici on appelle la fonction pour calculer le chemin (pas détaillée ici)
    *nb_etapes = calculer_chemin(chemin, ligne_depart, colonne_depart, ligne_actu, colonne_actu, c);
    if (*nb_etapes <= 1) return;  // pas de chemin ou déplacement impossible

    // Lancer l’animation
    lancer_animation(perso, chemin, etape_courante, *nb_etapes);
}
