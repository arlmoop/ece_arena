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
    c.num_joueur=0;
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
    obs.type=n;
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

void creer_fichier_map() {
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

void charger_fichier_map(int tab_map[TAILLE_MAP][TAILLE_MAP]) {
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

void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], int equipe, int nb_joueurs){
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

void creer_fichier_obs(t_case c[TAILLE_MAP][TAILLE_MAP]) {
    FILE* fichier=fopen("obstacles.txt", "w");
    if(fichier==NULL) {
        printf("Erreur\n");
        exit(1);
    }
    for(int i=0;i<TAILLE_MAP;i++) {
        for(int j=0;j<TAILLE_MAP;j++) {
            if(c[i][j].type==1 && 1+rand()%100>100-PRCNT_OBS) {
                fprintf(fichier,"%d 1 ", 1+rand()%NB_OBS);
            }
            else {
                fprintf(fichier,"%d 0 ", 1+rand()%NB_OBS);
            }
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

void charger_fichier_obs(t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP]) {
    FILE* fic=fopen("obstacles.txt", "r");
    if(fic==NULL) {
        printf("Erreur\n");
        exit(1);
    }
    for(int i=0;i<TAILLE_MAP;i++) {
        for(int j=0;j<TAILLE_MAP;j++) {
            fscanf(fic,"%d %d", &tab_obs[i][j].type, &tab_obs[i][j].e);
        }
    }
}

void creer_obstacles(t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            int m=tab_obs[i][j].type;
            int n=tab_obs[i][j].e;
            obs[i][j]=init_obstacle(m, c[i][j].x, c[i][j].y-20);
            if(n==1) {
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

            if (c[i][j].num_joueur != 0) {
                for (int k = 0; k < NB_PERSOS; k++) {
                    if (p[k].num==c[i][j].num_joueur) {
                        //int frame = p[k].imgcourante % p[k].nb_images;
                        if(p[k].mort==0)
                            draw_sprite(buffer, p[k].img[p[k].imgcourante], p[k].x, p[k].y);
                        else {
                            BITMAP *sac=load_bitmap("Images\\pierre_tombale.bmp", NULL);
                            draw_sprite(buffer, sac, p[k].x, p[k].y+15);
                        }
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

int comparer_coord(t_perso p, int ligne_actu, int colonne_actu, int *distance) {
    int r=0;
    if(abs(p.ligne-ligne_actu) + abs(p.colonne-colonne_actu)<=p.pm){
       *distance=abs(p.ligne-ligne_actu) + abs(p.colonne-colonne_actu);
        r=1;
    }
    return r;
}

void afficher_chemin(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], t_coord chemin[], int tour_perso,
                     int ligne_actu, int colonne_actu, int *distance, BITMAP* buffer, int *deplacement_valide) {
    *deplacement_valide=0;
    int ligne=p[tour_perso - 1].ligne;
    int colonne=p[tour_perso - 1].colonne;
    p[tour_perso - 1].dx=ligne_actu-ligne;
    p[tour_perso - 1].dy=colonne_actu-colonne;
    int signe_dx=(p[tour_perso - 1].dx>0)-(p[tour_perso - 1].dx<0);
    int signe_dy=(p[tour_perso - 1].dy>0)-(p[tour_perso - 1].dy<0);
    if(!comparer_coord(p[tour_perso-1], ligne_actu, colonne_actu, distance)) return;
    for(int i=0;i<*distance;i++){
        int changer=1;
        if(colonne != colonne_actu&&c[ligne][colonne+signe_dy].o==0&&c[ligne][colonne+signe_dy].p==0){
            colonne+=signe_dy;
            chemin[i].ligne=ligne;
            chemin[i].colonne=colonne;
            changer=0;
        }
        else if(ligne != ligne_actu&&changer&&c[ligne+signe_dx][colonne].o==0&&c[ligne+signe_dx][colonne].p==0){
            ligne+=signe_dx;
            chemin[i].ligne=ligne;
            chemin[i].colonne=colonne;
        }
    }
    if(chemin[*distance-1].ligne==ligne_actu&&chemin[*distance-1].colonne==colonne_actu){
        *deplacement_valide=1;
    }

    if(*deplacement_valide){
       for (int i = 0; i < *distance; i++) {
            remplir_losange(c[chemin[i].ligne][chemin[i].colonne], buffer, makecol(100, 255, 100));
        }
    }
}


void afficher_pause(t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP], BITMAP *buffer, int *compteur, int *degats, char nom_potion[], int *ligne_prec, int *ligne_actu,
                int *colonne_prec, int *colonne_actu, bool *valider_pm, bool *valider_pa, bool *passer_tour,
                int *tour_perso, int *nb_joueurs, int *distance,
                int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int *equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS],
                int choix_joueurs[], int *tour_depart, clock_t *depart, clock_t *pause, clock_t *tps_pause,
                bool *quitter, int *ca, int *deplacement_valide,
                int *numero_potion, int *chance_attaque, int *nb_morts) {

    if (clic_gauche(0, 0, SCREEN_W/18, SCREEN_H/15) && *compteur==0) {
        *compteur=1;
        *pause=clock();
        while(mouse_b & 1);
    }
    if (clic_gauche(SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, SCREEN_W-15*SCREEN_W/18, SCREEN_H-15*SCREEN_H/18) && *compteur>0) {
        if(*compteur==1) {
            *tps_pause+=clock()-*pause;
            *compteur=0;
        }
        if(*compteur>=2) {
            *compteur=1;
        }
        while(mouse_b & 1);
    }
    if (clic_gauche(SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10) && *compteur==1) {
        *compteur=2;
        while(mouse_b & 1);
    }
    if(clic_gauche(SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3) && *compteur==1) {
        *quitter=1;
    }
    if (clic_gauche(SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10) && *compteur==2) {
        recommencer(tab_obs, degats, nom_potion, ligne_prec, ligne_actu, colonne_prec, colonne_actu,
            compteur, valider_pm, valider_pa, passer_tour, tour_perso, nb_joueurs,
            distance, tab_map, c, equipe, obs, p, choix_joueurs, tour_depart, 0, depart, tps_pause,
            quitter, ca, deplacement_valide, numero_potion, chance_attaque, nb_morts);
        *depart=clock();
        while(mouse_b & 1);
    }
    if (clic_gauche(SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3) && *compteur==2) {
        recommencer(tab_obs, degats, nom_potion, ligne_prec, ligne_actu, colonne_prec, colonne_actu,
            compteur, valider_pm, valider_pa, passer_tour, tour_perso, nb_joueurs,
            distance, tab_map, c, equipe, obs, p, choix_joueurs, tour_depart, 1, depart, tps_pause,
            quitter, ca, deplacement_valide, numero_potion, chance_attaque, nb_morts);
        *depart=clock();
        while(mouse_b & 1);
    }


    if (*compteur==0) {
        rectfill(buffer, 0, 0, SCREEN_W/18, SCREEN_H/15, makecol(20, 20, 20));
        rect(buffer, 0, 0, SCREEN_W/18, SCREEN_H/15, makecol(200, 150, 60));
        rectfill(buffer, 10, 10, SCREEN_W/36-3, SCREEN_H/15-10, makecol(200, 200, 200));
        rectfill(buffer, SCREEN_W/18-10, 10, SCREEN_W/36+3, SCREEN_H/15-10, makecol(200, 200, 200));
    }
    if (*compteur==1) {
        hg(buffer, 0);
        //RECOMMENCER
        rectfill(buffer, SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10, makecol(50, 50, 50));
        rect(buffer, SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10, makecol(200, 150, 60));
        textout_centre_ex(buffer, font, "RECOMMENCER", SCREEN_W/2, SCREEN_H/3+40, makecol(200, 150, 60), -1);
        //QUITTER
        rectfill(buffer, SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3, makecol(50, 50, 50));
        rect(buffer, SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3, makecol(200, 150, 60));
        textout_centre_ex(buffer, font, "QUITTER LA PARTIE", SCREEN_W/2, SCREEN_H/2+50, makecol(200, 150, 60), -1);
    }
    if (*compteur==2) {
        hg(buffer, 0);
        //sur cette map
        rectfill(buffer, SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10, makecol(50, 50, 50));
        rect(buffer, SCREEN_W/3, SCREEN_H/3, 2*SCREEN_W/3, SCREEN_H/2-10, makecol(200, 150, 60));
        textout_centre_ex(buffer, font, "SUR CETTE MAP", SCREEN_W/2, SCREEN_H/3+40, makecol(200, 150, 60), -1);
        //sur autre map
        rectfill(buffer, SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3, makecol(50, 50, 50));
        rect(buffer, SCREEN_W/3, SCREEN_H/2+10, 2*SCREEN_W/3, 2*SCREEN_H/3, makecol(200, 150, 60));
        textout_centre_ex(buffer, font, "AUTRE MAP", SCREEN_W/2, SCREEN_H/2+50, makecol(200, 150, 60), -1);
    }
    if(*compteur==3) {
        hg(buffer, 1);

        for(int i=0; i<*nb_joueurs; i++) {
            textout_centre_ex(buffer, font, "CLASSEMENT :", SCREEN_W/2, SCREEN_H-12*SCREEN_H/13, makecol(200, 150, 60), -1);
            rectfill(buffer, SCREEN_W/8, SCREEN_H/5+i*100, 7*SCREEN_W/8, SCREEN_H/5+i*100+90, makecol(200, 200, 200));
            rect(buffer, SCREEN_W/8, SCREEN_H/5+i*100, 7*SCREEN_W/8, SCREEN_H/5+i*100+90, makecol(200, 150, 60));
            rect(buffer, SCREEN_W/8+5, SCREEN_H/5+i*100+5, SCREEN_W/8+85, SCREEN_H/5+i*100+90-5, makecol(200, 150, 60));
            for(int j=0; j<*nb_joueurs; j++) {
                if(p[j].cl==i+1) {
                    stretch_sprite(buffer, p[j].img[0], SCREEN_W/8+5, SCREEN_H/5+i*100+5, p[j].tx*21/15, p[j].ty*25/15);
                    textprintf_ex(buffer, font, SCREEN_W/8+105, SCREEN_H/5+i*100+35, makecol(0, 0, 25), -1, "%s : %d", p[j].nom, p[j].cl);
                }
            }
        }
    }
}

void recommencer(t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP], int *degats, char nom_potion[], int *ligne_prec, int *ligne_actu,
                int *colonne_prec, int *colonne_actu, int *compteur,
                bool *valider_pm, bool *valider_pa, bool *passer_tour,
                int *tour_perso, int *nb_joueurs, int *distance,
                int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int *equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS],
                int choix_joueurs[], int *tour_depart, bool map, clock_t *depart, clock_t *tps_pause, bool *quitter,
                int *ca, int *deplacement_valide, int *numero_potion, int *chance_attaque, int *nb_morts) {

    *degats = 100;
    *ligne_prec=-1;
    *colonne_prec=-1;
    *ligne_actu = -1;
    *colonne_actu = -1;
    *compteur=0;
    *valider_pm=0;
    *valider_pa=0;
    *passer_tour=0;
    *tour_perso=*tour_depart;
    *distance=0;
    *depart=clock();
    *tps_pause=0;
    *quitter=0;
    *ca=0;
    *deplacement_valide=0;
    *numero_potion=0;
    *chance_attaque=3;
    *nb_morts=0;

    if(map) {
        creer_fichier_map();
        charger_fichier_map(tab_map);
        creer_map(tab_map, c, *equipe, *nb_joueurs);
        creer_fichier_obs(c);
        charger_fichier_obs(tab_obs);
        creer_obstacles(tab_obs, c, obs);
    }

    placer_persos(c, p, choix_joueurs);
    equiper_potion(p, nom_potion);
    equiper_attaque(p, *nb_joueurs);
}

void sauvegarde(int tab_map[TAILLE_MAP][TAILLE_MAP], t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int nb_joueurs, int choix_joueurs[],
                char nom_potion[]) {
    charger_fichier_map(tab_map);
    creer_map(tab_map, c, equipe, nb_joueurs);
    charger_fichier_obs(tab_obs);
    creer_obstacles(tab_obs, c, obs);
    placer_persos(c, p, choix_joueurs);
    equiper_potion(p, nom_potion);
    equiper_attaque(p, nb_joueurs);
}

void nouvelle_partie (BITMAP *buffer, int tab_map[TAILLE_MAP][TAILLE_MAP], t_obstacle tab_obs[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int nb_joueurs, int choix_joueurs[],
                char nom_potion[]) {
    creer_fichier_map();
    charger_fichier_map(tab_map);
    creer_map(tab_map, c, equipe, nb_joueurs);
    creer_fichier_obs(c);
    charger_fichier_obs(tab_obs);
    creer_obstacles(tab_obs, c, obs);
    placer_persos(c, p, choix_joueurs);
    equiper_potion(p, nom_potion);
    equiper_attaque(p, nb_joueurs);
    saisir_noms(buffer, p, nb_joueurs);
}

void afficher_infos (double *secondes, clock_t depart, clock_t tps_pause, t_perso p[NB_PERSOS],
    BITMAP *buffer, int tour_perso, int nb_joueurs) {
    char t[30];
    char pm[30];
    char pa[30];
    //TOUR
    sprintf(t, "Au tour de : %s", p[tour_perso-1].nom);
    textout_ex(buffer, font, t, 600, 10, makecol(0, 255, 255), -1);
    //JOUEURS SUIVANTS
    joueurs_suivants(p, buffer, tour_perso, nb_joueurs);
    //PA
    sprintf(pa, "Il vous reste %d PA", p[tour_perso-1].pa);
    textout_ex(buffer, font, pa, 100, 30, makecol(255, 0, 255), -1);
    //PM
    sprintf(pm, "Il vous reste %d PM", p[tour_perso-1].pm);
    textout_ex(buffer, font, pm, 100, 50, makecol(255, 0, 255), -1);
    //TIMER
    timer(buffer, secondes, depart, tps_pause);
}

void hg(BITMAP *buffer, bool g) {
    rectfill(buffer, 17*SCREEN_W/18, 17*SCREEN_H/18, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, makecol(20, 20, 20));
    rect(buffer, 17*SCREEN_W/18, 17*SCREEN_H/18, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, makecol(200, 150, 60));
    rectfill(buffer, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, SCREEN_W-15*SCREEN_W/18, SCREEN_H-15*SCREEN_H/18, makecol(50, 50, 50));
    rect(buffer, SCREEN_W-17*SCREEN_W/18, SCREEN_H-17*SCREEN_H/18, SCREEN_W-15*SCREEN_W/18, SCREEN_H-15*SCREEN_H/18, makecol(200, 150, 60));
    if(g==0) {
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-17*SCREEN_H/18+10, makecol(200, 200, 200));
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36-1, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-17*SCREEN_H/18+10-1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36+1, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-17*SCREEN_H/18+10+1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-15*SCREEN_H/18-10, makecol(200, 200, 200));
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36-1, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-15*SCREEN_H/18-10-1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-32*SCREEN_H/36+1, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-15*SCREEN_H/18-10+1, makecol(200, 200, 200));
    }
    else {
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-17*SCREEN_H/18+10, makecol(200, 200, 200));
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36-1, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-17*SCREEN_H/18+10-1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36+1, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-17*SCREEN_H/18+10+1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-15*SCREEN_H/18-10, makecol(200, 200, 200));
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36-1, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-15*SCREEN_H/18-10-1, makecol(200, 200, 200));
        line(buffer, SCREEN_W-15*SCREEN_W/18-12, SCREEN_H-32*SCREEN_H/36+1, SCREEN_W-17*SCREEN_W/18+12, SCREEN_H-15*SCREEN_H/18-10+1, makecol(200, 200, 200));
    }
}

