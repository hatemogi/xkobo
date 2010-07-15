
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

#include "xlcmap.h"
extern "C" {
#include "getvclass.h"
}

//--------------------------------------------------------------------------//
//                     class     win_cmap                                   //
//--------------------------------------------------------------------------//


win_cmap::win_cmap()
{
    mask |= ColormapChangeMask;
}

win_cmap::~win_cmap()
{
    if (cmap) XFreeColormap(disp, cmap);
}

void win_cmap::make(win *back,int wx,int wy,int sizex,int sizey,
                    int use_private)
{
    this->win_backing::make(back,wx,wy,sizex,sizey);
    
    use_private_cmap = use_private;
    switch (get_visual_class(disp, scr)){
      case PseudoColor:
        break;
      case GrayScale:
        break;
      case DirectColor:
        break;
      default:
        use_private_cmap = 0;
    }
    if (use_private_cmap){
        cmap = XCreateColormap(disp,RootWindow(disp,scr),
                           DefaultVisual(disp,scr),AllocNone);
    }
    else {
        cmap = DefaultColormap(disp, scr);
    }
}

void win_cmap::cmset()
{
    XSetWindowColormap(disp,w,cmap);
}

unsigned long win_cmap::alloc_color(long r, long g, long b)
{
    XColor col;
    col.red = r;
    col.green = g;
    col.blue = b;
    if (XAllocColor(disp, cmap, &col) == False)
        return (this->search_near(r, g, b));
    return (col.pixel);
}

unsigned long win_cmap::search_near(long r, long g, long b)
{
    const int num_cell_max = 256;
    static XColor col[num_cell_max];
    static unsigned long norm[num_cell_max];
    int num_cell = DisplayCells(disp, scr);
    int i;
    if (num_cell > num_cell_max) num_cell = num_cell_max;
    for (i=0; i<num_cell; i++)
        col[i].pixel = i;
    XQueryColors(disp, cmap, col, num_cell);
    r >>= 8;
    g >>= 8;
    b >>= 8;
    for (i=0; i<num_cell; i++){
        int x;
        x = (r - (col[i].red >> 8));
        norm[i] = x*x;
        x = (g - (col[i].green >> 8));
        norm[i] += x*x;
        x = (b - (col[i].blue >> 8));
        norm[i] += x*x;
    }
    int min = norm[0];
    int min_pixel = 0;
    for (i=0; i<num_cell; i++){
        if (norm[i] < min){
            min = norm[i];
            min_pixel = i;
        }
    }
    return min_pixel;
}
