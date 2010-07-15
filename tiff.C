
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
#include "tiff.h"
#include "xkobo.h"
 
/********    tiny TIFF loader  ( 256 colors, not compressed )   ********/

static int pos_image   = 0;
static int pos_cmap    = 0;
static int sizex       = 0;
static int sizey       = 0;
static int endianness;

static unsigned int get_16bits(unsigned char *p)
{
    if (endianness == 0x49)
        return (p[0] + (p[1]<<8));
    return (p[1] + (p[0]<<8));
}

static unsigned int get_32bits(unsigned char *p)
{
    if (endianness == 0x49)
        return (p[0] + (p[1]<<8) + (p[2]<<16) + (p[3]<<24));
    return (p[3] + (p[2]<<8) + (p[1]<<16) + (p[0]<<24));
}

static int read_tag(unsigned char *data)
{
    unsigned char *p = data;
    int dir_p = 0;
    int tag_max;
    int tag_type;
    int i;
    
    endianness = p[0];
    if ((endianness != 0x49) && (endianness != 0x4d)) return 1;
    if (endianness != p[1]) return 2;
    if (get_16bits(p + 2) != 0x2a) return 3;
    p += 4;

    for (;;){
        dir_p = get_32bits(p);
        if (dir_p == 0) break;
        p = data + dir_p;
        
        tag_max = get_16bits(p);
        p += 2;
        for (i=0; i<tag_max; i++){
            tag_type = get_16bits(p);
            switch (tag_type){
              case 0x0100:
                {
                    /* image width */
                    sizex = get_16bits(p + 8);
                    break;
                };
              case 0x0101:
                {
                    /* image length */
                    sizey = get_16bits(p + 8);
                    break;
                };
              case 0x0102:
                {
                    /* bitss per sample */
                    if (get_16bits(p + 8) != 8)
                        return 4;
                    break;
                }
              case 0x0103:
                {
                    /* compression */
                    if (get_16bits(p + 8) != 1)
                        return 5;
                    break;
                }
              case 0x0111:
                {
                    /* strip offset */
                    pos_image = get_32bits(p + 8);
                    break;
                }
              case 0x0140:
                {
                    /* color map */
                    pos_cmap = get_32bits(p + 8);
                    break;
                }
            }
            p += 12;
        }
    }
    
    if ((!pos_image) || (!pos_cmap) || (!sizex) || (!sizey))
        return 6;
    return 0;
}

int link_tiff(unsigned char *data, win_cmap& wcm, win_chip& wch)
{
    unsigned long r[256],g[256],b[256];
    unsigned long pixel[256],mask[256];
    Display *d = win::disp;
    
    if (read_tag(data)){
         fprintf(stderr, "tiff.C : fatal error (Illegal format)\n");
         return -1;
    }
    int i, j=0;
    for (i=0; i<256; i++){
        r[j] = *(data + pos_cmap + j*2      ) * (65536/256);
        g[j] = *(data + pos_cmap + j*2 +512 ) * (65536/256);
        b[j] = *(data + pos_cmap + j*2 +1024) * (65536/256);
        if ((r[j]==0xff00)&&(g[j]==0)&&(b[j]==0)){
            /* through */
            mask[j] = 0xffffffff;
            pixel[j] = 0;
        }
        else {
            pixel[j] = wcm.alloc_color(r[j], g[j], b[j]);
            mask[j] = 0;
        }
        /* shuffle */
        j = (j + 169) & 255;
    }

    GC gc;
    gc = XCreateGC(d, wch.chip, 0, 0);
    
    unsigned char *ptr;
    int x, y;
    ptr = (unsigned char *)(data + pos_image);
    for (y=0; y<sizey; y++){
        for (x=0; x<sizex; x++){
            XSetForeground(d, gc, pixel[*ptr]);
            XFillRectangle(d, wch.chip, gc, x<<scale_log2, y<<scale_log2,
                           1<<scale_log2, 1<<scale_log2);
            ptr++;
        }
    }
    ptr = (unsigned char *)(data + pos_image);
    for (y=0; y<sizey; y++){
        for (x=0; x<sizex; x++){
            XSetForeground(d, gc, mask[*ptr]);
            XFillRectangle(d, wch.chip_mask, gc, x<<scale_log2, y<<scale_log2,
                           1<<scale_log2, 1<<scale_log2);
            ptr++;
        }
    }
    wcm.cmset();
    return 0;
}

