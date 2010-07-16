
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

#ifndef XKOBO_H_XLBACKING
#define XKOBO_H_XLBACKING

#include "xlwin.h"

/*
 *
 *     win  ---- win_backing ----
 *                            \
 *                             \
 *
 */

class win_backing : public win{

  protected:
    GC gc;
    long fgcolor;
    long bgcolor;
    Pixmap p;
    
  public:
    win_backing();
    virtual ~win_backing();
    void make(win *back,int wx=0,int wy=0,int sizex=200,int sizey=200);
    int eventloop();
    void clear();
    void clear(int x,int y,int h,int v);
    virtual void expose_backing();                 /* from eventroop() */
    void foreground(int c);
    void background(int c);
    void font(char *s);
    void warp_pointer(int x, int y);
    inline void point_back(int x,int y){
        XDrawPoint(disp,p,gc,x,y);}
    inline void point(int x,int y){
        XDrawPoint(disp,p,gc,x,y);XDrawPoint(disp,w,gc,x,y);}
    inline void string_back(int x,int y,char *t){
        XDrawImageString(disp,p,gc,x,y,t,strlen(t));}
    inline void string(int x,int y,char *t){
        XDrawImageString(disp,p,gc,x,y,t,strlen(t));
        XDrawImageString(disp,w,gc,x,y,t,strlen(t));}
    inline void rectangle_back(int x,int y,int h,int v){
        XDrawRectangle(disp,p,gc,x,y,h,v);}
    inline void rectangle(int x,int y,int h,int v){
        XDrawRectangle(disp,p,gc,x,y,h,v);
        XDrawRectangle(disp,w,gc,x,y,h,v);}
    inline void fillrectangle_back(int x,int y,int h,int v){
        XFillRectangle(disp,p,gc,x,y,h,v);}
    inline void fillrectangle(int x,int y,int h,int v){
        XFillRectangle(disp,p,gc,x,y,h,v);
        XFillRectangle(disp,w,gc,x,y,h,v);}
    inline void back(){
        XCopyArea(disp,p,w,gc,0,0,sx,sy,0,0);}
    inline void back(int x,int y,int h,int v){
        XCopyArea(disp,p,w,gc,x,y,h,v,x,y);}
};

#endif // XKOBO_H_XLBACKING
