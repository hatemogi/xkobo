
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

#include "screen.h"
#include "key.h"
#include "manage.h"
#include "enemies.h"
#include "myship.h"
#include "random.h"

/*
 * ===========================================================================
 *                                (template)
 * ===========================================================================
 */

inline void _enemy::move_enemy_template(int quick, int maxspeed)
{
    if (diffx > 0){
        if (h > -maxspeed) h -= quick;
    }
    else if (diffx < 0){
        if (h < maxspeed) h += quick;
    }
    if (diffy > 0){
        if (v > -maxspeed) v -= quick;
    }
    else if (diffy < 0){
        if (v < maxspeed) v += quick;
    }
    
    int m = MAX(MAX(h, -(h)), MAX(v, -(v)));
    if (m == h){
        if ((m>>1) < v)
          di = 4;
        else if ((m>>1) < -(v))
          di = 2;
        else di = 3;
    }
    else if (m == -(h)){
        if ((m>>1) < v)
          di = 6;
        else if ((m>>1) < -(v))
          di = 8;
        else di = 7;
    }
    else if (m == v){
        if ((m>>1) < h)
          di = 4;
        else if ((m>>1) < -(h))
          di = 6;
        else di = 5;
    }
    else {
        if ((m>>1) < h)
          di = 2;
        else if ((m>>1) < -(h))
          di = 8;
        else di = 1;
    }
}
inline void _enemy::move_enemy_template_2(int quick, int maxspeed)
{
    h = -(diffy << (SHIFT-quick));
    v =  (diffx << (SHIFT-quick));
    
    if (diffx > 0){
        if (h > -maxspeed) h -= quick;
    }
    else if (diffx < 0){
        if (h < maxspeed) h += quick;
    }
    if (diffy > 0){
        if (v > -maxspeed) v -= quick;
    }
    else if (diffy < 0){
        if (v < maxspeed) v += quick;
    }
    
    int m = MAX(MAX(h, -(h)), MAX(v, -(v)));
    if (m == h){
        if ((m>>1) < v)
          di = 4;
        else if ((m>>1) < -(v))
          di = 2;
        else di = 3;
    }
    else if (m == -(h)){
        if ((m>>1) < v)
          di = 6;
        else if ((m>>1) < -(v))
          di = 8;
        else di = 7;
    }
    else if (m == v){
        if ((m>>1) < h)
          di = 4;
        else if ((m>>1) < -(h))
          di = 6;
        else di = 5;
    }
    else {
        if ((m>>1) < h)
          di = 2;
        else if ((m>>1) < -(h))
          di = 8;
        else di = 1;
    }
}
inline void _enemy::move_enemy_template_3(int quick, int maxspeed)
{
    h =  (diffy << (SHIFT-quick));
    v = -(diffx << (SHIFT-quick));
    
    if (diffx > 0){
        if (h > -maxspeed) h -= quick;
    }
    else if (diffx < 0){
        if (h < maxspeed) h += quick;
    }
    if (diffy > 0){
        if (v > -maxspeed) v -= quick;
    }
    else if (diffy < 0){
        if (v < maxspeed) v += quick;
    }
    
    int m = MAX(MAX(h, -(h)), MAX(v, -(v)));
    if (m == h){
        if ((m>>1) < v)
          di = 4;
        else if ((m>>1) < -(v))
          di = 2;
        else di = 3;
    }
    else if (m == -(h)){
        if ((m>>1) < v)
          di = 6;
        else if ((m>>1) < -(v))
          di = 8;
        else di = 7;
    }
    else if (m == v){
        if ((m>>1) < h)
          di = 4;
        else if ((m>>1) < -(h))
          di = 6;
        else di = 5;
    }
    else {
        if ((m>>1) < h)
          di = 2;
        else if ((m>>1) < -(h))
          di = 8;
        else di = 1;
    }
}
inline void _enemy::shot_template(const enemy_kind *ekp,
                           int shift, int rnd, int maxspeed)
{
    int vx = -diffx;
    int vy = -diffy;
    if (rnd){
        vx += rand_num.get() & (rnd-1) - (rnd>>1);
        vy += rand_num.get() & (rnd-1) - (rnd>>1);
    }
    vx <<= (SHIFT-shift);
    vy <<= (SHIFT-shift);
    if (maxspeed > 0){
        if (vx > maxspeed) vx = maxspeed;
        else if (vx < -maxspeed) vx = -maxspeed;
        if (vy > maxspeed) vy = maxspeed;
        else if (vy < -maxspeed) vy = -maxspeed;
    }
    enemies.make(ekp, (x+vx)>>SHIFT, (y+vy)>>SHIFT, vx, vy);
}
void _enemy::shot_template_8_dir(const enemy_kind *ekp)
{
    static int vx[] = {    0,  200,  300,  200,    0, -200, -300, -200 };
    static int vy[] = { -300, -200,    0,  200,  300,  200,    0, -200 };
    int i;
    for (i=0; i<8; i++)
      enemies.make(ekp, (x>>SHIFT), (y>>SHIFT), vx[i], vy[i]);
}


