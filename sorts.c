#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_potion init_inventaire (char nom_potion[20], int i) {
    // initialisation des potions
    t_potion pot;
    pot.x = 0;
    pot.y = 0;
    pot.xf = 0;
    pot.yf = 0;
    sprintf(nom_potion, "Images\\potion_%d.bmp", i+1);
    pot.img = load_bitmap(nom_potion, NULL);
    return pot;
}

void creer_potion (t_potion pot[NB_POTION], char nom_potion[20]) {
    for (int i = 0; i < NB_POTION; i++) {
        pot[i] = init_inventaire(nom_potion, i);
        // fonctions pour que ca soit dans des cases de l'inventaire
        pot[i].x = i*102+22;
        pot[i].y = SCREEN_H-92;
        pot[i].xf = pot[i].x+68;
        pot[i].yf = pot[i].y+68;
    }
}

void point_vie (BITMAP* buffer, t_potion pot[NB_POTION], int degats) {
    // il faudra remplacer les degats par les pv des joueurs dans les structures
    textprintf_ex(buffer, font, 724, SCREEN_H-70, makecol(0,0,0), -1, "%d", degats);
}

void afficher_inventaire (BITMAP* buffer, t_potion pot[NB_POTION], int degats) {
    for (int i = 0; i < NB_POTION; i++) {
        draw_sprite(buffer, pot[i].img,  pot[i].x, pot[i].y);
    }
    point_vie(buffer, pot, degats);
}

void souris_potion (BITMAP* buffer, t_potion pot[NB_POTION]) {// manque la structure personnage qui permetterait de recuperer me type de perso
    int souris = 0;
    // test pour savoir si les potions sont appuyées dans l'inventaire
    for (int i = 0; i < NB_POTION; i++) {
        if (mouse_x>pot[i].x && mouse_x<pot[i].xf && mouse_y>pot[i].y && mouse_y<pot[i].yf) {
            textout_ex(buffer, font, "Potion cliquée", 100, 100, makecol(255, 255, 255), -1);
            BITMAP* image = load_bitmap("Images\\potion_15.bmp", NULL);
            blit(image, buffer, 0, 0, 0, 0, image->w, image->h);
        } else {
            textout_ex(buffer, font, "Potion pas cliquée", 100, 120, makecol(255, 255, 255), -1);
        }
    }

}