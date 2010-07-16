
/*
 * XKOBO, a video-oriented game
 * Copyright (C) 1995,1996  Akira Higuchi
 *     a-higuti@math.hokudai.ac.jp
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

extern "C"{
#include <stdlib.h>
}
#include "xkobo.h"
#include "screen.h"
#include "myship.h"
#include "radar.h"

int _radar::sx       = MAP_SIZEX;
int _radar::sy       = MAP_SIZEY;
int _radar::mx_old;
int _radar::my_old;
int _radar::pixel_f0 = -1;
int _radar::pixel_f1 = -1;
int _radar::pixel_f2 = -1;
int _radar::pixel_b  = -1;

void _radar::set_a_pixel(int x, int y)
{
    if (scale_log2 == 0)
        wradar.point(x, y);
    else
        wradar.fillrectangle(x<<scale_log2, y<<scale_log2,
                             1<<scale_log2, 1<<scale_log2);
}

void _radar::prepare()
{
    int i, j;
    if (pixel_f0 < 0){
        pixel_f0 = wbase.alloc_color(128<<8, 240<<8, 240<<8);
        pixel_f1 = wbase.alloc_color(200<<8, 240<<8, 240<<8);
        pixel_f2 = wbase.alloc_color(64 <<8, 128<<8, 128<<8);
        pixel_b  = wbase.alloc_color(32 <<8, 48 <<8, 64 <<8);
    }
    wradar.foreground(pixel_f1);
    wradar.background(pixel_b);
    wradar.clear();
    for (i=0; i<sx; i++)
        for (j=0; j<sy; j++){
            int a = screen.get_chip_number(i, j);
            if (a == CORE){
                wradar.foreground(pixel_f1);
                _radar::set_a_pixel(i, j);
            }
            else if ((a==U_MASK) || (a==R_MASK) ||
                     (a==D_MASK) || (a==L_MASK)){
                wradar.foreground(pixel_f2);
                _radar::set_a_pixel(i, j);
            }
            else if ((a & HIT_MASK)){
                wradar.foreground(pixel_f2);
                _radar::set_a_pixel(i, j);
            }
        }
    wradar.back();
    mx_old = -1;
    my_old = -1;
}

void _radar::erase(int x, int y)
{
    wradar.foreground(pixel_b);
    _radar::set_a_pixel(x, y);
}

void _radar::trace_myship()
{
    int mx_new = (myship.get_x() & (SCREEN_SIZEX-1)) >> 4;
    int my_new = (myship.get_y() & (SCREEN_SIZEY-1)) >> 4;
    if ((mx_new == mx_old) && (my_new == my_old)) return;
    if (screen.get_chip_number(mx_new, my_new) != SPACE) return;
    wradar.foreground(pixel_f0);
    _radar::set_a_pixel(mx_new, my_new);
    if (mx_old >= 0){
        wradar.foreground(pixel_b);
        _radar::set_a_pixel(mx_old, my_old);
    }
    mx_old = mx_new;
    my_old = my_new;
}
