
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

#include "xkobo.h"
#include "screen.h"
#include "manage.h"
#include "enemies.h"
#include "myship.h"
#include "key.h"
#include "radar.h"
#include "scenes.h"
#include "config.h"
#include "random.h"
#include "version.h"

int  _screen::scene_max;
int  _screen::scene_num;
int  _screen::level;
int  _screen::generate_count;
_map _screen::map;
int  _screen::pixel_f0                             = -1;
int  _screen::pixel_f1                             = -1;
int  _screen::pixel_f2                             = -1;
int  _screen::pixel_s[8];

void _screen::copy_a_chip(int n, int posx, int posy)
{
    int x = n, y = 7;
    if (n & HARD){
        y = 6;
        if (n & U_MASK) x = 8;
        else if (n & R_MASK) x = 9;
        else if (n & D_MASK) x = 10;
        else if (n & L_MASK) x = 11;
    }
    else if (n & CORE){
        y = 6;
        x = 15;
    }
    wchip.torus_copy_from_chip_and_store(x<<(CHIP_SIZEX_LOG2+scale_log2),
                                         y<<(CHIP_SIZEY_LOG2+scale_log2),
                                         CHIP_SIZEX<<scale_log2,
                                         CHIP_SIZEY<<scale_log2,
                                         posx<<(CHIP_SIZEX_LOG2+scale_log2),
                                         posy<<(CHIP_SIZEY_LOG2+scale_log2));
}

void _screen::init()
{
    scene_max = 0;
    while (scene[scene_max].ratio != -1) scene_max++;
    if (pixel_f0 < 0){
        pixel_f0 = wbase.alloc_color(200<<8, 255<<8, 255<<8);
        pixel_f1 = wbase.alloc_color(140<<8, 200<<8, 200<<8);
        pixel_f2 = wbase.alloc_color(100<<8, 200<<8, 100<<8);
        int i;
        for (i=0; i<8; i++){
            pixel_s[i] = wbase.alloc_color(40000, 15000+i*4000, 50000-i*5000);
        }
    }
}

void _screen::title()
{
    char s[256];
    int pos = (WSIZE/2)*((1<<scale_log2)-1);
    manage.get_highscore(s);
    wchip.clear();
    wchip.foreground(pixel_f0);
    wchip.font("-adobe-new century "
               "schoolbook-bold-r-normal--24-240-75-75-p-149-*");
    wchip.string_back(65+pos, 55+pos, "XKOBO");
    wchip.foreground(pixel_f1);
    wchip.font(NORMAL_FONT);
    wchip.string_back(83+pos, 70+pos, XKOBO_VERSION);
    wchip.string_back(27+pos, 90+pos, XKOBO_DATE);
    if (cheat_mode)
        wchip.string_back(80+pos, 150+pos, "cheat mode");
    else
        wchip.string_back(45+pos, 150+pos, s);
    wchip.foreground(pixel_f2);
    wchip.string_back(30+pos, 175+pos, "push 's' key to start a game");
    wchip.string_back(45+pos, 190+pos, "'n','m' to choose a stage");
    wchip.string_back(85+pos, 205+pos, "'q' to quit");
    wchip.set_position(0, 0);
    map.init();
    radar.prepare();
}

void _screen::init_scene(int sc)
{
    if (sc < 0){
    	scene_num = -1;
        _screen::title();
        return;
    }
    scene_num = sc % scene_max;
    level     = sc / scene_max;
    _scene *s = &scene[scene_num];
    
    map.init();
    int i;
    for (i=0; i<s->base_max; i++)
      map.make_maze(s->base[i].x, s->base[i].y, s->base[i].h, s->base[i].v);
    map.convert(s->ratio);
    
    generate_count = 0;
}

int _screen::prepare()
{
    if (scene_num < 0) return 0;
    _scene *s = &scene[scene_num];
    int i,j;
    int count_core=0;
    int c=0;
    
    int interval_1, interval_2;
    interval_1 = (s->ek1_interval) >> level;
    interval_2 = (s->ek2_interval) >> level;
    if (interval_1 < 4) interval_1 = 4;
    if (interval_2 < 4) interval_2 = 4;
    enemies.set_ekind_to_generate(s->ek1, interval_1, s->ek2, interval_2);
    
    wchip.clear();
    for (i=0; i<MAP_SIZEX; i++)
      for (j=0; j<MAP_SIZEY; j++){
          int m = map.pos(i, j);
          if (m) _screen::copy_a_chip(m, i, j);
          if ((m==U_MASK) || (m==R_MASK) || (m==D_MASK) || (m==L_MASK)){
              enemies.make(&cannon, i*16+8, j*16+8);
              c++;
          }
          else if (m == CORE){
              enemies.make(&core, i*16+8, j*16+8);
              count_core++;
              c++;
          }
      }
    
    int x, y;
    for (i=0; i<8; i++){
        wchip.foreground(pixel_s[i]);
        for (j=0; j<2000; j++){
            x = rand_num.get(SCREEN_SIZEX_LOG2);
            y = rand_num.get(SCREEN_SIZEY_LOG2);
            if (map.pos(x>>4, y>>4) == SPACE)
                wchip.fillrectangle_back(x<<scale_log2, y<<scale_log2,
                                         1<<scale_log2, 1<<scale_log2);
        }
    }
    wchip.torus();
    wchip.store();

    myship.set_position(s->startx<<4, s->starty<<4);

    radar.prepare();
    return count_core;
}

void _screen::generate_fixed_enemies()
{
    static int sint[16] =
      { 0, 12, 23, 30, 32, 30, 23, 12, 0, -12, -23, -30, -32, -30, -23, -12};
    static int cost[16] =
      { 32, 30, 23, 12, 0, -12, -23, -30, -32, -30, -23, -12, 0, 12, 23, 30};
    _scene *s = &scene[scene_num];
    if (generate_count < s->enemy_max){
        int j;
        for (j=0; j<s->enemy[generate_count].num; j++){
            int sp = s->enemy[generate_count].speed;
            int x, y, h, v, t;
            x = rand_num.get() % (SCREEN_SIZEX - WSIZE * 2);
            y = rand_num.get() % (SCREEN_SIZEY - WSIZE * 2);
            x -= (SCREEN_SIZEX / 2 - WSIZE);
            y -= (SCREEN_SIZEY / 2 - WSIZE);
            if (x < 0) x -= WSIZE;
            else x += WSIZE;
            if (y < 0) y -= WSIZE;
            else y += WSIZE;
            x += myship.get_x();
            y += myship.get_y();

            t = rand_num.get(4);
            h = (sp * sint[t]) << (SHIFT-6);
            v = (sp * cost[t]) << (SHIFT-6);
            enemies.make(s->enemy[generate_count].kind, x, y, h, v);
        }
        generate_count++;
    }
    if (generate_count >= s->enemy_max)
      generate_count = 0;
}

int _screen::get_chip_number(int x, int y)
{
    return map.pos(x, y);
}

void _screen::set_chip_number(int x, int y, int n)
{
    map.pos(x, y) = n;
    _screen::copy_a_chip(n, x, y);
    if (n == 0) radar.erase(x, y);
}
