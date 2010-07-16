
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

#include "xlscroll.h"

//--------------------------------------------------------------------------//
//                     class     win_scroll                                 //
//--------------------------------------------------------------------------//

win_scroll::win_scroll()
{
    ;
}

win_scroll::~win_scroll()
{
    ;
}

void win_scroll::make(win *back,int wx,int wy, int sizex,int sizey,
                      int vsizex, int vsizey)
{
    vsx = vsizex;
    vsy = vsizey;
    vx  = 0;
    vy  = 0;
    this->win::make(back,wx,wy,sizex,sizey);
    p = XCreatePixmap(disp,w,vsx,vsy,DefaultDepth(disp,scr));
    
    gc = XCreateGC(disp,w,0,0);
    XSetForeground(disp,gc,WhitePixel(disp,scr));
    XFillRectangle(disp,p,gc,0,0,sx,sy);
    XGCValues gcv;
    gcv.foreground = BlackPixel(disp,scr);
    gcv.background = WhitePixel(disp,scr);
    gcv.graphics_exposures = False;
    XChangeGC(disp,gc, GCForeground | GCBackground | GCGraphicsExposures,&gcv);
    fgcolor = BlackPixel(disp,scr);
    bgcolor = WhitePixel(disp,scr);
}

void win_scroll::clear()
{
    XSetForeground(disp,gc,bgcolor);
    XFillRectangle(disp,p,gc,0,0,vsx,vsy);
    XFillRectangle(disp,w,gc,0,0,vsx,vsy);
    XSetForeground(disp,gc,fgcolor);
}

void win_scroll::expose_backing()
{
    int x = xevt.xexpose.x;
    int y = xevt.xexpose.y;
    int a = xevt.xexpose.width;
    int b = xevt.xexpose.height;
    XCopyArea(disp,p,w,gc,vx+x,vy+y,a,b,x,y);
}

void win_scroll::set_position(int vposx, int vposy)
{
    vx = vposx;
    vy = vposy;
    XCopyArea(disp,p,w,gc,vx,vy,sx,sy,0,0);
    XFlush(disp);
}

void win_scroll::torus()
{
    int x = vsx - sx;
    int y = vsy - sy;
    XCopyArea(disp,p,p,gc,0,0,sx,y,x,0);
    XCopyArea(disp,p,p,gc,0,0,vsx,sy,0,y);
}
