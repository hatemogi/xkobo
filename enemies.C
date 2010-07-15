
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
}
#include "enemies.h"
#include "radar.h"

_enemy            _enemies::enemy[ENEMY_MAX];
_enemy           *_enemies::enemy_max;
const enemy_kind *_enemies::ekind_to_generate_1;
const enemy_kind *_enemies::ekind_to_generate_2;
int               _enemies::e1_interval;
int               _enemies::e2_interval;

int _enemies::init()
{
    _enemy *enemyp;
    for (enemyp=enemy; enemyp<enemy+ENEMY_MAX; enemyp++)
        enemyp->init();
    enemy_max = enemy;
    ekind_to_generate_1 = NULL;
    ekind_to_generate_2 = NULL;
    e1_interval = 1;
    e2_interval = 1;
    return 0;
}

void _enemies::move()
{
    _enemy *enemyp;
    /* realize reserved enemies */
    for (enemyp=enemy; enemyp<enemy+ENEMY_MAX; enemyp++){
        if (enemyp->realize())
            enemy_max = enemyp;
    }
    for (enemyp=enemy; enemyp<=enemy_max; enemyp++)
        enemyp->move();
}

void _enemies::put()
{
    _enemy *enemyp;
    if (scale_log2 == 0){
        for (enemyp=enemy; enemyp<=enemy_max; enemyp++)
          enemyp->put();
    }
    else {
        for (enemyp=enemy; enemyp<=enemy_max; enemyp++)
          enemyp->put_zoom();
    }
}

int _enemies::make(const enemy_kind *ek, int x, int y, int h, int v, int di)
{
    _enemy *enemyp;
    for (enemyp=enemy; enemyp<enemy+ENEMY_MAX; enemyp++)
      if (!enemyp->make(ek, x, y, h, v, di)) return 0;
    return 1;
}

int _enemies::erase_cannon(int x, int y)
{
    int count = 0;
    _enemy *enemyp;
    for (enemyp=enemy; enemyp<enemy+ENEMY_MAX; enemyp++)
      count += enemyp->erase_cannon(x, y);
    if (count) radar.erase(x, y);
    return count;
}

int _enemies::exist_pipe()
{
    int count = 0;
    _enemy *enemyp;
    for (enemyp=enemy; enemyp<enemy+ENEMY_MAX; enemyp++)
      if (enemyp->is_pipe()) count++;
    return count;
}
            
void _enemies::set_ekind_to_generate(const enemy_kind *e1, int i1,
                                     const enemy_kind *e2, int i2)
{
    ekind_to_generate_1 = e1;
    ekind_to_generate_2 = e2;
    e1_interval = i1;
    e2_interval = i2;
}
