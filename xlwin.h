
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

#ifndef XKOBO_H_XLWIN
#define XKOBO_H_XLWIN
  
#define EVENTMAX 33

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <string.h>
}

Bool event_predicate(Display *, XEvent *, char *);

class win{

  protected:
    Window w;
    int x,y,sx,sy;
    long mask;
    int ecount;
    int etype[EVENTMAX];
    void (*ec[EVENTMAX])(win& w);
    Atom acm;
    void *ownerobject;
    Pixmap icon;
    static char disp_string[1024];
    Cursor null_cursor;
    Pixmap null_bm;
    
  public:
    static Display *disp;
    static int scr;
    static XEvent xevt;
    static inline void xnextevent(){
        XNextEvent(disp,&xevt);}
    static inline  int xpending(){
        return XPending(disp);}
    static inline void xsync(){
        XSync(disp,False);}
    static inline void xflush(){
        XFlush(disp);}
    static inline int xcheckevent(){
        return (XCheckIfEvent(disp, &xevt, &event_predicate, NULL) == True);}
    static inline void set_disp_string(char *t){
        strncpy(disp_string, t, 1024);}
    
    win();
    virtual ~win();
    int event(int etyp,void (*c)(win& w));
    int event(int etyp,unsigned int emask,void (*c)(win& w));
    void make(win *back=NULL,int wx=0,int wy=0,int sizex=200,int sizey=200);
    void map();
    void unmap();
    int eventloop();
    void title(char *title);
    void set_wm_close();
    void hold_size();
    void erase_cursor();
    void setowner(void *owner);
    void *getowner();
    void appeal(char *name);
    Window *getwindow(char *name);
    void setborder(unsigned long color);
    void sendevent(Window *w1,Atom a1,
                   int a=0,int b=0,int c=0,int d1=0,int e=0);
};

#endif // XKOBO_H_XLWIN
