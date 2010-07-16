
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
#include <unistd.h>
}
#include "manage.h"
#include "xkobo.h"
#include "key.h"

int _key::space;
int _key::left;
int _key::up;
int _key::down;
int _key::right;
int _key::dl;
int _key::dr;
int _key::ul;
int _key::ur;
int _key::shot;
int _key::direction;

void _key::init()
{
    direction = 1;
    _key::clear();
}

void _key::clear()
{
    left      = 0;
    right     = 0;
    up        = 0;
    down      = 0;
    shot      = 0;
}

void _key::press(KeySym sym)
{
    if      (sym == KEY_LEFT ) left  = 1;
    else if (sym == XK_Left  ) left  = 1;
    else if (sym == KEY_RIGHT) right = 1;
    else if (sym == XK_Right ) right = 1;
    else if (sym == KEY_UP   ) up    = 1;
    else if (sym == XK_Up    ) up    = 1;
    else if (sym == KEY_DOWN ) down  = 1;
    else if (sym == XK_Down  ) down  = 1;
    else if (sym == KEY_DL   ) dl    = 1;
    else if (sym == XK_End   ) dl    = 1;
    else if (sym == KEY_DR   ) dr    = 1;
    else if (sym == XK_Next  ) dr    = 1;
    else if (sym == KEY_UL   ) ul    = 1;
    else if (sym == XK_Home  ) ul    = 1;
    else if (sym == KEY_UR   ) ur    = 1;
    else if (sym == XK_Prior ) ur    = 1;
    else if (sym == KEY_SHOT ) shot  = 1;
    else if (sym == KEY_EXIT ) manage.exit_key();
    else if (sym == KEY_START) manage.start_key();
    else if (sym == KEY_PLUS ) manage.plus_key();
    else if (sym == KEY_MINUS) manage.minus_key();
    _key::change();
}

void _key::release(KeySym sym)
{
    if      (sym == KEY_LEFT ) left  = 0;
    else if (sym == XK_Left  ) left  = 0;
    else if (sym == KEY_RIGHT) right = 0;
    else if (sym == XK_Right ) right = 0;
    else if (sym == KEY_UP   ) up    = 0;
    else if (sym == XK_Up    ) up    = 0;
    else if (sym == KEY_DOWN ) down  = 0;
    else if (sym == XK_Down  ) down  = 0;
    else if (sym == KEY_DL   ) dl    = 0;
    else if (sym == XK_End   ) dl    = 0;
    else if (sym == KEY_DR   ) dr    = 0;
    else if (sym == XK_Next  ) dr    = 0;
    else if (sym == KEY_UL   ) ul    = 0;
    else if (sym == XK_Home  ) ul    = 0;
    else if (sym == KEY_UR   ) ur    = 0;
    else if (sym == XK_Prior ) ur    = 0;
    else if (sym == KEY_SHOT ) shot  = 0;
    _key::change();
}

void _key::change_up(unsigned long r)
{
    up = (r == 0) ? 1 : 0;
    _key::change();
}

void _key::change_down(unsigned long r)
{
    down = (r == 0) ? 1 : 0;
    _key::change();
}

void _key::change_left(unsigned long r)
{
    left = (r == 0) ? 1 : 0;
    _key::change();
}

void _key::change_right(unsigned long r)
{
    right = (r == 0) ? 1 : 0;
    _key::change();
}

void _key::change_shot(unsigned long r)
{
    shot = (r == 0) ? 1 : 0;
    _key::change();
}

void _key::mouse_press(int n)
{
    if      (n == 1) shot = 1;
    else if (n == 3) manage.start_key();
    _key::change();
}

void _key::mouse_release(int n)
{
    if      (n == 1) shot = 0;
    _key::change();
}

void _key::mouse_position(int h, int v)
{
    if (h > 0){
        if (v > 0){
            if (h > (v<<1)) direction = 3;
            else if (v > (h<<1)) direction = 5;
            else direction = 4;
        }
        else if (v < 0){
            if (h > ((-v)<<1)) direction = 3;
            else if ((-v) > (h<<1)) direction = 1;
            else direction = 2;
        }
        else direction = 3;
    }
    else if (h < 0){
        if (v > 0){
            if ((-h) > (v<<1)) direction = 7;
            else if (v > ((-h)<<1)) direction = 5;
            else direction = 6;
        }
        else if (v < 0){
            if ((-h) > ((-v)<<1)) direction = 7;
            else if ((-v) > ((-h)<<1)) direction = 1;
            else direction = 8;
        }
        else direction = 7;
    }
    else if (v > 0){
        direction = 5;
    }
    else {
        direction = 1;
    }
}

void _key::change()
{
    int lr = left - right + ul - ur + dl - dr;
    int ud = up   - down  + ul + ur - dl - dr;
    if (lr > 0){
        if (ud > 0) direction = 8;
        else if (ud < 0) direction = 6;
        else direction = 7;
    }
    else if (lr < 0){
        if (ud > 0) direction = 2;
        else if (ud < 0) direction = 4;
        else direction = 3;
    }
    else {
        if (ud > 0) direction = 1;
        else if (ud < 0) direction = 5;
    }
    mouse_x = 0;
    mouse_y = 0;
}
