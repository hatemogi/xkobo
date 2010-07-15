
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

#ifndef XKOBO_H_XLSCROLL
#define XKOBO_H_XLSCROLL

#include "xlbacking.h"

/*
 *
 *     win_backing  ---- win_scroll ----
 *                   \
 *                    \
 *                   
 */
 
class win_scroll : public win_backing{

  protected:
    int vsx,vsy; /* size of virtual screen */
    int vx,vy;   /* current position */
    
  public:
    win_scroll();
    ~win_scroll();
    void make(win *back, int wx, int wy, int sizex, int sizey,
              int vsizex, int vsizey);
    void clear();
    virtual void expose_backing();                 /* from eventroop() */
    void set_position(int vposx, int vposy);
    void torus();
    inline void back(){
        XCopyArea(disp,p,w,gc,vx,vy,vx+sx,vy+sy,0,0);}
    inline void back(int x,int y,int h,int v){
        XCopyArea(disp,p,w,gc,vx+x,vy+y,h,v,x,y);}
};

#endif // XKOBO_H_XLSCROLL
