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