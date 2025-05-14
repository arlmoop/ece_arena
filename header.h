#ifndef HEADER_H
#define HEADER_H

#include <allegro.h>
#include <stdbool.h>

#define TAILLE_MAP 19 //Impair pr bien placer les spawns d'equipe
#define NB_IMG_PERSOS 1 //Nb de sprite par perso (mieux si tt pareil)
#define NB_CASES 10 //Nb de cases hors spawns
#define NB_OBS 8
#define NB_SPAWNS 4
#define Y_DEPART 10
#define PRCNT_OBS 8
#define NB_POTION 4 // nombre de potion par perso
#define MENU_W 400
#define MENU_H 300
#define MENU_CLASSES_W 600
#define MENU_CLASSES_H 480
#define PM 3
#define NB_PERSOS 4

typedef struct {
    int ligne, colonne;
}t_coord;

typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, ycentre_losange, p, num_joueur;
    bool o, r;
    // TYPE 1:terre 2:autre
    BITMAP *img;
} t_case;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre;
    bool e, a;
    // E 0:existe pas 1:existe
    // A 0:n'a pas ete affiche 1:a ete affiche
    BITMAP *img;
} t_obstacle;

typedef struct {
    int x, y, xf, yf;
    int pot_survol; // si la souris est au dessus de la potion
    //xf et yf les coordonnees de la fin de la taille des images des potions
    char intitule[50];
    int degats;
    BITMAP* img;
} t_potion;

typedef struct {
    int x, y, dx, dy, tx, ty, xcentre, ycentre, classe, equipe, ligne, colonne, num;
    int imgcourante, cptimg, tmpimg, anim_en_cours, frames_restantes, nb_images;
    int pm, pa, pv;
    t_potion pot[NB_POTION];
    BITMAP *img[5];
}t_perso;

typedef enum {
    MENU_PRINCIPAL,
    CHOIX_MAP,
    CHOIX_JOUEURS,
    CHOIX_CLASSES
} EtatMenu;


//OUTILS.C
void initialisation_allegro();
void magenta (BITMAP* image, int r_max, int g_max, int b_max);
int clic_gauche(int x1, int y1, int x2, int y2);
int clic_droit(int x1, int y1, int x2, int y2);


// MENU.C
t_spriteimmo initspriteimmo();
void disparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2);
void apparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2);
int bouton_bas_droite(BITMAP *menu, int est_retour,int format_menu);
int afficher_menu_principal(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut, BITMAP *fond_menu);
int afficher_menu_map(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut,int *theme,int *etat_barre_aleatoire, BITMAP *fond_menu);
int afficher_choix_joueurs(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut, int n,int *nb_joueurs,int *lancer,int *equipe, BITMAP *fond_menu);
int afficher_classes_personnages(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut, BITMAP *fond_menu);
int menu(int *aleatoire,int *theme,int *nb_joueurs,int choix_joueurs[], int *equipe);


// PERSOS.C
t_perso init_perso(int n, int x, int y);
void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int choix_joueurs[]);
void deplacement (t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu, int *distance);
void animer(t_perso* perso, bool *valider_pm, int *distance);
void gerer_tours(int *tour_perso, t_perso *p, bool *valider_pm, bool *valider_pa, bool *passer_tour, int nb_joueurs);
void passer(bool *passer_tour, BITMAP *buffer);
void val_pa(bool *valider_pa, BITMAP *buffer);


// MAP.C
t_case init_case(int n, int x, int y);
t_obstacle init_obstacle(int n, int x, int y);
t_potion init_inventaire(char nom_potion[20], int i);
void creer_fichier(char *nom_fichier);
void charger_fichier(char *nom_fichier, int tab_map[TAILLE_MAP][TAILLE_MAP]);
void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], int equipe, int nb_joueurs);
void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]);
void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);
void afficher_obstacles_persos(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS]);
int point_dans_losange(t_case c);
void remplir_losange(t_case c, BITMAP * buffer, int couleur);
void souris_tab(t_case c[TAILLE_MAP][TAILLE_MAP], BITMAP *buffer, int *ligne_prec, int *colonne_prec, int *ligne_actu, int*colonne_actu);
int comparer_coord(t_perso p, int ligne_actu, int colonne_actu, int *distance);
int chemin_valide(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu, int *distance);
void calculer_chemin(t_coord chemin[], int ligne_depart, int colonne_depart,int ligne_arrivee, int colonne_arrivee);
void afficher_chemin(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso,int ligne_actu, int colonne_actu,int *distance, BITMAP* buffer);
void recommencer(int *degats, char nom_potion[], int *ligne_prec, int *ligne_actu,
                int *colonne_prec, int *colonne_actu, int *compteur,
                bool *valider_pm, bool *valider_pa, bool *passer_tour,
                int *tour_perso, int *nb_joueurs, int *distance,
                int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int *equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS],
                int choix_joueurs[], int *tour_depart, bool map, clock_t *depart, clock_t *tps_pause);
void afficher_pause(BITMAP *buffer, int *compteur, int *degats, char nom_potion[], int *ligne_prec, int *ligne_actu,
                int *colonne_prec, int *colonne_actu,
                bool *valider_pm, bool *valider_pa, bool *passer_tour,
                int *tour_perso, int *nb_joueurs, int *distance,
                int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP],
                int *equipe, t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS],
                int choix_joueurs[], int *tour_depart, clock_t *depart, clock_t *pause, clock_t *tps_pause);
void timer(char texte[50], BITMAP *buffer, double *secondes, clock_t depart, clock_t tps_pause);


// SORTS.C
void equiper_potion (t_perso p[NB_PERSOS], char nom_potion[20]);
void point_vie (BITMAP* buffer, int degats);
void afficher_inventaire (BITMAP* buffer, int degats, t_perso p[NB_PERSOS], int tour_perso);
void affichage_potions (BITMAP *buffer, t_perso *perso,t_case c[TAILLE_MAP][TAILLE_MAP], int tour_perso);
int potion_1 (int tour_perso);
int potion_2 (int tour_perso);
int potion_3 (int tour_perso);
int potion_4 (int tour_perso);


#endif //HEADER_H
