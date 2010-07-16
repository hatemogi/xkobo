
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

#ifndef XKOBO_H_RADAR
#define XKOBO_H_RADAR


class _radar{
    static int sx, sy;
    static int mx_old, my_old;
    static int pixel_f0, pixel_f1, pixel_f2, pixel_b;
    static void set_a_pixel(int x, int y);
  public:
    static void prepare();
    static void erase(int x, int y);
    static void trace_myship();
};

extern _radar radar;

#endif // XKOBO_H_RADAR
