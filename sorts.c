#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>


t_potion init_inventaire (char nom_potion[20], int n) {
    // initialisation des potions
    t_potion pot;
    pot.x = pot.y = pot.xf = pot.yf = 0;
    pot.degats = 0;
    sprintf(nom_potion, "Images\\potion_%d.bmp", n+1);
    pot.img = load_bitmap(nom_potion, NULL);
    pot.pa = 0;
    pot.degat_plus = pot.degat_min = pot.degat_pourcent = 0;
    pot.type=0;
    return pot;
}

void equiper_potion (t_perso p[NB_PERSOS], char nom_potion[20]) {

    int index = 0;

    char noms_potion[16][50] = {
        "Potion électrique",
        "Potion de soin",
        "Potion de gel",
        "Potion de rage",
        "Potion sacrée",
        "Potion toxique",
        "Potion naturelle",
        "Potion chauve-souris",
        "Potion d’illusion",
        "Potion de rapidité",
        "Potion toxique",
        "Potion de puissance",
        "Potion des ténèbres",
        "Potion temporelle",
        "Potion de séisme",
        "Potion squelettique"
    };

    int degats_potion[NB_PERSOS*NB_POTION] = {25, 0, 15, 30, 0, 15, 25, 40, 40, 0, 25, 35, 25, 40, 15, 35};

    int degats_potion_plus[NB_PERSOS*NB_POTION] = {5, 0, 8, 6, 0, 8, 5, 6, 6, 0, 8, 6, 5, 6, 8, 6};

    int pa_potion[NB_PERSOS*NB_POTION] = {3, 2, 2, 3, 2, 2, 3, 5, 5, 2, 3, 3, 3, 5, 2, 3};

    for (int i = 0; i < NB_PERSOS; i++) {

        if (p[i].classe == 1) {
            index = 0;
            //p[i].pot[1].type = 1;
        } else if (p[i].classe == 2) {
           index = 4;
            //p[i].pot[0].type = 2;
        } else if (p[i].classe == 3) {
            index = 8;
            //p[i].pot[1].type = 3;
        } else if (p[i].classe == 4) {
            index = 12;
        }
        for (int k = 0; k < NB_POTION; k++) {
            p[i].pot[k] = init_inventaire(nom_potion, k+index);
            p[i].pot[k].x = k*101+23;
            p[i].pot[k].y = SCREEN_H-92;
            p[i].pot[k].xf = p[i].pot[k].x+p[i].pot[k].img->w;
            p[i].pot[k].yf = p[i].pot[k].y+p[i].pot[k].img->h;

            strcpy(p[i].pot[k].intitule, noms_potion[k+index]);
            p[i].pot[k].intitule[sizeof(p[i].pot[k].intitule) - 1] = '\0';

            p[i].pot[k].degat_min = degats_potion[k+index];

            p[i].pot[k].degat_plus = degats_potion_plus[k+index];

            p[i].pot[k].pa = pa_potion[k+index];

            if (p[i].pot[k].degat_plus > 0) {
                p[i].pot[k].degats = p[i].pot[k].degat_min + rand()%(p[i].pot[k].degat_plus);
            } else if (p[i].pot[k].degat_plus <= 0) {
                p[i].pot[k].degats = p[i].pot[k].degat_min;
            }
        }
    }
}

int potion_1 (t_perso p[NB_PERSOS], int tour_perso) {
    if (mouse_x >= 23 && mouse_x <= 91 && mouse_y >= 508 && mouse_y <= 576) {
        return 0 + (p[tour_perso-1].classe-1) * NB_POTION;
    }
    return -1;
}

int potion_2 (t_perso p[NB_PERSOS], int tour_perso) {
    if (mouse_x >= 124 && mouse_x <= 192 && mouse_y >= 508 && mouse_y <= 576) {
        return 1 + (p[tour_perso-1].classe-1) * NB_POTION;
    }
    return -1;
}

int potion_3 (t_perso p[NB_PERSOS], int tour_perso) {
    if (mouse_x >= 225 && mouse_x <= 293 && mouse_y >= 508 && mouse_y <= 576) {
        return 2 + (p[tour_perso-1].classe-1) * NB_POTION;
    }
    return -1;
}

