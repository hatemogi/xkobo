
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

#ifndef XKOBO_H_XKOBO
#define XKOBO_H_XKOBO

#include "xlchip.h"
#include "xlcmap.h"

extern win_chip wchip;
extern win_cmap wbase;
extern win_backing wradar;
extern win_backing wscore;
extern int scale_log2;
extern int mouse_x, mouse_y;
extern int cheat_mode;
extern "C" {
    extern unsigned char spdata[];
}

#endif // XKOBO_H_XKOBO
