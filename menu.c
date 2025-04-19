#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_spriteimmo init_spriteimmo() {
    t_spriteimmo s;
    s.x=0, s.y=0;
    s.img=load_bitmap("Images\\logo_final.bmp", NULL);
    s.tx=s.img->w;
    s.ty=s.img->h;
    return s;
}

void disparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2) {
    BITMAP*nuage=create_bitmap(SCREEN_W,SCREEN_H);
    int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;
    while(y1!=500&&x2!=250) {
        clear_bitmap(nuage);
        blit(menu,nuage,0,0,0,0,SCREEN_W,SCREEN_H);
        x1 -= 5; y1 += 5;
        x2 += 5; y2 -= 5;
        stretch_sprite(nuage, fond1, x1, y1,SCREEN_W,SCREEN_H);
        stretch_sprite(nuage, fond2, x2, y2,SCREEN_W,SCREEN_H);
        blit(nuage, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }
    destroy_bitmap(nuage);
}

void apparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2) {
    BITMAP*nuage=create_bitmap(SCREEN_W,SCREEN_H);
    int x1 = -250, y1 = 250;
    int x2 = 250, y2 = -250;
    while (y1 > 0 || x2 > 0) {
        clear_bitmap(nuage);
        blit(menu,nuage,0,0,0,0,SCREEN_W,SCREEN_H);
        x1 += 5;
        y1 -= 5;
        x2 -= 5;
        y2 += 5;

        stretch_sprite(nuage, fond1, x1, y1, SCREEN_W, SCREEN_H);
        stretch_sprite(nuage, fond2, x2, y2, SCREEN_W, SCREEN_H);
        blit(nuage, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(10);
    }
    destroy_bitmap(nuage);
}

int bouton_bas_droite(BITMAP *menu, int est_retour,int format_menu) {
    int bouton_w = 50;
    int bouton_h = 40;
    int x,y;
    if(!format_menu){
        x = (SCREEN_W + MENU_W) / 2 - bouton_w ;
        y = (SCREEN_H + MENU_H) / 2 - bouton_h ;
    } else{
        x = (SCREEN_W + MENU_CLASSES_W) / 2 - bouton_w ;
        y = (SCREEN_H + MENU_CLASSES_H) / 2 - bouton_h ;
    }
    int mx = mouse_x;
    int my = mouse_y;
    int curseur = (mx >= x && mx <= x + bouton_w && my >= y && my <= y + bouton_h);
    int couleur = curseur ? makecol(180, 80, 60) : makecol(100, 50, 30);
    int fleche = makecol(255, 255, 0);

    rectfill(menu, x, y, x + bouton_w, y + bouton_h, couleur);
    rect(menu, x, y, x + bouton_w, y + bouton_h, makecol(255, 215, 0));

    if (est_retour) {
        line(menu, x + 35, y + 10, x + 15, y + 20, fleche);
        line(menu, x + 15, y + 20, x + 35, y + 30, fleche);
    } else {
        line(menu, x + 15, y + 10, x + 35, y + 30, fleche);
        line(menu, x + 35, y + 10, x + 15, y + 30, fleche);
    }

    return (curseur && (mouse_b & 1));
}

int afficher_menu_principal(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut) {
    int menu_x = (SCREEN_W - MENU_W) / 2;
    int menu_y = (SCREEN_H - MENU_H) / 2;

    int bouton_h = 60;
    int bouton_w = 350;
    int bouton_x = menu_x + 25;

    const char *options_1[2] = {"Commencer une nouvelle partie", "Charger une partie"};
    int choix = -1;

    while (!key[KEY_ESC] && choix==-1) {
        clear_bitmap(menu);
        rectfill(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(60, 40, 20));
        rect(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(255, 215, 0));
        textprintf_centre_ex(menu, font, menu_x + MENU_W / 2, menu_y + 20, makecol(255, 255, 0), -1, "MENU");

        for (int i = 0; i < 2; i++) {
            int mx = mouse_x;
            int my = mouse_y;
            int bouton_y = menu_y + 60 + i * (bouton_h + 10);
            int curseur = (mx >= bouton_x && mx <= bouton_x + bouton_w && my >= bouton_y && my <= bouton_y + bouton_h);
            int couleur = curseur ? makecol(200, 150, 60) : makecol(120, 80, 30);
            int txt_couleur = curseur ? makecol(255, 255, 255) : makecol(220, 180, 100);

            rectfill(menu, bouton_x, bouton_y, bouton_x + bouton_w, bouton_y + bouton_h, couleur);
            rect(menu, bouton_x, bouton_y, bouton_x + bouton_w, bouton_y + bouton_h, makecol(255, 215, 0));
            textprintf_centre_ex(menu, font, bouton_x + bouton_w / 2, bouton_y + 20, txt_couleur, -1, options_1[i]);

            if (curseur && (mouse_b & 1)) {
                choix = i;
                while(mouse_b & 1);
                break;
            }
            if (bouton_bas_droite(menu, 0 ,0)) {
                exit(0);
            }
        }

        stretch_sprite(menu, fond_nuage_haut, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(menu, fond_nuage_bas, 0, 0, SCREEN_W, SCREEN_H);

        blit(menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    return choix;
}

int afficher_menu_map(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut,int *theme,int *etat_barre_aleatoire) {
    int menu_x = (SCREEN_W - MENU_W) / 2;
    int menu_y = (SCREEN_H - MENU_H) / 2;

    int bouton_h = 60;
    int bouton_w = 350;
    int bouton_x = menu_x + 25;
    int espace_map=15;
    int map_y=menu_y+130;
    int map_w=bouton_w/2-0.5*espace_map;
    int map_h=bouton_h*2;
    int aleatoire_x=menu_x+MENU_W/2+50;
    int aleatoire_y=map_y+map_h+22;
    int aleatoire_r=8;
    int barre_aleatoire_x=aleatoire_x-aleatoire_r;
    int barre_aleatoire_y=aleatoire_y-aleatoire_r;
    int barre_aleatoire_w=2*aleatoire_r+14;
    int barre_aleatoire_h=2*aleatoire_r;
    //const char *options_2[3] = {"Choix de la classe des personnages"};
    int choix = -1;

    while (!key[KEY_ESC] && choix==-1) {
        clear_bitmap(menu);
        int mx = mouse_x;
        int my = mouse_y;

        rectfill(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(60, 40, 20));
        rect(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(255, 215, 0));
        textprintf_centre_ex(menu, font, menu_x + MENU_W / 2, menu_y + 20, makecol(255, 255, 0), -1, "Choix de la map");
        textprintf_centre_ex(menu, font, menu_x + MENU_W / 2, map_y+map_h+20, makecol(255, 255, 0), -1, "Aleatoire");

        int curseur_barre_aleatoire=(mx >= barre_aleatoire_x && mx <= barre_aleatoire_x + barre_aleatoire_w && my >= barre_aleatoire_y && my <= barre_aleatoire_y + barre_aleatoire_h);
        if(curseur_barre_aleatoire &&(mouse_b & 1)){
            while(mouse_b & 1)
            *etat_barre_aleatoire=!(*etat_barre_aleatoire);
        }
        int couleur_barre_aleatoire=*etat_barre_aleatoire ? makecol(200, 150, 60) : makecol(120, 80, 30);
        for(int i=0;i<15;i++){
            circlefill(menu,aleatoire_x+i,aleatoire_y,aleatoire_r+2,makecol(200, 150, 60));
        }
        for(int i=0;i<15;i++){
            circlefill(menu,aleatoire_x+i,aleatoire_y,aleatoire_r,couleur_barre_aleatoire);
        }
        if(*etat_barre_aleatoire){
                circlefill(menu,aleatoire_x+12,aleatoire_y,aleatoire_r-2,makecol(255,255,255));
        } else{
            circlefill(menu,aleatoire_x+2,aleatoire_y,aleatoire_r-2,makecol(255,255,255));
        }
        for (int i = 0; i < 1; i++) {
            int bouton_y = menu_y + 60 + i * (bouton_h + 10);
            int curseur = (mx >= bouton_x && mx <= bouton_x + bouton_w && my >= bouton_y && my <= bouton_y + bouton_h);
            int couleur = curseur ? makecol(200, 150, 60) : makecol(120, 80, 30);
            int txt_couleur = curseur ? makecol(255, 255, 255) : makecol(220, 180, 100);
            rectfill(menu, bouton_x, bouton_y, bouton_x + bouton_w, bouton_y + bouton_h, couleur);
            rect(menu, bouton_x, bouton_y, bouton_x + bouton_w, bouton_y + bouton_h, makecol(255, 215, 0));
            textprintf_centre_ex(menu, font, bouton_x + bouton_w / 2, bouton_y + 20, txt_couleur, -1, "Choix de la classe des personnages");

            if (curseur && (mouse_b & 1)) {
                choix = i;
                while(mouse_b & 1);
                break;
            }

        }
        for(int i=0;i<2;i++){
            int map_x = bouton_x + i*(map_w+espace_map);
            int curseur_map = (mx >= map_x && mx <= map_x + map_w && my >= map_y && my <= map_y + map_h);
            int couleur = curseur_map ? makecol(200, 150, 60) : makecol(120, 80, 30);
            //int txt_couleur = curseur ? makecol(255, 255, 255) : makecol(220, 180, 100);
            if(curseur_map) rectfill(menu, map_x-3, map_y-3, map_x + map_w+3, map_y + map_h+3, couleur);
            rectfill(menu, map_x, map_y, map_x + map_w, map_y + map_h, makecol(255,0,0));
            if(*theme==0&&!i){
                rectfill(menu, map_x-3, map_y-3, map_x + map_w+3, map_y + map_h+3, makecol(0,0,255));
                rectfill(menu, map_x, map_y, map_x + map_w, map_y + map_h, makecol(255,0,0));
            }
            else if(*theme==1&&i){
                rectfill(menu, map_x-3, map_y-3, map_x + map_w+3, map_y + map_h+3, makecol(0,0,255));
                rectfill(menu, map_x, map_y, map_x + map_w, map_y + map_h, makecol(255,0,0));
            }
            if (curseur_map && (mouse_b & 1)) {
                *theme=i;
                break;
            }
        }
        if (bouton_bas_droite(menu, 1, 0)) {
            choix = -2;
            while(mouse_b & 1);
            break;
        }
        stretch_sprite(menu, fond_nuage_haut, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(menu, fond_nuage_bas, 0, 0, SCREEN_W, SCREEN_H);

        blit(menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    }
    return choix;
}

int afficher_choix_joueurs(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut, int n,int *nb_joueurs,int *lancer,int *equipe){
    BITMAP *bouton_lancer[2];
    for(int i=0;i<2;i++){
        char filename[30];
        sprintf(filename,"Images\\bouton_lancer%d.bmp",i);
        bouton_lancer[i]=load_bitmap(filename,NULL);
    }

    int menu_x = (SCREEN_W - MENU_W) / 2;
    int menu_y = (SCREEN_H - MENU_H) / 2;

    int bouton_h = 20;
    int bouton_w = 20;
    int bouton_y = menu_y + 10;
    int bouton_equipe_h = 24;
    int bouton_equipe_w = 80;
    int bouton_equipe_y = menu_y + 10;
    int bouton_equipe_x = menu_x + MENU_W/2 + 90;
    int espace_cadre=20;
    int cadre_w=MENU_W/2-35;
    int cadre_h=MENU_H/2-bouton_h-50;
    int bouton_lancer_w=cadre_w;
    int bouton_lancer_h=cadre_h/2;
    int bouton_lancer_x=menu_x/2+MENU_W/2+15;
    int bouton_lancer_y=menu_y+2*cadre_h+espace_cadre+70;
    int choix = -1;

    while (!key[KEY_ESC] && choix==-1) {
        clear_bitmap(menu);
        int mx = mouse_x;
        int my = mouse_y;

        rectfill(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(60, 40, 20));
        rect(menu, menu_x, menu_y, menu_x + MENU_W, menu_y + MENU_H, makecol(255, 215, 0));
        int curseur_bouton_lancer = (mx >= bouton_lancer_x && mx <= bouton_lancer_x + bouton_lancer_w && my >= bouton_lancer_y && my <= bouton_lancer_y + bouton_lancer_h);
        curseur_bouton_lancer ? stretch_sprite(menu,bouton_lancer[1],bouton_lancer_x+6,bouton_lancer_y+2,bouton_lancer_w-12,bouton_lancer_h-4) : stretch_sprite(menu,bouton_lancer[0],bouton_lancer_x,bouton_lancer_y,bouton_lancer_w,bouton_lancer_h);
        textprintf_centre_ex(menu, font, bouton_lancer_x+bouton_lancer_w/2, bouton_lancer_y+bouton_lancer_h/2-2, makecol(255, 255, 255), -1, "LANCER");
        if(curseur_bouton_lancer&&(mouse_b & 1)){
            *lancer=1;
            while(mouse_b & 1);
            return;
        }
        for(int i=0;i<2;i++){
            int bouton_x = menu_x + MENU_W/2 + i*40;
            int curseur_bouton=(mx >= bouton_x && mx <= bouton_x + bouton_w && my >= bouton_y && my <= bouton_y + bouton_h);
            rectfill(menu,bouton_x+4,bouton_y+4,bouton_x+4+bouton_w,bouton_y+4+bouton_h,makecol(120, 80, 30));
            rect(menu,bouton_x+4,bouton_y+4,bouton_x+4+bouton_w,bouton_y+4+bouton_h,makecol(200, 150, 60));
            rectfill(menu,bouton_x,bouton_y,bouton_x+bouton_w,bouton_y+bouton_h,makecol(255, 215, 0));
            rect(menu,bouton_x,bouton_y,bouton_x+bouton_w,bouton_y+bouton_h,makecol(200, 150, 60));
            //makecol(200, 150, 60) : makecol(120, 80, 30);
            line(menu, bouton_x+2, bouton_y+bouton_h/2, bouton_x+bouton_w-2, bouton_y+bouton_h/2, makecol(0,0,0));
            if(i)line(menu, bouton_x+bouton_w/2, bouton_y+2, bouton_x+bouton_w/2, bouton_y+bouton_h-2, makecol(0,0,0));
            if(curseur_bouton && (mouse_b & 1)){
                if(i&&*nb_joueurs<4){
                    (*nb_joueurs)++;
                    while(mouse_b & 1);
                } else if(!i&&*nb_joueurs>2){
                    (*nb_joueurs)--;
                    *equipe=0;
                    while(mouse_b & 1);
                }
            }
        }
        char filename[30];
        sprintf(filename,"Nombre de joueurs : %d",*nb_joueurs);
        textprintf_centre_ex(menu, font, menu_x + MENU_W / 4, menu_y + 20, makecol(255, 255, 0), -1, filename);
        int curseur_bouton_equipe=(mx >= bouton_equipe_x && mx <= bouton_equipe_x + bouton_equipe_w && my >= bouton_equipe_y && my <= bouton_equipe_y + bouton_equipe_h);
        int x=*equipe;
        if(curseur_bouton_equipe&&(mouse_b & 1)&&*nb_joueurs==4){
            while(mouse_b & 1)
            (*equipe)=!x;
        }
        int couleur_bouton_equipe=(*equipe)?makecol(255, 215, 0):makecol(120, 80, 30);
        if(curseur_bouton_equipe){
            rectfill(menu,bouton_equipe_x+2,bouton_equipe_y+2,bouton_equipe_x+bouton_equipe_w-2,bouton_equipe_y+bouton_equipe_h-2,couleur_bouton_equipe);
            rect(menu,bouton_equipe_x+2,bouton_equipe_y+2,bouton_equipe_x+bouton_equipe_w-2,bouton_equipe_y+bouton_equipe_h-2,makecol(200, 150, 60));
        } else{
            rectfill(menu,bouton_equipe_x,bouton_equipe_y,bouton_equipe_x+bouton_equipe_w,bouton_equipe_y+bouton_equipe_h,couleur_bouton_equipe);
            rect(menu,bouton_equipe_x,bouton_equipe_y,bouton_equipe_x+bouton_equipe_w,bouton_equipe_y+bouton_equipe_h,makecol(200, 150, 60));
        }

        textprintf_centre_ex(menu, font, bouton_equipe_x+bouton_equipe_w/2, bouton_equipe_y+bouton_equipe_h/2.5, makecol(255, 255, 255), -1, "Equipe");

        for(int i=0;i<*nb_joueurs;i++){
            int cadre_x=menu_x+25+(i%2)*(espace_cadre+cadre_w);
            int cadre_y=menu_y+60;
            if(i>1)cadre_y+=espace_cadre+cadre_h;
            int curseur_cadre=(mx >= cadre_x && mx <= cadre_x + cadre_w && my >= cadre_y && my <= cadre_y + cadre_h);
            if(curseur_cadre){
                rectfill(menu,cadre_x-4,cadre_y-4,cadre_x+cadre_w+4,cadre_y+cadre_h+4,makecol(200, 150, 60));
            }
            rectfill(menu,cadre_x,cadre_y,cadre_x+cadre_w,cadre_y+cadre_h,makecol(120, 80, 30));
            char j[30];
            sprintf(j,"joueur %d",i+1);
            int couleur_joueur;
            if(*equipe){
                if(i<2) couleur_joueur=makecol(0,0,255);
                else couleur_joueur=makecol(255,0,0);
            }
            else couleur_joueur=makecol(255, 255, 255);
            textprintf_centre_ex(menu,font,cadre_x+cadre_w/2,cadre_y, couleur_joueur,-1,j);

            if (curseur_cadre&&(mouse_b & 1)) {
                choix = i;
                while(mouse_b & 1);
                break;
            }
        }

        if (bouton_bas_droite(menu, 1, 0)) {
            choix = -2;
            while(mouse_b & 1);
            break;
        }
        if(n==0)apparition_fond_nuage(menu,fond_nuage_bas,fond_nuage_haut);
        stretch_sprite(menu, fond_nuage_haut, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(menu, fond_nuage_bas, 0, 0, SCREEN_W, SCREEN_H);

        blit(menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        n++;
    }
    return choix;
}

int afficher_classes_personnages(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut) {
    BITMAP *banieres[4];
    BITMAP *bouton_choix[2];
    BITMAP *icone_skin[2];
    for(int i=0;i<4;i++){
        char filename[20];
        sprintf(filename,"Images\\baniere%d.bmp",i);
        banieres[i]=load_bitmap(filename,NULL);
    }
    for(int i=0;i<2;i++){
        char filename[20];
        sprintf(filename,"Images\\choix_classe%d.bmp",i);
        bouton_choix[i]=load_bitmap(filename,NULL);
    }
    for(int i=0;i<2;i++){
        char filename[20];
        sprintf(filename,"Images\\icone_skin%d.bmp",i);
        icone_skin[i]=load_bitmap(filename,NULL);
    }
    clear_to_color(menu, makecol(25, 25, 25));
    int cadre_total_x= 100;
    int cadre_total_y = 60;

    int choix=-1;
    int n=0;
    while(!key[KEY_ESC]&&choix==-1){
        clear_bitmap(menu);
        rectfill(menu, cadre_total_x, cadre_total_y, cadre_total_x + MENU_CLASSES_W, cadre_total_y + MENU_CLASSES_H, makecol(40, 30, 20));
        rect(menu, cadre_total_x, cadre_total_y, cadre_total_x + MENU_CLASSES_W, cadre_total_y + MENU_CLASSES_H, makecol(255, 215, 0));
        textprintf_centre_ex(menu, font, SCREEN_W / 2, cadre_total_y + 10, makecol(255, 255, 0), -1, "CLASSES DES PERSONNAGES");

        int espace = 30;
        int cadre_w = (MENU_CLASSES_W/4-espace);
        int cadre_h = MENU_CLASSES_H-4*espace;
        int total_w = 4 * cadre_w + 3 * espace;
        int x_depart = (SCREEN_W - total_w) /2;
        int y = cadre_total_y + 60;

        for (int i = 0; i < 4; i++) {
            int x = x_depart + i * (cadre_w + espace);
            int bouton_choix_x=x+10;
            int bouton_choix_y=y+0.85*cadre_h;
            int bouton_choix_w=cadre_w-20;
            int bouton_choix_h=cadre_h/10;
            int icone_skin_x=x-2;
            int icone_skin_y=y+0.60*cadre_h;
            int icone_skin_w=cadre_w/4;
            int icone_skin_h=cadre_h/10;

            int mx = mouse_x;
            int my = mouse_y;
            int curseur_bouton_choix = (mx >= bouton_choix_x && mx <= bouton_choix_x + bouton_choix_w && my >= bouton_choix_y && my <= bouton_choix_y + bouton_choix_h);
            int curseur_icone_skin = (mx >= icone_skin_x && mx <= icone_skin_x + icone_skin_w && my >= icone_skin_y && my <= icone_skin_y + icone_skin_h);
            rectfill(menu, x, y, x + cadre_w, y + cadre_h, makecol(70, 50, 30));
            rect(menu, x, y, x + cadre_w, y + cadre_h, makecol(255, 215, 0));

            stretch_sprite(menu,banieres[i],x-5,y-20,cadre_w+10,75);
            curseur_bouton_choix ? stretch_sprite(menu,bouton_choix[1],bouton_choix_x+6,bouton_choix_y+2,bouton_choix_w-12,bouton_choix_h-4) : stretch_sprite(menu,bouton_choix[0],bouton_choix_x,bouton_choix_y,bouton_choix_w,bouton_choix_h);
            curseur_icone_skin ? stretch_sprite(menu,icone_skin[1],icone_skin_x,icone_skin_y+4,icone_skin_w,icone_skin_h-8) : stretch_sprite(menu,icone_skin[0],icone_skin_x,icone_skin_y,icone_skin_w,icone_skin_h);

            const char* nom_classe[] = {"Guerrier", "Archer", "Mage", "Championne"};
            textprintf_centre_ex(menu, font, x + cadre_w / 2, y + 20, makecol(255, 255, 255), -1, "%s", nom_classe[i]);
            textprintf_centre_ex(menu, font, bouton_choix_x+bouton_choix_w/2, bouton_choix_y+bouton_choix_h/2.5, makecol(0, 0, 0), -1, "Choisir");

            if(curseur_bouton_choix && (mouse_b & 1)){
                choix=i;
                break;
            }
            if(curseur_icone_skin && (mouse_b & 1)){
                choix=i+4;
                break;
            }
        }


        if (bouton_bas_droite(menu, 1, 1)) {
            choix = -2;
            while(mouse_b & 1);
            break;
        }

        if(n==0)disparition_fond_nuage(menu,fond_nuage_bas,fond_nuage_haut);
        blit(menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        n++;
    }
    return choix;
}

int menu(int *aleatoire,int *theme,int *nb_joueurs,int choix_joueurs[],int *equipe) {
    BITMAP *menu = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *fond_nuage_bas = load_bitmap("Images\\fond_nuage_bas.bmp", NULL);
    BITMAP *fond_nuage_haut = load_bitmap("Images\\fond_nuage_haut.bmp", NULL);
    EtatMenu etat_actuel = MENU_PRINCIPAL;

    int fin_du_jeu=0;
    int choix = -1;
    int choix_map = -1;
    int joueurs = -1;
    int classes = -1;
    int anime_nuage=1;
    int lancer=0;
    for(int i=0;i<4;i++){
        choix_joueurs[i]=-1;
    }
    while (!fin_du_jeu) {
        switch (etat_actuel) {
            case MENU_PRINCIPAL:
                choix = afficher_menu_principal(menu,fond_nuage_bas,fond_nuage_haut);
                if (choix == 0) etat_actuel = CHOIX_MAP;
                else if (choix == 1) return 0;// RAJOUTER CHARGER PARTIE
                else if(choix==-2) fin_du_jeu=1;
                break;
            case CHOIX_MAP:
                choix_map = afficher_menu_map(menu,fond_nuage_bas,fond_nuage_haut,theme,aleatoire);
                if (choix_map == 0) etat_actuel = CHOIX_JOUEURS;
                else if (choix_map == -2) etat_actuel = MENU_PRINCIPAL;
                break;
            case CHOIX_JOUEURS:
                joueurs = afficher_choix_joueurs(menu,fond_nuage_bas,fond_nuage_haut,anime_nuage,nb_joueurs,&lancer,equipe);
                if(joueurs>=0&&joueurs<4){
                    etat_actuel= CHOIX_CLASSES;
                }
                if (joueurs == -2){
                    etat_actuel = CHOIX_MAP;
                    anime_nuage=1;
                }
                if(lancer){
                    destroy_bitmap(menu);
                    destroy_bitmap(fond_nuage_bas);
                    destroy_bitmap(fond_nuage_haut);
                    return 1;
                }
                break;
            case CHOIX_CLASSES:
                classes = afficher_classes_personnages(menu,fond_nuage_bas,fond_nuage_haut);
                if(classes>=0&&classes<4){
                    choix_joueurs[joueurs]=classes;
                    etat_actuel = CHOIX_JOUEURS;
                    anime_nuage=0;
                }
                if (classes == -2){
                    etat_actuel = CHOIX_JOUEURS;
                    anime_nuage=0;
                }
                break;

        }
    }
    destroy_bitmap(menu);
    destroy_bitmap(fond_nuage_bas);
    destroy_bitmap(fond_nuage_haut);
    return 0;
}
