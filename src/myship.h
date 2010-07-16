
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

#ifndef XKOBO_H_MYSHIP
#define XKOBO_H_MYSHIP

#include "config.h"

#define SHIFT    6                               /* fixed pointer */
#define ABS(x)   (((x)>=0) ? (x) : (-(x)))
#define MAX(x,y) (((x)>(y)) ? (x) : (y))

//---------------------------------------------------------------------------//
enum _myship_state { normal, dead };
class _myship{
    static _myship_state state;
    static int di;      /* direction */
    static int virtx, virty; /* scroll position */
    static int x, y;
    static int lapx, lapy;
    static int beamx[BEAM_MAX],beamy[BEAM_MAX];
    static int beamdi[BEAM_MAX],beamst[BEAM_MAX];
  public:
    static inline int get_x(){ return x;}
    static inline int get_y(){ return y;}
    static inline int get_virtx(){ return virtx;}
    static inline int get_virty(){ return virty;}
    static int init();
    static int move();
    static int put();
    static int shot();
    static int hit_structure();
    static int hit_beam(int ex, int ey, int hitsize);
    static void destroyed();
    static void set_position(int px, int py);
};

extern _myship    myship;

#endif // XKOBO_H_MYSHIP
