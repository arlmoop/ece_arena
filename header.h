#ifndef HEADER_H
#define HEADER_H

#include <allegro.h>
#include <stdbool.h>

#define TAILLE_MAP 19
#define NB_IMG_PERSOS 1
#define NB_CASES 10
#define NB_OBS 8
#define NB_SPAWNS 4
#define Y_DEPART 10
#define PRCNT_OBS 8
#define NB_POTION 4
#define MENU_W 400
#define MENU_H 300
#define MENU_CLASSES_W 600
#define MENU_CLASSES_H 480
#define PM 3
#define NB_PERSOS 4
#define NB_FRAMES 10
#define TAILLE_CASE_X 42
#define TAILLE_CASE_Y 25


typedef struct {
    int x, y, tx, ty;
    BITMAP *img;
} t_spriteimmo;

typedef struct {
    int x, y, tx, ty, xcentre, ycentre, type, ycentre_losange, p;
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
    int x, y, dx, dy, tx, ty, xcentre, ycentre, classe, equipe, ligne, colonne, num;
    int imgcourante, cptimg, tmpimg, anim_en_cours, frames_restantes, nb_images;
    int pm, pa, pv;
    t_potion pot[NB_POTION];
    BITMAP *img[5];
}t_perso;

typedef struct {
    int ligne;
    int colonne;
} t_coord;

typedef enum {
    MENU_PRINCIPAL,
    CHOIX_MAP,
    CHOIX_JOUEURS,
    CHOIX_CLASSES
} EtatMenu;

typedef struct {
    int x, y, xf, yf;
    //xf et yf les coordonnees de la fin de la taille des images des potions
    BITMAP* img;
} t_potion;


//OUTILS.C
void initialisation_allegro();
void magenta (BITMAP* image, int r_max, int g_max, int b_max);


// MENU.C
t_spriteimmo initspriteimmo();
void disparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2);
void apparition_fond_nuage(BITMAP *menu,BITMAP *fond1, BITMAP *fond2);
int bouton_bas_droite(BITMAP *menu, int est_retour,int format_menu);
int afficher_menu_principal(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut);
int afficher_menu_map(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut,int *theme,int *etat_barre_aleatoire);
int afficher_choix_joueurs(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut, int n,int *nb_joueurs,int *lancer,int *equipe);
int afficher_classes_personnages(BITMAP *menu,BITMAP *fond_nuage_bas,BITMAP *fond_nuage_haut);
int menu(int *aleatoire,int *theme,int *nb_joueurs,int choix_joueurs[], int *equipe);


// PERSOS.C
t_perso init_perso(int n, int x, int y);
void placer_persos(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], bool equipe, int choix_joueurs[]);
void lancer_animation(t_perso *perso, t_coord chemin[], int *etape_courante, int nb_etapes);
void animer(t_perso *perso, t_coord chemin[], int *etape_courante, int nb_etapes);
void deplacement(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso,int ligne_actu, int colonne_actu, t_coord chemin[], int *nb_etapes, int *etape_courante);

// MAP.C
t_case init_case(int n, int x, int y);
t_obstacle init_obstacle(int n, int x, int y);
t_potion init_inventaire(char nom_potion[20], int i);
void creer_fichier();
void charger_fichier(int tab_map[TAILLE_MAP][TAILLE_MAP]);
void creer_map(int tab_map[TAILLE_MAP][TAILLE_MAP], t_case c[TAILLE_MAP][TAILLE_MAP], bool equipe, int nb_joueurs);
void afficher_map(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP]);
void creer_obstacles(t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP]);
void afficher_obstacles_persos(BITMAP *buffer, t_case c[TAILLE_MAP][TAILLE_MAP], t_obstacle obs[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS]);
int point_dans_losange(t_case c);
void remplir_losange(t_case c, BITMAP * buffer, int couleur);
void souris_tab(t_case c[TAILLE_MAP][TAILLE_MAP], BITMAP *buffer, int *ligne_prec, int *colonne_prec, int *ligne_actu, int*colonne_actu);
int comparer_coord(t_perso p, int ligne_actu, int colonne_actu, int *distance);
int chemin_valide(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu, int *distance);
void calculer_chemin(t_coord chemin[], int ligne_depart, int colonne_depart,int ligne_arrivee, int colonne_arrivee);
void afficher_chemin(t_case c[TAILLE_MAP][TAILLE_MAP], t_perso p[NB_PERSOS], int tour_perso, int ligne_actu, int colonne_actu,int *distance, BITMAP* buffer);
void afficher_pause(BITMAP *buffer, int *compteur);


// SORTS.C
void creer_potion (t_potion p[NB_POTION], char nom_potion[20]);
void point_vie (BITMAP* buffer, t_potion p[NB_POTION], int degats);
void afficher_inventaire (BITMAP* buffer, t_potion p[NB_POTION], int degats);
void souris_potion (BITMAP* buffer, t_potion p[NB_POTION], t_case c[TAILLE_MAP][TAILLE_MAP]);


#endif //HEADER_H
