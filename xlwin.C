
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
#include <stdio.h>
#include <stdlib.h>
}
#include "xlwin.h"

char       win::disp_string[1024] = {0};
Display    *win::disp    = NULL;
int        win::scr;
XEvent     win::xevt;
const char icon_bitmap[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xf8, 0x01,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x98, 0x01, 0x00, 0x00, 0x77, 0x00,
    0x00, 0x00, 0x9c, 0x01, 0x00, 0x00, 0x63, 0x00, 0xe0, 0x00, 0x8e, 0x01,
    0x00, 0x00, 0x63, 0xc0, 0xf1, 0x01, 0xc7, 0x01, 0x00, 0x00, 0xc3, 0xe0,
    0xb3, 0x81, 0xc3, 0x00, 0x00, 0x00, 0xc7, 0x61, 0xb3, 0xc1, 0xe1, 0x00,
    0x00, 0x00, 0x86, 0x63, 0xb3, 0xf3, 0x70, 0x00, 0x00, 0x00, 0x0e, 0x6f,
    0x73, 0x3f, 0x3e, 0x00, 0x00, 0x00, 0x1c, 0x7e, 0x63, 0x8e, 0x1f, 0x00,
    0x00, 0x00, 0x38, 0x38, 0x63, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0xe7, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xc6, 0x78, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x1f, 0xc6, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0xc6, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0xf8, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x07,
    0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0xf8, 0x03, 0x00,
    0x00, 0x00, 0xf7, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xfb, 0x01,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0xcf, 0x03, 0x00, 0x00, 0x78, 0x00,
    0x00, 0x80, 0x03, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x80, 0x03, 0x06,
    0x00, 0x00, 0xc0, 0x01, 0x00, 0x80, 0x01, 0x06, 0x00, 0x00, 0x80, 0x03,
    0x80, 0xbf, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x07, 0xe0, 0xff, 0x0d, 0x06,
    0x00, 0x00, 0x00, 0x0e, 0x70, 0xe0, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x1c,
    0x38, 0x80, 0x01, 0x06, 0x00, 0x00, 0x00, 0x18, 0x18, 0x80, 0x03, 0x06,
    0x00, 0x00, 0x00, 0x38, 0x18, 0x00, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x30,
    0x1c, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0xf8, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xf0, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
    0x80, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x3f, 0x00,
    0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xf8, 0x00, 0x78, 0x00, 0xff, 0x01,
    0x00, 0x00, 0xe0, 0x01, 0xfc, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x80, 0x07,
    0xce, 0xc1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x86, 0xe1, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xbc, 0x87, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
    0x03, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x1e, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//--------------------------------------------------------------------------//
Bool event_predicate(Display *, XEvent *, char *)
{
    return True;
}


//--------------------------------------------------------------------------//
//                     class     win                                        //
//--------------------------------------------------------------------------//

win::win()
{
    mask = 0;
    ecount = 0;
}

win::~win()
{
    if (icon) XFreePixmap(disp, icon);
    if (w) XDestroyWindow(disp,w);
    if (null_cursor) XFreeCursor(disp, null_cursor);
    if (null_bm) XFreePixmap(disp, null_bm);
}   

void win::make(win *back,int wx,int wy,int sizex,int sizey)
{
    if (mask == -1) return;
    if (disp == NULL){
        disp = XOpenDisplay(disp_string);
        if (disp == NULL){
            fprintf(stderr, "xlwin: can't open display\n");
            exit(1);
        }
        scr = DefaultScreen(disp);
    }
    x = wx;
    y = wy;
    sx = sizex;
    sy = sizey;
    acm = XInternAtom(disp, "ACM",False);
    Window bk; 
    if (back == NULL) bk = RootWindow(disp,scr);
    else bk = back->w;
    w = XCreateSimpleWindow(disp,bk,x,y,sx,sy,
                            1,BlackPixel(disp,scr),WhitePixel(disp,scr));

    XSelectInput(disp,w,mask);
    mask = -1;

    XWMHints wmhint;
    icon = XCreateBitmapFromData(disp, w, icon_bitmap, 64, 64);
    wmhint.icon_pixmap = icon;
    wmhint.flags = IconPixmapHint;
    XSetWMHints(disp, w, &wmhint);
}

void win::map()
{
    XMapWindow(disp,w);
}

void win::unmap()
{
    XUnmapWindow(disp,w);
}

void win::title(char *title)
{
    XStoreName(disp,w,title);
    XSetIconName(disp,w,title);
}

void win::set_wm_close()
{
    Atom wm_delete_window;
    wm_delete_window = XInternAtom(disp, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(disp, w, &wm_delete_window, 1);
}

void win::hold_size()
{
    XSizeHints hint;
    hint.flags = (PMinSize | PMaxSize);
    hint.min_width  = sx;
    hint.max_width  = sx;
    hint.min_height = sy;
    hint.max_height = sy;
    XSetWMNormalHints(disp, w, &hint);
}

void win::setowner(void *owner)
{
    ownerobject = owner;
}

void *win::getowner()
{
    return ownerobject;
}

void win::setborder(unsigned long color)
{
    XSetWindowBorder(disp, w, color);
}

int win::event(int etyp,void (*c)(win& w))
{
    switch(etyp){
      case KeyPress:         {mask |= KeyPressMask; break;}
      case KeyRelease:       {mask |= KeyReleaseMask; break;}
      case ButtonPress:      {mask |= ButtonPressMask; break;}
      case ButtonRelease:    {mask |= ButtonReleaseMask; break;}
      case MotionNotify:     {mask |= PointerMotionMask; break;}
      case EnterNotify:      {mask |= EnterWindowMask; break;}
      case LeaveNotify:      {mask |= LeaveWindowMask; break;}
      case FocusIn:          {mask |= FocusChangeMask; break;}
      case FocusOut:         {mask |= FocusChangeMask; break;}
      case KeymapNotify:     {mask |= KeymapStateMask; break;}
      case Expose:           {mask |= ExposureMask; break;}
      case GraphicsExpose:   {mask |= 0; break;}
      case NoExpose:         {mask |= 0; break;}
      case VisibilityNotify: {mask |= VisibilityChangeMask; break;}
      case CreateNotify:     {mask |= SubstructureNotifyMask; break;}
      case DestroyNotify:    {mask |= StructureNotifyMask; break;}
      case UnmapNotify:      {mask |= StructureNotifyMask; break;}
      case MapNotify:        {mask |= StructureNotifyMask; break;}
      case MapRequest:       {mask |= SubstructureRedirectMask; break;}
      case ReparentNotify:   {mask |= StructureNotifyMask; break;}
      case ConfigureNotify:  {mask |= StructureNotifyMask; break;}
      case ConfigureRequest: {mask |= SubstructureRedirectMask; break;}
      case GravityNotify:    {mask |= StructureNotifyMask; break;}
      case ResizeRequest:    {mask |= ResizeRedirectMask; break;}
      case CirculateNotify:  {mask |= StructureNotifyMask; break;}
      case CirculateRequest: {mask |= SubstructureRedirectMask; break;}
      case PropertyNotify:   {mask |= PropertyChangeMask; break;}
      case SelectionClear:   {mask |= 0; break;}
      case SelectionRequest: {mask |= 0; break;}
      case SelectionNotify:  {mask |= 0; break;}
      case ColormapNotify:   {mask |= ColormapChangeMask; break;}
      case ClientMessage:    {mask |= 0; break;}
      case MappingNotify:    {mask |= 0; break;}
    }
    ec[ecount] = c;
    etype[ecount] = etyp;
    if (ecount < EVENTMAX-1) ecount++;
    return 0;
}

int win::event(int etyp,unsigned int emask,void (*c)(win& w))
{
    mask |= emask;
    ec[ecount] = c;
    etype[ecount] = etyp;
    if (ecount < EVENTMAX-1) ecount++;
    return 0;
}

int win::eventloop()
{
    if (xevt.xany.window != w) return 0;
    int i;
    for (i=0; i<ecount; i++){
        if (xevt.type == etype[i]){
            ec[i](*this);
        }
    }
    return 1;
}

void win::appeal(char *name)
{
    Atom a;

    a = XInternAtom(disp,name,False);
    XChangeProperty(disp,RootWindow(disp,scr),a,
                    XA_WINDOW,32,PropModeReplace,(unsigned char *)&w,1);
}

Window *win::getwindow(char *name)
{
    Atom a1,type;
    int result;
    int fmt;
    unsigned long nitems,left;
    Window *w2;

    a1 = XInternAtom(disp,name,False);
    if (a1 == 0) return NULL;
    result = XGetWindowProperty(disp,RootWindow(disp,scr),a1,0,4,False,
                                XA_WINDOW,&type,&fmt,&nitems,
                                &left,(unsigned char **)&w2);
    if (result != Success || type != XA_WINDOW) return 0;
    return w2;
}

void win::sendevent(Window *w1,Atom a1,int a,int b,int c,int d1,int e)
{
    XEvent cm;
    
    if (a1 == 0) a1 = acm;
    cm.xclient.type = ClientMessage;
    cm.xclient.display = disp;
    cm.xclient.window = *w1;
    cm.xclient.message_type = a1;
    cm.xclient.format = 32;
    cm.xclient.data.l[0] = a;
    cm.xclient.data.l[1] = b;
    cm.xclient.data.l[2] = c;
    cm.xclient.data.l[3] = d1;
    cm.xclient.data.l[4] = e;
    XSendEvent(disp,*w1,False,NoEventMask,&cm);
}

void win::erase_cursor()
{
    char bmdata[] = {0};
    XColor cc;
    
    null_bm = XCreateBitmapFromData(disp,w,bmdata,1,1);
    null_cursor = XCreatePixmapCursor(disp,null_bm,null_bm,&cc,&cc,0,0);
    XDefineCursor(disp,w,null_cursor);
}