/*
 * ===========================================================================
 *                                beam
 * ===========================================================================
 */
void _enemy::make_beam()
{
    di = 1;
    shield = -1;
}
void _enemy::move_beam()
{
    if (norm >= ((WSIZE>>1)+32)) state = notuse;
    if (++di > 8) di = 1;
}
const enemy_kind beam = {
    0,
    &_enemy::make_beam,
    &_enemy::move_beam,
    2,
    0, 0,
    6,
};

/*
 * ===========================================================================
 *                                rock
 * ===========================================================================
 */
void _enemy::make_rock()
{
    count = 500;
    shield = 255;
    di = (rand_num.get() % 3) + 1;
}
void _enemy::move_rock()
{
    ;
}
const enemy_kind rock = {
    10,
    &_enemy::make_rock,
    &_enemy::move_rock,
    4,
    11, 4,
    16, 
};

/*
 * ===========================================================================
 *                                ring
 * ===========================================================================
 */
void _enemy::make_ring()
{
    count = 500;
    shield = 1;
    di = 1;
}
void _enemy::move_ring()
{
    ;
}
const enemy_kind ring = {
    1,
    &_enemy::make_ring,
    &_enemy::move_ring,
    4,
    15, 4,
    16,
};

/*
 * ===========================================================================
 *                                bomb
 * ===========================================================================
 */
void _enemy::make_bomb()
{
    count = 500;
    shield = 1;
    di = 1;
}
void _enemy::move_bomb1()
{
    int h1 = ABS(diffx);
    int v1 = ABS(diffy);
    if (((h1 < 100) && (v1 < 30)) || (h1 < 30) && (v1 < 100)){
        int vx1 = (-diffx << (SHIFT-3)) / 3;
        int vy1 = (-diffy << (SHIFT-3)) / 3;
        int vx2 = vx1, vx3 = vx1;
        int vy2 = vy1, vy3 = vy1;
        int i;
        for (i=0; i<4; i++){
            int tmp = vx2;
            vx2 += (vy2 >> 4);
            vy2 -= (tmp >> 4);
            tmp = vx3;
            vx3 -= (vy3 >> 4);
            vy3 += (tmp >> 4);
        }
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx2, vy2);
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx3, vy3);
        state = notuse;
    }
}
const enemy_kind bomb1 = {
    5,
    &_enemy::make_bomb,
    &_enemy::move_bomb1,
    5,
    14, 4, 
    16,
};

/*
 * ===========================================================================
 *                                bomb2
 * ===========================================================================
 */