int potion_4 (t_perso p[NB_PERSOS], int tour_perso) {
    if (mouse_x >= 326 && mouse_x <= 394 && mouse_y >= 508 && mouse_y <= 576) {
        return 3 + (p[tour_perso-1].classe-1) * NB_POTION;
    }
    return -1;
}

void point_vie (BITMAP* buffer, t_perso p[NB_PERSOS], int tour_perso) {
    char pv_joueur[10];
    sprintf(pv_joueur, "%d", p[tour_perso-1].pv);
    textout_ex(buffer, font, pv_joueur, 724, SCREEN_H-70, makecol(255, 255, 250), -1);
}

void afficher_inventaire (BITMAP* buffer, t_perso p[NB_PERSOS], int tour_perso) {
    for (int k = 0; k < NB_POTION; k++) {
        draw_sprite(buffer, p[tour_perso-1].pot[k].img, p[tour_perso-1].pot[k].x, p[tour_perso-1].pot[k].y);
    }
    textout_ex(buffer, font, "Potion :", 420, 510, makecol(255, 255, 255), -1);
    textout_ex(buffer, font, "Dégats :", 420, 540, makecol(255, 255, 255), -1);
    textout_ex(buffer, font, "PA :", 420, 570, makecol(255, 255, 255), -1);
    point_vie(buffer, p, tour_perso);
}

void attaque_croix(BITMAP* buffer, t_case c[TAILLE_MAP][TAILLE_MAP], int tab_attaque[TAILLE_MAP][TAILLE_MAP], int x, int y, int portee) {

    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            int dx = abs(i - x);
            int dy = abs(j - y);

            if ((dx == 0 || dy == 0) && dx + dy <= portee && !(dx == 0 && dy == 0)) {
                remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                tab_attaque[i][j] = 1;
            }
        }
    }
}

void attaque_cercle(BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP],int tab_attaque[TAILLE_MAP][TAILLE_MAP], int x, int y, int rayon) {

    for (int i = x - rayon; i <= x + rayon; i++) {
        for (int j = y - rayon; j <= y + rayon; j++) {
            if (i >= 0 && i < TAILLE_MAP && j >= 0 && j < TAILLE_MAP) {
                int dx = i - x;
                int dy = j - y;
                if (dx*dx + dy*dy <= rayon*rayon && !(dx == 0 && dy == 0)) {
                    remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                    tab_attaque[i][j] = 1;
                }
            }
        }
    }
}

void tableau_aleatoire (int tab[TAILLE_MAP][TAILLE_MAP], int n) {
    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            tab[i][j] = rand()%n;
        }
    }
}

void attaque_cercle_aleatoire(BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP],int tab_attaque[TAILLE_MAP][TAILLE_MAP], int tab_aleatoire[TAILLE_MAP][TAILLE_MAP], int x, int y, int rayon) {

    for (int i = x - rayon; i <= x + rayon; i++) {
        for (int j = y - rayon; j <= y + rayon; j++) {
            if (i >= 0 && i < TAILLE_MAP && j >= 0 && j < TAILLE_MAP) {
                int dx = i - x;
                int dy = j - y;
                if (dx*dx + dy*dy <= rayon*rayon && !(dx == 0 && dy == 0) && tab_aleatoire[i][j] == 0) {
                    remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                    tab_attaque[i][j] = 1;
                }
            }
        }
    }
}

void attaque_anneau(BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP], int tab_attaque[TAILLE_MAP][TAILLE_MAP], int x, int y, int r_min, int r_max) {

    for (int i = x - r_max; i <= x + r_max; i++) {
        for (int j = y - r_max; j <= y + r_max; j++) {
            if (i >= 0 && i < TAILLE_MAP && j >= 0 && j < TAILLE_MAP) {
                int dx = i - x;
                int dy = j - y;

                int dist = dx*dx + dy*dy;

                if (dist >= r_min*r_min && dist <= r_max*r_max) {
                    remplir_losange(c[i][j], buffer, makecol(98, 107, 255));
                    tab_attaque[i][j] = 1;
                }
            }
        }
    }
}

