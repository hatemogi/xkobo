
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

#ifndef XKOBO_H_KEY
#define XKOBO_H_KEY

extern "C"{
#include <X11/Xlib.h>
#include <X11/keysym.h>
}
#include "config.h"

#define KEY_DOWN      XK_KP_2
#define KEY_LEFT      XK_KP_4
#define KEY_UP        XK_KP_8
#define KEY_RIGHT     XK_KP_6
#define KEY_DL        XK_KP_1
#define KEY_DR        XK_KP_3
#define KEY_UL        XK_KP_7
#define KEY_UR        XK_KP_9

#define KEY_START     XK_s
#define KEY_SHOT      XK_Shift_L
#define KEY_EXIT      XK_q
#define KEY_PLUS      XK_m
#define KEY_MINUS     XK_n

class _key{
    static int  space;
    static int  left, up, down, right, ul, ur, dl, dr;
    static int  shot;
    static int  direction;
    static void change();
  public:
    static void init();
    static void clear();
    static void press(KeySym sym);
    static void release(KeySym sym);
    static void change_up(unsigned long r);
    static void change_down(unsigned long r);
    static void change_left(unsigned long r);
    static void change_right(unsigned long r);
    static void change_shot(unsigned long r);
    static void mouse_press(int n);
    static void mouse_release(int n);
    static void mouse_position(int h, int v);
    static inline int dir(){ return direction; }
    static inline int get_shot(){ int ret = shot; return ret; }
};

extern _key key;

#endif // XKOBO_H_KEY