void _enemy::move_bomb2()
{
    int h1 = ABS(diffx);
    int v1 = ABS(diffy);
    if (((h1 < 100) && (v1 < 20)) || (h1 < 20) && (v1 < 100)){
        int vx1 = (-diffx << (SHIFT-3)) / 3;
        int vy1 = (-diffy << (SHIFT-3)) / 3;
        int vx2 = vx1, vx3 = vx1;
        int vy2 = vy1, vy3 = vy1;
        int i;
        for (i=0; i<6; i++){
            int tmp = vx2;
            vx2 += (vy2 >> 4);
            vy2 -= (tmp >> 4);
            tmp = vx3;
            vx3 -= (vy3 >> 4);
            vy3 += (tmp >> 4);
        }
        int vx4 = vx2, vx5 = vx3;
        int vy4 = vy2, vy5 = vy3;
        for (i=0; i<6; i++){
            int tmp = vx2;
            vx2 += (vy2 >> 4);
            vy2 -= (tmp >> 4);
            tmp = vx3;
            vx3 -= (vy3 >> 4);
            vy3 += (tmp >> 4);
        }
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx1, vy1);
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx2, vy2);
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx3, vy3);
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx4, vy4);
        enemies.make(&beam, (x>>SHIFT), (y>>SHIFT), vx5, vy5);
        state = notuse;
    }
}
const enemy_kind bomb2 = {
    20,
    &_enemy::make_bomb,
    &_enemy::move_bomb2,
    5,
    14, 4,
    16,
};

/*
 * ===========================================================================
 *                                explosion
 * ===========================================================================
 */
void _enemy::make_expl()
{
    di = 0;
    shield = -1;
}
void _enemy::move_expl()
{
    if (++di > 8) state = notuse;
}
const enemy_kind explosion = {
    0,
    &_enemy::make_expl,
    &_enemy::move_expl,
    -1,
    0, 1,
    16,
};

/*
 * ===========================================================================
 *                                cannon
 * ===========================================================================
 */
void _enemy::make_cannon()
{
    count = 0;
    shield = 1;
    b = enemies.eint1() - 1;
    a = rand_num.get() & b;
}
void _enemy::move_cannon()
{
    (count)++;
    (count) &= (b);
    if (count == a && norm < ((WSIZE>>1)+8)){
        int shift = (enemies.ek1() == &beam) ? 6 : 5;
        this->shot_template(enemies.ek1(), shift, 32, 0);
    }
}
const enemy_kind cannon = {
    10,
    &_enemy::make_cannon,
    &_enemy::move_cannon,
    4,
    0, 0,
    0,
};

/*
 * ===========================================================================
 *                                core
 * ===========================================================================
 */
void _enemy::make_core()
{
    count = 0;
    shield = 1;
    b = enemies.eint2() - 1;
    a = rand_num.get() & b;
}
void _enemy::move_core()
{
    (count)++;
    (count) &= (b);
    if (count == a && norm < ((WSIZE>>1)+8)){
        int shift = (enemies.ek2() == &beam) ? 6 : 5;
        this->shot_template(enemies.ek2(), shift, 0, 0);
    }
}
const enemy_kind core = {
    200,
    &_enemy::make_core,
    &_enemy::move_core,
    4,
    0, 0,
    0,
};

/*
 * ===========================================================================
 *                                pipe1
 * ===========================================================================
 */
void _enemy::make_pipe1()
{
    shield = -1;
    count = 4;
    a = 0;
}
void _enemy::move_pipe1()
{
    if ((norm < ((WSIZE>>1)+32)) && (count == 1))
        enemies.make(&explosion, (x>>SHIFT)+rand_num.get(4)-8,
                     (y>>SHIFT)+rand_num.get(4)-8); 
    if (++count < 4) return;
    count = 0;
    int x1 = ((x>>SHIFT) & (SCREEN_SIZEX-1))>>4;
    int y1 = ((y>>SHIFT) & (SCREEN_SIZEY-1))>>4;
    int a_next = 0;
    int x_next = 0;
    int y_next = 0;
    int p = screen.get_chip_number(x1, y1);
    if (p == 0){
        state = notuse;
        return;
    }
    if (norm < ((WSIZE>>1)+32))
        enemies.make(&explosion, (x>>SHIFT), (y>>SHIFT)); 
    if ((p^a) == U_MASK){
        a_next = D_MASK;
        y_next = -(16<<SHIFT);
    }
    if ((p^a) == R_MASK){
        a_next = L_MASK;
        x_next = (16<<SHIFT);
    }
    if ((p^a) == D_MASK){
        a_next = U_MASK;
        y_next = (16<<SHIFT);
    }
    if ((p^a) == L_MASK){
        a_next = R_MASK;
        x_next = -(16<<SHIFT);
    }
    if (a_next){
        screen.set_chip_number(x1, y1, 0);
        x += x_next;
        y += y_next;
        a  = a_next;
        return;
    }
    if (p != CORE){
        p ^= a;
        screen.set_chip_number(x1, y1, p);
    }
    state = notuse;
}
const enemy_kind pipe1 = {
    0,
    &_enemy::make_pipe1,
    &_enemy::move_pipe1,
    -1,
    0, 0,
    0,
};