void attaque_sur_perso (BITMAP* buffer,t_case c[TAILLE_MAP][TAILLE_MAP], int tab_attaque[TAILLE_MAP][TAILLE_MAP],int x,int y) {

    remplir_losange(c[x][y], buffer, makecol(98, 107, 255));
    tab_attaque[x][y] = 2;
}

void attaque_potion (t_perso p[NB_PERSOS], t_case c[TAILLE_MAP][TAILLE_MAP], int tab_attaque[TAILLE_MAP][TAILLE_MAP], int tour_perso, int numero_potion, int equipe) {

    //int equipe_joueur = p[tour_perso-1].num+1%4;

    for (int i = 0; i < TAILLE_MAP; i++) {
        for (int j = 0; j < TAILLE_MAP; j++) {
            if (c[i][j].p > 0 && tab_attaque[i][j] > 0) {

                if (equipe == 1) {
                    for (int k = 0; k < NB_PERSOS; k++) {
                        if (p[k].colonne == j && p[k].ligne == i && p[tour_perso-1].equipe != p[k].equipe) {
                            p[k].pv = p[k].pv - p[tour_perso-1].pot[numero_potion].degats;
                        }
                    }
                } else if (equipe == 0) {
                    for (int k = 0; k < NB_PERSOS; k++) {
                        if (p[k].colonne == j && p[k].ligne == i ) {
                            p[k].pv = p[k].pv - p[tour_perso-1].pot[numero_potion].degats;
                        }
                    }
                }

            }
        }
    }
}

