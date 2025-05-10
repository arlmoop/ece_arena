#include "header.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro.h>

void magenta (BITMAP* image, int r_max, int g_max, int b_max) {
    for (int j = 0; j<SCREEN_W; j++) {
        for (int i = 0; i<SCREEN_H; i++) {
            int c = getpixel(image, j, i);
            if (getr(c) > r_max && getg(c) < g_max && getb(c) > b_max) {
                putpixel(image, j, i, makecol(255, 0, 255));
            }
        }
    }
    save_bitmap("Images\\serpent11.bmp", image,NULL);
}


void initialisation_allegro() {
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

int clic_gauche(int x1, int y1, int x2, int y2) {
    int r=0;
    if(mouse_b&1 && mouse_x<x2 && mouse_x>x1 && mouse_y<y2 && mouse_y>y1)
        r=1;
    return r;
}

int clic_droit(int x1, int y1, int x2, int y2) {
    int r=0;
    if(mouse_b&2 && mouse_x<x2 && mouse_x>x1 && mouse_y<y2 && mouse_y>y1)
        r=1;
    return r;
}