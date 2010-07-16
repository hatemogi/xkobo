
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

#include "config.h"
#include "map.h"
#include "random.h"

void _map::init()
{
    int i, j;
    for (i=0; i<sx; i++)
      for (j=0; j<sy; j++)
        pos(i, j) = SPACE;
}

void _map::make_maze(int x, int y, int difx, int dify)
{
    int i,j;
    int vx, vy;
    
    /* clear */
    for (i=x-difx; i<=x+difx; i++)
      for (j=y-dify; j<=y+dify; j++)
        pos(i,j) = SPACE;
    pos(x, y) = CORE;
    
    /* push initial sites */
    site_max = 0;
    
    if (rand_num.get(8) < 128){
        this->maze_push(x-1, y  );
        this->maze_push(x+1, y  );
    }
    else{
        this->maze_push(x  , y-1);
        this->maze_push(x  , y+1);
    }
    
    
    for (;;){
        /* get one */
        if (this->maze_pop()) break;
        vx = sitex[site_max];
        vy = sitey[site_max];
        
        int dirs[4];
        for (i=0; i<4; i++) dirs[i] = 0;
        int dirs_max = 0;
        if (this->maze_judge(x, y, difx, dify, vx+2, vy+0))
          dirs[dirs_max++] = 1;
        if (this->maze_judge(x, y, difx, dify, vx+0, vy+2))
          dirs[dirs_max++] = 2;
        if (this->maze_judge(x, y, difx, dify, vx-2, vy+0))
          dirs[dirs_max++] = 3;
        if (this->maze_judge(x, y, difx, dify, vx+0, vy-2))
          dirs[dirs_max++] = 4;
        if (dirs_max == 0) continue; /* there are no places to go */
        i = rand_num.get() % dirs_max;
        this->maze_move_and_push(vx, vy, dirs[i]);
        this->maze_push(vx, vy);
    }
}

int _map::maze_pop()
{
    if (site_max == 0) return 1;
    int i = rand_num.get() % site_max;
    site_max--;
    if (i != site_max){
        int tmpx = sitex[site_max];
        int tmpy = sitey[site_max];
        sitex[site_max] = sitex[i];
        sitey[site_max] = sitey[i];
        sitex[i] = tmpx;
        sitey[i] = tmpy;
    }
    return 0;
}

void _map::maze_push(int x, int y)
{
    sitex[site_max]   = x;
    sitey[site_max++] = y;
    pos(x, y) = WALL;
}

void _map::maze_move_and_push(int x, int y, int d)
{
    int x1 = x;
    int y1 = y;
    switch (d){
      case 1:{x1 += 2; break;}
      case 2:{y1 += 2; break;}
      case 3:{x1 -= 2; break;}
      case 4:{y1 -= 2; break;}
    }
    this->maze_push(x1, y1);
    pos((x+x1)/2, (y+y1)/2) = WALL;
}

int _map::maze_judge(int cx, int cy, int dx, int dy, int x, int y)
{
    if ((x < cx-dx) || (x > cx+dx) || (y < cy-dy) || (y > cy+dy)) return 0;
    if (pos(x, y) == WALL) return 0;
    return 1;
}

void _map::convert(int ratio)
{
    int i, j;
    int p = 0;
    for (i=0; i<sx; i++)
      for (j=0; j<sy; j++){
          p = 0;
          if (pos(i,j) != WALL) continue;
          if ((j > 0   ) && (pos(i  , j-1) != SPACE)) p |= U_MASK;
          if ((i < sx-1) && (pos(i+1, j  ) != SPACE)) p |= R_MASK;
          if ((j < sy-1) && (pos(i  , j+1) != SPACE)) p |= D_MASK;
          if ((i > 0   ) && (pos(i-1, j  ) != SPACE)) p |= L_MASK;
          if ((p==U_MASK) || (p==R_MASK) || (p==D_MASK) || (p==L_MASK)){
              if (rand_num.get(8) < ratio)
                p |= HARD;
          }
          pos(i,j) = p;
      }
}
