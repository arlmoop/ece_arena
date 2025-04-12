#include <stdio.h>
#include <allegro.h>
#include <time.h>
#include <stdlib.h>

#define TAILLE_MAP 19
#define NB_IMG_PERSOS 10
#define NB_CASES 10
#define NB_OBS 5
#define NB_IMG_EAU 5
#define NB_IMG_TERRE 3
#define Y_DEPART 60


typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type;
    // TYPE 1:terre 2:le reste
    BITMAP *img;
} t_case;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, e;
    // E 1:affiche 2:pas affiche
    // TYPE 1:arbre 2:mur
    BITMAP *img;
} t_obstacle;

typedef struct {
    int x, y, dx, dy, tx, ty, xcentre, ycentre, imgcourante, cptimg, tmpimg;
    BITMAP *img[NB_IMG_PERSOS];
}t_perso;


void initialisation_allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0)!=0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen);
}

t_spriteimmo initspriteimmo() {
    t_spriteimmo s;
    s.x=0, s.y=0;
    s.img=load_bitmap("logo_final.bmp", NULL);
    s.tx=s.img->w;
    s.ty=s.img->h;
    return s;
}

t_case init_case(int n, int i, int j) {
    t_case c;
    c.x=i, c.y=j;
    if (n==1) {
        c.img=load_bitmap("spawn_barbare.bmp", NULL);
        c.type=2;
    }
    else if (n==2) {
        c.img=load_bitmap("spawn_archer.bmp", NULL);
        c.type=2;
    }
    else if (n==3) {
        c.img=load_bitmap("bloc_glace.bmp", NULL);
        c.type=2;
    }
    else {
        c.img=load_bitmap("bloc_terre.bmp", NULL);
        c.type=1;
    }
    c.tx=c.img->w;
    c.ty=c.img->h;
    c.xcentre=c.x+c.tx/2;
    c.ycentre=c.y+c.ty/2;
    return c;
}

t_obstacle init_obstacle(int n, int i, int j, int a) {
    t_obstacle obs;
    obs.x=i, obs.y=j;
    obs.e=a;
    if (n==1) {
        obs.img=load_bitmap("gros_arbre.bmp", NULL);
        obs.type=1;
    }
    else if (n==2) {
        obs.img=load_bitmap("tronc.bmp", NULL);
        obs.type=1;
    }
    else if (n==3) {
        obs.img=load_bitmap("sapin.bmp", NULL);
        obs.type=1;
    }
    else {
        obs.img=load_bitmap("gem_box.bmp", NULL);
        obs.type=2;
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

void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP]){
    t_case case1=init_case(1,0,0);
    int m;
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            m=tab_map[i][j];
            c[i][j]=init_case(m,SCREEN_W/2-case1.tx/2+(case1.tx/2)*j-(case1.tx/2)*i,Y_DEPART+(case1.y+12)*j+(case1.y+12)*i);
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
            if(c[i][j].type==1 && rand()%10>8) {
                obs[i][j]=init_obstacle(1+rand()%NB_OBS, c[i][j].x, c[i][j].y-35, 1);
            }
            else {
                obs[i][j]=init_obstacle(1+rand()%NB_OBS, c[i][j].x, c[i][j].y-35, 0);
            }
        }
    }
}

void afficher_obstacles(BITMAP *buffer, t_obstacle obs[TAILLE_MAP][TAILLE_MAP]) {
    for(int i=0;i<TAILLE_MAP;i++){
        for(int j=0;j<TAILLE_MAP;j++){
            if(obs[i][j].e==1) {
                draw_sprite(buffer, obs[i][j].img, obs[i][j].x, obs[i][j].y);
            }
        }
    }
}

void menu(BITMAP *buffer) {
    t_spriteimmo logo=initspriteimmo();
    stretch_sprite(buffer, logo.img, 0, 0, buffer->w, buffer->h);
}


int main(void) {
    srand(time(NULL));
    initialisation_allegro();

    t_obstacle obs[TAILLE_MAP][TAILLE_MAP];
    t_case c[TAILLE_MAP][TAILLE_MAP];
    int tab_map[TAILLE_MAP][TAILLE_MAP];

    BITMAP *fond=load_bitmap("fond2.bmp", NULL);
    BITMAP *buffer=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *decor=create_bitmap(SCREEN_W,SCREEN_H);
    stretch_blit(fond,decor,0,0,fond->w,fond->h,0, 0, SCREEN_W,SCREEN_H);

    creer_fichier();
    charger_fichier(tab_map);
    creer_map(tab_map, c);
    creer_obstacles(c, obs);
    /*
    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        menu(buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    rest(100);
    */
    while(!key[KEY_ESC]) {
        clear_bitmap(buffer);
        blit(decor, buffer, 0, 0, 0, 0, SCREEN_W,SCREEN_H);
        afficher_map(buffer, c);
        //afficher_obstacles(buffer, obs);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    allegro_exit();
    return 0;
}
END_OF_MAIN();