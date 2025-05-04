#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_potion init_inventaire (char nom_potion[20], int n) {
    // initialisation des potions
    t_potion pot;
    pot.x = 0;
    pot.y = 0;
    pot.xf = 0;
    pot.yf = 0;
    pot.pot_survol = 0;
    pot.degats = 0;
    sprintf(nom_potion, "Images\\potion_%d.bmp", n+1);
    pot.img = load_bitmap(nom_potion, NULL);
    return pot;
}

void equiper_potion (t_perso p[NB_PERSOS], char nom_potion[20]) {

    char noms[16][50] = {
        "Potion de foudre",
        "Potion de soin",
        "Potion de gel",
        "Potion de rage",
        "Potion de glace",
        "Potion de poison",
        "Potion de terre",
        "Potion d’air",
        "Potion de foudre",
        "Potion d’ombre",
        "Potion de lumière",
        "Potion magique",
        "Potion rouge",
        "Potion bleue",
        "Potion verte",
        "Potion dorée"
    }; 

    for (int i = 0; i < NB_PERSOS; i++) {
        for (int k = 0; k < NB_POTION; k++) {
            int index = (i * NB_PERSOS) + k;
            p[i].pot[k] = init_inventaire(nom_potion, index);
            p[i].pot[k].x = k*101+23;
            p[i].pot[k].y = SCREEN_H-92;
            p[i].pot[k].xf = p[i].pot[k].x+p[i].pot[k].img->w;
            p[i].pot[k].yf = p[i].pot[k].y+p[i].pot[k].img->h;
            p[i].pot[k].degats = (rand() % 21) + 5;
            strcpy(p[i].pot[k].intitule, noms[index]);
            p[i].pot[k].intitule[sizeof(p[i].pot[k].intitule) - 1] = '\0';
        }
    }
}

int potion_1 (int tour_perso) {
    if (mouse_x >= 23 && mouse_x <= 91 && mouse_y >= 508 && mouse_y <= 576) {
        return 0 + tour_perso * NB_POTION;
    }
    return -1;
}

int potion_2 (int tour_perso) {
    if (mouse_x >= 124 && mouse_x <= 192 && mouse_y >= 508 && mouse_y <= 576) {
        return 1 + tour_perso * NB_POTION;
    }
    return -1;
}

int potion_3 (int tour_perso) {
    if (mouse_x >= 225 && mouse_x <= 293 && mouse_y >= 508 && mouse_y <= 576) {
        return 2 + tour_perso * NB_POTION;
    }
    return -1;
}

int potion_4 (int tour_perso) {
    if (mouse_x >= 326 && mouse_x <= 394 && mouse_y >= 508 && mouse_y <= 576) {
        return 3 + tour_perso * NB_POTION;
    }
    return -1;
}

void point_vie (BITMAP* buffer, int degats) {
    // il faudra remplacer les degats par les pv des joueurs dans les structures
    textprintf_ex(buffer, font, 724, SCREEN_H-70, makecol(0,0,0), -1, "%d", degats);
}

void afficher_inventaire (BITMAP* buffer, int degats, t_perso p[NB_PERSOS], int tour_perso) {
    for (int k = 0; k < NB_POTION; k++) {
        draw_sprite(buffer, p[tour_perso].pot[k].img,  p[tour_perso].pot[k].x, p[tour_perso].pot[k].y);
    }
    textout_ex(buffer, font, "Potion :", 420, 510, makecol(255, 255, 255), -1);
    textout_ex(buffer, font, "Dégats :", 420, 545, makecol(255, 255, 255), -1);
    point_vie(buffer, degats);
}

void attaque_croix (BITMAP* buffer, t_case c[TAILLE_MAP][TAILLE_MAP], int x, int y, int portee) {
    for (int i = 1; i < portee+1; i++) { // sort en ligne
        remplir_losange(c[x-i][y], buffer, makecol(98, 107, 255));
        remplir_losange(c[x+i][y], buffer, makecol(98, 107, 255));
        remplir_losange(c[x][y-i], buffer, makecol(98, 107, 255));
        remplir_losange(c[x][y+i], buffer, makecol(98, 107, 255));
    }
}