/*
 * ===========================================================================
 *                                pipe2
 * ===========================================================================
 */
void _enemy::make_pipe2()
{
    int x1 = ((x>>SHIFT) & (SCREEN_SIZEX-1))>>4;
    int y1 = ((y>>SHIFT) & (SCREEN_SIZEY-1))>>4;
    screen.set_chip_number(x1, y1, 0);
    shield = -1;
    count = 4;
    switch(di){
      case 1:{
          a = D_MASK;
          y -= (16<<SHIFT);
          break;
      }
      case 3:{
          a = L_MASK;
          x += (16<<SHIFT);
          break;
      }
      case 5:{
          a = U_MASK;
          y += (16<<SHIFT);
          break;
      }
      case 7:{
          a = R_MASK;
          x -= (16<<SHIFT);
          break;
      }
    }
}
void _enemy::move_pipe2()
{
    if ((norm < ((WSIZE>>1)+32)) && (count == 1))
        enemies.make(&explosion, (x>>SHIFT)+rand_num.get(4)-8,
                     (y>>SHIFT)+rand_num.get(4)-8); 
    if (++count < 4) return;
    count = 0;
    
    int x1 = ((x>>SHIFT) & (SCREEN_SIZEX-1))>>4;
    int y1 = ((y>>SHIFT) & (SCREEN_SIZEY-1))>>4;
    int a_next = 0;
    int x_next = 0;
    int y_next = 0;
    int p = screen.get_chip_number(x1, y1);
    if (p == 0){
        state = notuse;
        return;
    }
    if (norm < ((WSIZE>>1)+32))
        enemies.make(&explosion, (x>>SHIFT), (y>>SHIFT)); 
    if ((p^a) == 0){
        manage.add_score(30);
        state = notuse;
        enemies.erase_cannon(x1, y1);
        screen.set_chip_number(x1, y1, 0);
        return;
    }
    if ((p^a) == HARD){
        state = notuse;
        screen.set_chip_number(x1, y1, 0);
        return;
    }
    if ((p^a) == U_MASK){
        a_next = D_MASK;
        y_next = -(16<<SHIFT);
    }
    if ((p^a) == R_MASK){
        a_next = L_MASK;
        x_next = (16<<SHIFT);
    }
    if ((p^a) == D_MASK){
        a_next = U_MASK;
        y_next = (16<<SHIFT);
    }
    if ((p^a) == L_MASK){
        a_next = R_MASK;
        x_next = -(16<<SHIFT);
    }
    screen.set_chip_number(x1, y1, 0);
    if (a_next){
        x += x_next;
        y += y_next;
        a  = a_next;
        return;
    }
    p ^= a;
    int a=0, b=0, c=0, d=0;
    if (p & U_MASK)
      a=enemies.make(&pipe2, (x>>SHIFT), (y>>SHIFT), 0, 0, 1);
    if (p & R_MASK)
      b=enemies.make(&pipe2, (x>>SHIFT), (y>>SHIFT), 0, 0, 3);
    if (p & D_MASK)
      c=enemies.make(&pipe2, (x>>SHIFT), (y>>SHIFT), 0, 0, 5);
    if (p & L_MASK)
      d=enemies.make(&pipe2, (x>>SHIFT), (y>>SHIFT), 0, 0, 7);
    manage.add_score(10);
    state = notuse;
}
const enemy_kind pipe2 = {
    0,
    &_enemy::make_pipe2,
    &_enemy::move_pipe2,
    -1,
    0, 0,
    0,
};

