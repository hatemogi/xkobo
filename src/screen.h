
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

#ifndef XKOBO_H_SCREEN
#define XKOBO_H_SCREEN

#include "map.h"

class _screen{
    static int scene_num;
    static int level;
    static int generate_count;
    static _map map;
    static void title();
    static void copy_a_chip(int n, int posx, int posy);
    static int scene_max;
    static int pixel_f0, pixel_f1, pixel_f2, pixel_s[8];
  public:
    static void init();
    static void init_scene(int sc);
    static int prepare();
    static void generate_fixed_enemies();
    static int get_chip_number(int x, int y);
    static void set_chip_number(int x, int y, int n);
};

extern _screen screen;

#endif // XKOBO_H_SCREEN