void attaque_cercle(BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP], int x, int y, int rayon) {
    for (int i = x - rayon; i <= x + rayon; i++) {
        for (int j = y - rayon; j <= y + rayon; j++) {
            if (i >= 0 && i < TAILLE_MAP && j >= 0 && j < TAILLE_MAP) {
                int dx = i - x;
                int dy = j - y;
                if (dx*dx + dy*dy <= rayon*rayon) {
                    remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                }
            }
        }
    }
}

void attaque_anneau(BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP], int x, int y, int r_min, int r_max) {
    for (int i = x - r_max; i <= x + r_max; i++) {
        for (int j = y - r_max; j <= y + r_max; j++) {
            if (i >= 0 && i < TAILLE_MAP && j >= 0 && j < TAILLE_MAP) {
                int dx = i - x;
                int dy = j - y;

                int dist = dx*dx + dy*dy;

                if (dist >= r_min*r_min && dist <= r_max*r_max) {
                    remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                }
            }
        }
    }
}

void affichage_potions (BITMAP* buffer, t_perso p[NB_PERSOS], t_case c[TAILLE_MAP][TAILLE_MAP], int tour_perso) {
    char degats_potion[10];

    if (potion_1(tour_perso) == 0) {
        textout_ex(buffer, font, "Potion 1 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[0].intitule, 487, 508, makecol(255, 255, 25), -1);

        attaque_croix(buffer, c, p[0].ligne, p[0].colonne, 3);

    } else if (potion_2(tour_perso) == 1) {
        textout_ex(buffer, font, "Potion 2 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[1].intitule, 487, 508, makecol(255, 255, 25), -1);

        remplir_losange(c[p[tour_perso].ligne][p[tour_perso].colonne], buffer, makecol(98, 107, 255));

    } else if (potion_3(tour_perso) == 2) {
        textout_ex(buffer, font, "Potion 3 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[2].intitule, 487, 508, makecol(255, 255, 25), -1);

        attaque_cercle(buffer, c, p[0].ligne, p[0].colonne, 4);

    } else if (potion_4(tour_perso) == 3) {
        textout_ex(buffer, font, "Potion 4 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[3].intitule, 487, 508, makecol(255, 255, 25), -1);

        attaque_anneau(buffer, c, p[0].ligne, p[0].colonne, 2, 3 );

    } else if (potion_1(tour_perso) == 4) {
        textout_ex(buffer, font, "Potion 5 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[0].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_2(tour_perso) == 5) {
        textout_ex(buffer, font, "Potion 6 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[1].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_3(tour_perso) == 6) {
        textout_ex(buffer, font, "Potion 7 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[2].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_4(tour_perso) == 7) {
        textout_ex(buffer, font, "Potion 8 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[3].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_1(tour_perso) == 8) {
        textout_ex(buffer, font, "Potion 9 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[0].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_2(tour_perso) == 9) {
        textout_ex(buffer, font, "Potion 10 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[1].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_3(tour_perso) == 10) {
        textout_ex(buffer, font, "Potion 11 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[2].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_4(tour_perso) == 11) {
        textout_ex(buffer, font, "Potion 12 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[3].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_1(tour_perso) == 12) {
        textout_ex(buffer, font, "Potion 13 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[0].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_2(tour_perso) == 13) {
        textout_ex(buffer, font, "Potion 14 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[1].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_3(tour_perso) == 14) {
        textout_ex(buffer, font, "Potion 15 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[2].intitule, 487, 508, makecol(255, 255, 25), -1);
    } else if (potion_4(tour_perso) == 15) {
        textout_ex(buffer, font, "Potion 16 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 495, 545, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso].pot[3].intitule, 487, 508, makecol(255, 255, 25), -1);
    }
}