/*
 * ===========================================================================
 *                                enemy1
 * ===========================================================================
 */
void _enemy::make_enemy1()
{
    di = 1;
    shield = 1;
}
void _enemy::move_enemy1()
{
    this->move_enemy_template(2, 256);
}
const enemy_kind enemy1 = {
    2,
    &_enemy::make_enemy1,
    &_enemy::move_enemy1,
    6,
    8, 0,
    16,
};

/*
 * ===========================================================================
 *                                enemy2
 * ===========================================================================
 */
void _enemy::make_enemy2()
{
    di = 1;
    shield = 1;
    count = rand_num.get() & 63;
}
void _enemy::move_enemy2()
{
    this->move_enemy_template(4, 192);
    if (--(count) <= 0){ 
        if (norm < ((WSIZE>>1)+8)){
            this->shot_template(&beam, 5, 0, 0);
        }
        count = 32;
    }
}
const enemy_kind enemy2 = {
    10,
    &_enemy::make_enemy2,
    &_enemy::move_enemy2,
    6,
    8, 1,
    16,
};

/*
 * ===========================================================================
 *                                enemy3
 * ===========================================================================
 */
void _enemy::make_enemy3()
{
    di = 1;
    shield = 1;
}
void _enemy::move_enemy3()
{
    this->move_enemy_template(32, 96);
}
const enemy_kind enemy3 = {
    1,
    &_enemy::make_enemy3,
    &_enemy::move_enemy3,
    6,
    8, 2,
    16,
};

/*
 * ===========================================================================
 *                                enemy4
 * ===========================================================================
 */
void _enemy::make_enemy4()
{
    di = 1;
    shield = 1;
}
void _enemy::move_enemy4()
{
    this->move_enemy_template(4, 96);
}
const enemy_kind enemy4 = {
    1,
    &_enemy::make_enemy4,
    &_enemy::move_enemy4,
    6,
    8, 3,
    16,
};

/*
 * ===========================================================================
 *                                enemy5
 * ===========================================================================
 */
void _enemy::make_enemy5()
{
    count = rand_num.get() & 127;
    di = 1;
    shield = 1;
    a = 0;
}
void _enemy::move_enemy5()
{
    if (a == 0){
        if (norm > ((WSIZE>>1) - 32))
          this->move_enemy_template(6, 192);
        else
          a = 1;
    }
    else {
        if (norm < WSIZE)
          this->move_enemy_template_2(4, 192);
        else
          a = 0;
    }
    if ((--count) <= 0){
        count = 8;
        if (norm > ((WSIZE>>1) - 32))
          this->shot_template(&beam, 6, 0, 0);
    }
}
const enemy_kind enemy5 = {
    5,
    &_enemy::make_enemy5,
    &_enemy::move_enemy5,
    6,
    0, 4,
    16,
};

/*
 * ===========================================================================
 *                                enemy6
 * ===========================================================================
 */
void _enemy::move_enemy6()
{
    if (a == 0){
        if (norm > ((WSIZE>>1) - 0))
          this->move_enemy_template(6, 192);
        else
          a = 1;
    }
    else {
        if (norm < WSIZE)
          this->move_enemy_template_2(5, 192);
        else
          a = 0;
    }
    if ((--count) <= 0){
        count = 128;
        if (norm > ((WSIZE>>1) - 32))
          this->shot_template(&beam, 6, 0, 0);
    }
}
const enemy_kind enemy6 = {
    2,
    &_enemy::make_enemy5,
    &_enemy::move_enemy6,
    6,
    0, 5,
    16,
};

/*
 * ===========================================================================
 *                                enemy7
 * ===========================================================================
 */
