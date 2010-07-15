
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

#include "xkobo.h"
#include "screen.h"
#include "myship.h"
#include "enemies.h"
#include "key.h"
#include "manage.h"
#include "random.h"

_myship_state _myship::state;
int           _myship::di;
int           _myship::virtx;
int           _myship::virty;
int           _myship::x;
int           _myship::y;
int           _myship::lapx;
int           _myship::lapy;
int           _myship::beamx[BEAM_MAX];
int           _myship::beamy[BEAM_MAX];
int           _myship::beamdi[BEAM_MAX];
int           _myship::beamst[BEAM_MAX];

int _myship::init()
{
    x        = SCREEN_SIZEX >> 1;
    y        = (SCREEN_SIZEY >> 2) * 3;
    virtx    = x - (WSIZE >> 1);
    virty    = y - (WSIZE >> 1);
    lapx     = 0;
    lapy     = 0;
    di       = 1;
    state    = normal;

    int i;
    for (i=0; i<BEAM_MAX; i++){
        beamx [i] = 0;
        beamy [i] = 0;
        beamdi[i] = 0;
        beamst[i] = 0;
    }
    return 0;
}

#define BEAMV1           12
#define BEAMV2           (BEAMV1*2/3)

int _myship::move()
{
    int i;
    di = key.dir();
    
    virtx    = x - (WSIZE >> 1);
    virty    = y - (WSIZE >> 1);
    if (state == normal){
        switch (di){
          case 1:{virty -= 3; break;}
          case 2:{virty -= 2; virtx += 2; break;}
          case 3:{virtx += 3; break;}
          case 4:{virtx += 2; virty += 2; break;}
          case 5:{virty += 3; break;}
          case 6:{virty += 2; virtx -= 2; break;}
          case 7:{virtx -= 3; break;}
          case 8:{virtx -= 2; virty -= 2; break;}
        }
    }
    else if (state == dead){
        enemies.make(&explosion, x+rand_num.get(6)-32,
                     y+rand_num.get(6)-32);
    }
    
    lapx = 0;
    lapy = 0;
    if (virtx < 0            ){virtx += SCREEN_SIZEX; lapx =  SCREEN_SIZEX;}
    if (virtx >= SCREEN_SIZEX){virtx -= SCREEN_SIZEX; lapx = -SCREEN_SIZEX;}
    if (virty < 0            ){virty += SCREEN_SIZEY; lapy =  SCREEN_SIZEY;}
    if (virty >= SCREEN_SIZEY){virty -= SCREEN_SIZEY; lapy = -SCREEN_SIZEY;}
    x = virtx + (WSIZE >> 1);
    y = virty + (WSIZE >> 1);
    
    if ((state == normal) && key.get_shot()) _myship::shot();
    
    for (i=0; i<BEAM_MAX; i++){
        if (!beamst[i]) continue;
        beamx[i] += lapx;
        beamy[i] += lapy;
        switch (beamdi[i]){
          case 1:{beamy[i] -=BEAMV1; break;}
          case 2:{beamy[i] -=BEAMV2; beamx[i] +=BEAMV2; break;}
          case 3:{beamx[i] +=BEAMV1; break;}
          case 4:{beamx[i] +=BEAMV2; beamy[i] +=BEAMV2; break;}
          case 5:{beamy[i] +=BEAMV1; break;}
          case 6:{beamy[i] +=BEAMV2; beamx[i] -=BEAMV2; break;}
          case 7:{beamx[i] -=BEAMV1; break;}
          case 8:{beamx[i] -=BEAMV2; beamy[i] -=BEAMV2; break;}
        }
        if ((ABS(beamx[i] - x) >= (WSIZE>>1)+16) ||
            (ABS(beamy[i] - y) >= (WSIZE>>1)+16))
          beamst[i] = 0;
    }
    return 0;
}

void _myship::destroyed()
{
    if (state != normal) return;
    manage.lost_myship();
    state = dead;
}

int _myship::hit_structure()
{
    int x1, y1;
    int i;
    for (i=0; i<BEAM_MAX; i++){
        if (!beamst[i]) continue;
        x1 = (beamx[i] & (SCREEN_SIZEX-1))>>4;
        y1 = (beamy[i] & (SCREEN_SIZEY-1))>>4;
        if (screen.get_chip_number(x1, y1) & HIT_MASK)
          beamst[i] = 0;
    }
    x1 = (x & (SCREEN_SIZEX-1))>>4;
    y1 = (y & (SCREEN_SIZEY-1))>>4;
    if (screen.get_chip_number(x1, y1) & HIT_MASK){
        _myship::destroyed();
    }
    return 0;
}
        
int _myship::hit_beam(int ex, int ey, int hitsize)
{
    int i;
    for (i=0; i<BEAM_MAX; i++){
        if (beamst[i] == 0) continue;
        if (ABS(ex-beamx[i]) >= hitsize) continue;
        if (ABS(ey-beamy[i]) >= hitsize) continue;
        if (!cheat_mode) myship.beamst[i] = 0;
        return 1;
    }
    return 0;
}

int _myship::put()
{
    if (state != normal) return 0;
    wchip.copy_from_chip_sprite((di-1)<<(CHIP_SIZEX_LOG2+scale_log2),
                                3<<(CHIP_SIZEY_LOG2+scale_log2),
                                CHIP_SIZEX<<scale_log2,
                                CHIP_SIZEY<<scale_log2,
                                (x-CHIP_SIZEX/2)<<scale_log2,
                                (y-CHIP_SIZEY/2)<<scale_log2);
    int i;
    for (i=0; i<BEAM_MAX; i++){
        if (beamst[i])
            wchip.copy_from_chip_sprite((beamdi[i]-1)
                                        <<(CHIP_SIZEX_LOG2+scale_log2),
                                        2<<(CHIP_SIZEY_LOG2+scale_log2),
                                        CHIP_SIZEX<<scale_log2,
                                        CHIP_SIZEY<<scale_log2,
                                        (beamx[i]-CHIP_SIZEX/2)
                                        <<scale_log2,
                                        (beamy[i]-CHIP_SIZEY/2)
                                        <<scale_log2);
    }
    if ((mouse_x == 0) && (mouse_y == 0)) return 0;
    wchip.copy_from_chip_sprite(15<<(CHIP_SIZEX_LOG2+scale_log2),
                                5<<(CHIP_SIZEY_LOG2+scale_log2),
                                5<<scale_log2,
                                5<<scale_log2,
                                (x+mouse_x-2)<<scale_log2,
                                (y+mouse_y-2)<<scale_log2);
    return 0;
}

int _myship::shot()
{
    int i, j;
    for (i=0; i<BEAM_MAX && beamst[i]; i++);
    for (j=i+1; j<BEAM_MAX && beamst[j]; j++);
    if (j>=BEAM_MAX) return 1;
    beamdi[i] = di;
    beamx [i] = x;
    beamy [i] = y;
    beamst[i] = 1;
    beamdi[j] = (di > 4) ? (di-4) : (di+4);
    beamx [j] = x;
    beamy [j] = y;
    beamst[j] = 1;
    return 0;
}

void _myship::set_position(int px, int py)
{
    x = px;
    y = py;
    virtx = x - (WSIZE >> 1);
    virty = y - (WSIZE >> 1);
}
