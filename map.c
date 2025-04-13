#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


void initialisation_allegro(){
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0)!=0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen);
}

t_case init_case(int n, int i, int j) {
    t_case c;
    c.x=i, c.y=j;
    c.o=0;
    c.p=0;
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

void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], bool equipe){
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
        for(int k=0; k<NB_PERSOS; k++) {
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

// Bug daffichage pas grave
void afficher_obstacles_persos(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS]) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            obs[i][j].a=0;
        }
    }
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            if(obs[i][j].e==1 && (i==0 || j==0 || (c[i][j+1].p==1 || c[i+1][j+1].p==1 || c[i+1][j].p==1))) {
                draw_sprite(buffer, obs[i][j].img, obs[i][j].x, obs[i][j].y);
                obs[i][j].a=1;
            }
        }
    }
    for(int k=0;k<NB_PERSOS;k++){
        draw_sprite(buffer, p[k].img, p[k].x, p[k].y);
    }
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            if(obs[i][j].e==1 && obs[i][j].a==0) {
                draw_sprite(buffer, obs[i][j].img, obs[i][j].x, obs[i][j].y);
                obs[i][j].a=1;
            }
        }
    }
}

t_potion init_inventaire (char nom_potion[20], int i) {
    // initialisation des potions
    t_potion p;
    p.x = 0;
    p.y = 0;
    p.xf = 0;
    p.yf = 0;
    sprintf(nom_potion, "Images\\potion_%d.bmp", i+1);
    p.img = load_bitmap(nom_potion, NULL);
    return p;
}

void creer_potion (t_potion p[NB_POTION], char nom_potion[20]) {
    for (int i = 0; i < NB_POTION; i++) {
        p[i] = init_inventaire(nom_potion, i);
        // fonctions pour que ca soit dans des cases de l'inventaire
        p[i].x = i*102+22;
        p[i].y = SCREEN_H-92;
        p[i].xf = p[i].x+68;
        p[i].yf = p[i].y+68;
    }
}

void point_vie (BITMAP* buffer, t_potion p[NB_POTION], int degats) {
    // il faudra remplacer les degats par les pv des joueurs dans les structures
    textprintf_ex(buffer, font, 724, SCREEN_H-70, makecol(0,0,0), -1, "%d", degats);
}

void afficher_inventaire (BITMAP* buffer, t_potion p[NB_POTION], int degats) {
    BITMAP* inventaire = load_bitmap("Images\\inventaire.bmp", NULL);
    blit(inventaire, buffer, 0, 0, 0, SCREEN_H-inventaire->h, SCREEN_W,SCREEN_H);
    for (int i = 0; i < NB_POTION; i++) {
        draw_sprite(buffer, p[i].img,  p[i].x, p[i].y);
    }
    point_vie(buffer, p, degats);
}

void souris (BITMAP* buffer, t_potion p[NB_POTION]) {// manque la structure personnage qui permetterait de recuperer me type de perso
    int souris = 0;
    // test pour savoir si les potions sont appuyées dans l'inventaire
    for (int i = 0; i < NB_POTION; i++) {
        if ((mouse_b & 1 || mouse_b & 2) && mouse_x>p[i].x && mouse_x<p[i].xf && mouse_y>p[i].y && mouse_y<p[i].yf) {
            souris = i+1;
        }
    }
    if (souris > 0) {
        // pour voir si ca fonctionne bien les boutons a changer plus tard
        BITMAP* image = load_bitmap("potion_1.bmp", NULL);
        blit(image, buffer, 0, 0, 0, 0, image->w, image->h);
    }
}