void _enemy::move_enemy7()
{
    if (a == 0){
        if (norm > ((WSIZE>>1) - 32))
          this->move_enemy_template(6, 192);
        else
          a = 1;
    }
    else {
        if (norm < WSIZE)
          this->move_enemy_template_3(4, 192);
        else
          a = 0;
    }
    if ((--count) <= 0){
        count = 8;
        if (norm > ((WSIZE>>1) - 32))
          this->shot_template(&beam, 6, 0, 0);
    }
}
const enemy_kind enemy7 = {
    5,
    &_enemy::make_enemy5,
    &_enemy::move_enemy7,
    6,
    0, 6,
    16,
};
/*
 * ===========================================================================
 *                                enemy_m1
 * ===========================================================================
 */
void _enemy::make_enemy_m1()
{
    di = 1;
    shield = 26;
    count = rand_num.get() & 15;
}
void _enemy::move_enemy_m1()
{
    this->move_enemy_template(3, 128);
    di = 1;
    if ((count--) <= 0){
        count = 4;
        if (norm < ((WSIZE>>1)-16)){
            this->shot_template(&enemy1, 4, 0, 0);
        }
    }
    if (shield < 10){
        this->shot_template_8_dir(&enemy2);
        state = notuse;
    }
}
const enemy_kind enemy_m1 = {
    50,
    &_enemy::make_enemy_m1,
    &_enemy::move_enemy_m1,
    12,
    8, 4,
    32,
};

/*
 * ===========================================================================
 *                                enemy_m2
 * ===========================================================================
 */
void _enemy::make_enemy_m2()
{
    di = 1;
    shield = 26;
    count = rand_num.get() & 15;
}
void _enemy::move_enemy_m2()
{
    this->move_enemy_template(3, 128);
    di = 1;
    if ((count--) <= 0){
        count = 8;
        if (norm < ((WSIZE>>1)+8)){
            this->shot_template(&enemy2, 4, 128, 192);
        }
    }
    if (shield < 10){
        this->shot_template_8_dir(&bomb2);
        state = notuse;
    }
}
const enemy_kind enemy_m2 = {
    50,
    &_enemy::make_enemy_m2,
    &_enemy::move_enemy_m2,
    12,
    8, 4,
    32,
};

/*
 * ===========================================================================
 *                                enemy_m3
 * ===========================================================================
 */
void _enemy::make_enemy_m3()
{
    di = 1;
    shield = 26;
    count = rand_num.get() & 15;
}
void _enemy::move_enemy_m3()
{
    this->move_enemy_template(3, 128);
    di = 1;
    if ((count--) <= 0){
        count = 64;
        if (norm < ((WSIZE>>1)+8)){
            this->shot_template_8_dir(&bomb2);
        }
    }
    if (shield < 10){
        this->shot_template_8_dir(&rock);
        state = notuse;
    }
}
const enemy_kind enemy_m3 = {
    50,
    &_enemy::make_enemy_m3,
    &_enemy::move_enemy_m3,
    12,
    8, 4,
    32,
};

/*
 * ===========================================================================
 *                                enemy_m4
 * ===========================================================================
 */
void _enemy::make_enemy_m4()
{
    di = 1;
    shield = 26;
    count = rand_num.get() & 15;
}
void _enemy::move_enemy_m4()
{
    this->move_enemy_template(2, 96);
    di = 1;
    static const enemy_kind *shot[8] = {
        &enemy1, &enemy2, &bomb2, &ring, &enemy1, &enemy2, &ring, &enemy1};
    if ((count--) <= 0){
        count = 64;
        if (norm < ((WSIZE>>1)+8)){
            this->shot_template_8_dir(shot[rand_num.get()&7]);
        }
    }
    if (shield < 10){
        this->shot_template_8_dir(&rock);
        state = notuse;
    }
}
const enemy_kind enemy_m4 = {
    100,
    &_enemy::make_enemy_m4,
    &_enemy::move_enemy_m4,
    12,
    8, 4,
    32,
};


/*---------------------------------------------------------------------------*/
/*  void _enemy::make_xxxx(){}
 *  void _enemy::move_xxxx(){}
 *  enemy_kind xxxxx = { score, make_xxxx, move_xxxx, hitsize, cpx, cpy, size};
 */