void affichage_potions (BITMAP* buffer, t_perso p[NB_PERSOS], t_case c[TAILLE_MAP][TAILLE_MAP],int tab_attaque[TAILLE_MAP][TAILLE_MAP], int tab_aleatoire[TAILLE_MAP][TAILLE_MAP], int tour_perso, int numero_potion, int equipe) {
    char degats_potion[10];
    char pa_potion[5];
    int joueur_attaque=0;

    if ((mouse_b & 1 || mouse_b & 2) == 0) {
        for (int i = 0; i < TAILLE_MAP; i++) {
            for (int j = 0; j < TAILLE_MAP; j++) {
                tab_attaque[i][j] = 0;
            }
        }
    }

    afficher_inventaire(buffer, p, tour_perso);

    if (potion_1(p, tour_perso) == 0 && p[tour_perso-1].classe == 1) {
        joueur_attaque=1;
        numero_potion = 0;
        textout_ex(buffer, font, "Potion 1 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[0].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[0].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_croix(buffer, c,tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 3);

    } else if (potion_2(p, tour_perso) == 1 && p[tour_perso-1].classe == 1) {
        joueur_attaque=1;
        numero_potion = 1;
        p[tour_perso-1].pot[numero_potion].type = 1;
        textout_ex(buffer, font, "Potion 2 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[1].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[1].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_sur_perso(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne);

    } else if (potion_3(p, tour_perso) == 2 && p[tour_perso-1].classe == 1) {
        joueur_attaque=1;
        numero_potion = 2;
        textout_ex(buffer, font, "Potion 3 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[2].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[2].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 3);

    } else if (potion_4(p, tour_perso) == 3 && p[tour_perso-1].classe == 1) {
        joueur_attaque=1;
        numero_potion = 3;
        textout_ex(buffer, font, "Potion 4 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[3].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[3].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_anneau(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 3, 4 );

    } else if (potion_1(p, tour_perso) == 4 && p[tour_perso-1].classe == 2) {
        joueur_attaque=1;
        numero_potion = 0;
        p[tour_perso-1].pot[numero_potion].type = 2;
        textout_ex(buffer, font, "Potion 5 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[0].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[0].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_sur_perso(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne);

    } else if (potion_2(p, tour_perso) == 5) {
        joueur_attaque=1;
        numero_potion = 1;
        textout_ex(buffer, font, "Potion 6 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[1].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[1].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 5);

    } else if (potion_3(p, tour_perso) == 6) {
        joueur_attaque=1;
        numero_potion = 2;
        textout_ex(buffer, font, "Potion 7 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[2].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[2].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_croix(buffer, c,tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 4);

    } else if (potion_4(p, tour_perso) == 7) {
        joueur_attaque=1;
        numero_potion = 3;
        textout_ex(buffer, font, "Potion 8 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[3].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[3].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle_aleatoire(buffer, c,tab_attaque, tab_aleatoire, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 5);

    } else if (potion_1(p, tour_perso) == 8) {
        joueur_attaque=1;
        numero_potion = 0;
        textout_ex(buffer, font, "Potion 9 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[0].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[0].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle_aleatoire(buffer, c,tab_attaque, tab_aleatoire, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 4);

    } else if (potion_2(p, tour_perso) == 9) {
        joueur_attaque=1;
        numero_potion = 1;
        p[tour_perso-1].pot[numero_potion].type = 3;
        textout_ex(buffer, font, "Potion 10 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[1].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[1].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_sur_perso(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne);

    } else if (potion_3(p, tour_perso) == 10) {
        joueur_attaque=1;
        numero_potion = 2;
        textout_ex(buffer, font, "Potion 11 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[2].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[2].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 3);

    } else if (potion_4(p, tour_perso) == 11) {
        joueur_attaque=1;
        numero_potion = 3;
        textout_ex(buffer, font, "Potion 12 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[3].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[3].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_anneau(buffer, c, tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 2, 4);

    } else if (potion_1(p, tour_perso) == 12) {
        joueur_attaque=1;
        numero_potion = 0;
        textout_ex(buffer, font, "Potion 13 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[0].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[0].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[0].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_croix(buffer, c,tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 2);

    } else if (potion_2(p, tour_perso) == 13) {
        joueur_attaque=1;
        numero_potion = 1;
        textout_ex(buffer, font, "Potion 14 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[1].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[1].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[1].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle_aleatoire(buffer, c,tab_attaque, tab_aleatoire, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 3);

    } else if (potion_3(p, tour_perso) == 14) {
        joueur_attaque=1;
        numero_potion = 2;
        textout_ex(buffer, font, "Potion 15 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[2].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[2].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[2].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_cercle(buffer, c,tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 5);

    } else if (potion_4(p, tour_perso) == 15) {
        joueur_attaque=1;
        numero_potion = 3;
        textout_ex(buffer, font, "Potion 16 survolée", 100, 100, makecol(255, 255, 255), -1);
        sprintf(degats_potion, "%d", p[tour_perso-1].pot[3].degats);
        textout_ex(buffer, font, degats_potion, 493, 540, makecol(255, 255, 25), -1);
        textout_ex(buffer, font, p[tour_perso-1].pot[3].intitule, 490, 510, makecol(255, 255, 25), -1);
        sprintf(pa_potion, "%d", p[tour_perso-1].pot[3].pa);
        textout_ex(buffer, font, pa_potion, 460, 570, makecol(255, 255, 25), -1);

        attaque_anneau(buffer, c,tab_attaque, p[tour_perso-1].ligne, p[tour_perso-1].colonne, 2, 3);

    }

    static int bouton_appuye_avant = 0;
    int bouton_appuye = mouse_b & 1 || mouse_b & 2;

    if (joueur_attaque==1 && bouton_appuye && !bouton_appuye_avant && (p[tour_perso-1].pa >= p[tour_perso-1].pot[numero_potion].pa)) {

        int chance_fail_attaque = rand()%2;

        if (chance_fail_attaque>0) {
            attaque_potion(p, c, tab_attaque, tour_perso, numero_potion, equipe);

            if (p[tour_perso-1].pot[numero_potion].type == 1) {
                p[tour_perso-1].pv += 20;
            } else if (p[tour_perso-1].pot[numero_potion].type == 2) {
                for (int i = 0; i < NB_POTION; i++) {
                    if (p[tour_perso-1].pot[i].type == 0) {
                        p[tour_perso-1].pot[i].degats += rand()%4+2;
                    }
                }
            } else if (p[tour_perso-1].pot[numero_potion].type == 3) {
                p[tour_perso-1].pm += 2;
            }
        }

        p[tour_perso-1].pa -= p[tour_perso-1].pot[numero_potion].pa;
    }

    //printf("%d", p[tour_perso-1].pot[numero_potion].type);

    bouton_appuye_avant = bouton_appuye;
}