
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

#ifndef XKOBO_H_XLCHIP
#define XKOBO_H_XLCHIP
 
#include "xlscroll.h"

/*
 *
 *      -----  win_scroll ---- win_chip
 *
 *
 */

#define SPRITE_MAX            1024

class win_cmap;
class win_chip : public win_scroll{

  protected:
    GC set_gc, and_gc, or_gc;
    int csx,csy;
    Pixmap p_store;
    Pixmap chip, chip_mask;
    struct _sprite{
        int cx, cy, x, y, h, v;
    } sprite[SPRITE_MAX];
    _sprite *spr_max;
    int store_policy;
    int clip(int& cx, int& cy, int& x, int& y, int& h, int& v);
    
  public:
    win_chip(); 
    ~win_chip();
    void make(win *back, int wx, int wy, int sizex, int sizey,
              int vsizex, int vsizey, int csizex, int csizey, int policy);
    void torus_copy_from_chip_and_store(int x, int y, int h, int v,
                                        int x1, int x2);
    void copy_from_chip_and_store(int x, int y, int h, int v,
                                  int x1, int x2);
    inline void copy_from_chip_sprite(int x, int y, int h, int v,
                                      int x1, int x2);
    void store();
    void set_position(int vposx, int vposy);
    friend int link_tiff(unsigned char *data, win_cmap& wcm, win_chip& wch);
};

inline void win_chip::copy_from_chip_sprite(int x, int y, int h, int v,
                                            int x1, int x2)
{
    if (spr_max >= sprite + SPRITE_MAX) return;
    spr_max->cx= x;
    spr_max->cy= y;
    spr_max->x = x1;
    spr_max->y = x2;
    spr_max->h = h;
    spr_max->v = v;
    spr_max++;
}

#endif // XKOBO_H_XLCHIP