void joueurs_suivants(t_perso p[NB_PERSOS], BITMAP *buffer, int tour_perso, int nb_joueurs) {
    textout_ex(buffer, font, "Joueurs suivants :", 552, 30, makecol(0, 128, 255), -1);

    if(tour_perso<nb_joueurs) {
        if(p[tour_perso].mort==0)
            textout_ex(buffer, font, p[tour_perso].nom, 704, 30, makecol(0, 128, 255), -1);
        else
            textout_ex(buffer, font, "//////////", 704, 30, makecol(0, 128, 255), -1);
    }
    else {
        if(p[tour_perso].mort==0)
            textout_ex(buffer, font, p[0].nom, 704, 30, makecol(0, 128, 255), -1);
        else
            textout_ex(buffer, font, "//////////", 704, 30, makecol(0, 128, 255), -1);
    }

    if(nb_joueurs>2) {
        if(tour_perso+1<nb_joueurs) {
            if(p[tour_perso+1].mort==0)
                textout_ex(buffer, font, p[tour_perso+1].nom, 704, 50, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
        else if (tour_perso<nb_joueurs){
            if(p[nb_joueurs-tour_perso-1].mort==0)
                textout_ex(buffer, font, p[nb_joueurs-tour_perso-1].nom, 704, 50, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
        else {
            if(p[nb_joueurs-tour_perso+1].mort==0)
                textout_ex(buffer, font, p[nb_joueurs-tour_perso+1].nom, 704, 50, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
    }

    if(nb_joueurs>3) {
        if(tour_perso+2<nb_joueurs){
            if(p[tour_perso+2].mort==0)
                textout_ex(buffer, font, p[tour_perso+2].nom, 704, 70, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
        else if(tour_perso+1<nb_joueurs) {
            if(p[0].mort==0)
                textout_ex(buffer, font, p[0].nom, 704, 70, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
        else if(tour_perso<nb_joueurs) {
            if(p[1].mort==0)
                textout_ex(buffer, font, p[1].nom, 704, 70, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
        else {
            if(p[2].mort==0)
                textout_ex(buffer, font, p[2].nom, 704, 70, makecol(0, 128, 255), -1);
            else
                textout_ex(buffer, font, "//////////", 704, 50, makecol(0, 128, 255), -1);
        }
    }
}

void timer (BITMAP *buffer, double *secondes, clock_t depart, clock_t tps_pause) {
    char temps[30];
    *secondes=(double)(clock()-depart-tps_pause)/CLOCKS_PER_SEC;
    sprintf(temps, "Temps : %.0f s", *secondes);
    textout_ex(buffer, font, temps, 100, 10, makecol(255, 255, 0), -1);
}
