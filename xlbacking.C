
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

#include "xlbacking.h"

//--------------------------------------------------------------------------//
//                     class     win_backing                                //
//--------------------------------------------------------------------------//

win_backing::win_backing()
{
    mask |= ExposureMask;
}

win_backing::~win_backing()
{
    if (p) XFreePixmap(disp, p);
}

void win_backing::make(win *back,int wx,int wy,int sizex,int sizey)
{
    this->win::make(back,wx,wy,sizex,sizey);
    p = XCreatePixmap(disp,w,sx,sy,DefaultDepth(disp,scr));
    
    gc = XCreateGC(disp,w,0,0);
    XSetForeground(disp,gc,WhitePixel(disp,scr));
    XSetBackground(disp,gc,BlackPixel(disp,scr));
    XFillRectangle(disp,p,gc,0,0,sx,sy);
    XGCValues gcv;
    gcv.foreground = BlackPixel(disp,scr);
    gcv.background = WhitePixel(disp,scr);
    gcv.graphics_exposures = False;
    XChangeGC(disp,gc, GCForeground | GCBackground | GCGraphicsExposures,&gcv);
    fgcolor = BlackPixel(disp,scr);
    bgcolor = WhitePixel(disp,scr);
}

int win_backing::eventloop()
{
    if (this->win::eventloop() == 0) return 0;
    if (xevt.type == Expose){
        this->expose_backing();
    }
    return 1;
}

void win_backing::expose_backing()
{
    int x = xevt.xexpose.x;
    int y = xevt.xexpose.y;
    int a = xevt.xexpose.width;
    int b = xevt.xexpose.height;
    XCopyArea(disp,p,w,gc,x,y,a,b,x,y);
}

void win_backing::clear()
{
    XSetForeground(disp,gc,bgcolor);
    XFillRectangle(disp,p,gc,0,0,sx,sy);
    XFillRectangle(disp,w,gc,0,0,sx,sy);
    XSetForeground(disp,gc,fgcolor);
}

void win_backing::clear(int x,int y,int h,int v)
{
    XSetForeground(disp,gc,bgcolor);
    XFillRectangle(disp,p,gc,x,y,h,v);
    XFillRectangle(disp,w,gc,x,y,h,v);
    XSetForeground(disp,gc,fgcolor);
}

void win_backing::foreground(int i)
{
    if (i == fgcolor) return;
    fgcolor = i;
    XSetForeground(disp,gc,fgcolor);
}

void win_backing::background(int i)
{
    if (i == bgcolor) return;
    bgcolor = i;
    XSetBackground(disp,gc,bgcolor);
}

void win_backing::font(char *s)
{
    Font f1 = XLoadFont(disp,s);
    XSetFont(disp,gc,f1);
}

void win_backing::warp_pointer(int x, int y)
{
    XWarpPointer(disp, None, w, 0, 0, 0, 0, x, y);
}
