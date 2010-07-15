
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

#ifndef XKOBO_H_ENEMIES
#define XKOBO_H_ENEMIES

extern "C"{
#include <stdio.h>
}
#include "xkobo.h"
#include "config.h"
#include "myship.h"
#include "manage.h"

#define SHIFT    6                               /* fixed pointer */
#define ABS(x)   (((x)>=0) ? (x) : (-(x)))
#define MAX(x,y) (((x)>(y)) ? (x) : (y))

class _enemy;
class  _enemies;
//---------------------------------------------------------------------------//
struct enemy_kind{
    int score;
    void (_enemy::*make)();
    void (_enemy::*move)();
    int hitsize;
    int chipposx, chipposy;
    int chipsize;
};


extern const enemy_kind beam;
extern const enemy_kind explosion;
extern const enemy_kind enemy1;
extern const enemy_kind enemy2;
extern const enemy_kind enemy3;
extern const enemy_kind enemy4;
extern const enemy_kind enemy5;
extern const enemy_kind enemy6;
extern const enemy_kind enemy7;
extern const enemy_kind bomb1;
extern const enemy_kind bomb2;
extern const enemy_kind cannon;
extern const enemy_kind pipe1;
extern const enemy_kind core;
extern const enemy_kind pipe2;
extern const enemy_kind rock;
extern const enemy_kind ring;
extern const enemy_kind enemy_m1;
extern const enemy_kind enemy_m2;
extern const enemy_kind enemy_m3;
extern const enemy_kind enemy_m4;


//---------------------------------------------------------------------------//
class _enemy{
    void hit_by_beam();
    const enemy_kind *ek;
    enum { notuse, reserved, moving } state;
    int x,y;
    int h,v;
    int di;
    int a,b;
    int count;
    int shield;
    int diffx, diffy;
    int norm;
    int hitsize;
    int chipposx, chipposy;
    int chipsize;
    void move_enemy_template(int quick, int maxspeed);
    void move_enemy_template_2(int quick, int maxspeed);
    void move_enemy_template_3(int quick, int maxspeed);
    void shot_template(const enemy_kind *ekp,
                       int shift, int rand_num, int maxspeed);
    void shot_template_8_dir(const enemy_kind *ekp);
  public:
    inline void init();
    inline void move();
    inline void put();
    inline void put_zoom();
    inline int make(const enemy_kind *k,
                    int px, int py, int h1, int v1, int dir=0);
    inline int realize();
    inline int is_pipe();
    inline int erase_cannon(int px, int py);
    
    void make_beam();
    void move_beam();
    void make_rock();
    void move_rock();
    void make_ring();
    void move_ring();
    void make_bomb();
    void move_bomb1();
    void move_bomb2();
    void make_expl();
    void move_expl();
    void make_cannon();
    void move_cannon();
    void make_core();
    void move_core();
    void make_pipe1();
    void move_pipe1();
    void make_pipe2();
    void move_pipe2();
    void make_enemy1();
    void move_enemy1();
    void make_enemy2();
    void move_enemy2();
    void make_enemy3();
    void move_enemy3();
    void make_enemy4();
    void move_enemy4();
    void make_enemy5();
    void move_enemy5();
    void move_enemy6();
    void move_enemy7();
    void make_enemy_m1();
    void move_enemy_m1();
    void make_enemy_m2();
    void move_enemy_m2();
    void make_enemy_m3();
    void move_enemy_m3();
    void make_enemy_m4();
    void move_enemy_m4();
};

//---------------------------------------------------------------------------//
class _enemies{
    static _enemy enemy[ENEMY_MAX];
    static _enemy *enemy_max;
    static const enemy_kind *ekind_to_generate_1;
    static const enemy_kind *ekind_to_generate_2;
    static int e1_interval;
    static int e2_interval;
  public:
    static int init();
    static void move();
    static void put();
    static int make(const enemy_kind *ek,
                    int x, int y, int h=0, int v=0, int di=0);
    static int erase_cannon(int x, int y);
    static int exist_pipe();
    static void set_ekind_to_generate(const enemy_kind *ek1, int i1,
                                      const enemy_kind *ek2, int i2);
    static inline const enemy_kind *ek1(){ return ekind_to_generate_1; }
    static inline const enemy_kind *ek2(){ return ekind_to_generate_2; }
    static inline int eint1(){ return e1_interval; }
    static inline int eint2(){ return e2_interval; }
};

extern _enemies   enemies;


inline void _enemy::init()
{
    state = notuse;
}

inline int _enemy::make(const enemy_kind *k, int px, int py,
                        int h1, int v1, int dir)
{
    if (state != notuse) return -1;
    state     = reserved;
    ek        = k;
    x         = px << SHIFT;
    y         = py << SHIFT;
    di        = dir;
    h         = h1;
    v         = v1;
    a         = 0;
    b         = 0;
    count     = 0;
    shield    = 1;
    hitsize   = ek->hitsize;
    chipposx  = ek->chipposx;
    chipposy  = ek->chipposy;
    chipsize = ek->chipsize;
    (this->*(ek->make))();
    return 0;
}

inline void _enemy::hit_by_beam()
{
    if ((--shield) > 0) return;
    manage.add_score(ek->score);
    if (ek == &cannon){
        enemies.make(&pipe1, x>>SHIFT, y>>SHIFT);
        state = notuse;
    }
    else if (ek == &core){
        enemies.make(&pipe2, x>>SHIFT, y>>SHIFT, 0, 0, 3);
        enemies.make(&pipe2, x>>SHIFT, y>>SHIFT, 0, 0, 7);
        enemies.make(&pipe2, x>>SHIFT, y>>SHIFT, 0, 0, 1);
        enemies.make(&pipe2, x>>SHIFT, y>>SHIFT, 0, 0, 5);
        enemies.make(&explosion, x>>SHIFT, y>>SHIFT);
        state = notuse;
        manage.destroyed_a_core();
    }
    else {
        enemies.make(&explosion, x>>SHIFT, y>>SHIFT);
        state = notuse;
    }
}

inline int _enemy::erase_cannon(int px, int py)
{
    if ((state != notuse) && (ek == &cannon) &&
        (((x>>SHIFT) & (SCREEN_SIZEX-1))>>4 == px) &&
        (((y>>SHIFT) & (SCREEN_SIZEY-1))>>4 == py)){
        state = notuse;
        return 1;
    }
    return 0;
}

inline void _enemy::move()
{
    if (state != moving) return;
    x += h;
    y += v;
    diffx = (x>>SHIFT) - myship.get_x();
    diffy = (y>>SHIFT) - myship.get_y();
    
    if (diffx >   (SCREEN_SIZEX>>1)){
        diffx -= SCREEN_SIZEX;
        x -= (SCREEN_SIZEX<<SHIFT);
    }
    if (diffx < - (SCREEN_SIZEX>>1)){
        diffx += SCREEN_SIZEX;
        x += (SCREEN_SIZEX<<SHIFT);
    }
    if (diffy >   (SCREEN_SIZEY>>1)){
        diffy -= SCREEN_SIZEY;
        y -= (SCREEN_SIZEY<<SHIFT);
    }
    if (diffy < - (SCREEN_SIZEY>>1)){
        diffy += SCREEN_SIZEY;
        y += (SCREEN_SIZEY<<SHIFT);
    }
    
    norm = MAX(ABS(diffx), ABS(diffy));
    (this->*(ek->move))();
    if ((hitsize >= 0) && (norm < (hitsize + HIT_MYSHIP))){
        myship.destroyed();
    }
    if ((shield < 0) || (norm >= ((WSIZE>>1)+8)))
      return;
    if (myship.hit_beam((x>>SHIFT), (y>>SHIFT), hitsize + HIT_BEAM))
      this->hit_by_beam();
}

inline void _enemy::put()
{
    if ((state == moving) && (chipsize) && (norm < ((WSIZE>>1)+8))){
        wchip.copy_from_chip_sprite((chipposx + di - 1)<<CHIP_SIZEX_LOG2,
                                    chipposy<<CHIP_SIZEY_LOG2,
                                    chipsize, chipsize,
                                    (x>>SHIFT)-(chipsize>>1),
                                    (y>>SHIFT)-(chipsize>>1));
    }
}

inline void _enemy::put_zoom()
{
    if ((state == moving) && (chipsize) && (norm < ((WSIZE>>1)+8))){
        wchip.copy_from_chip_sprite((chipposx + di - 1)
                                    <<(CHIP_SIZEX_LOG2 + scale_log2),
                                    chipposy << (CHIP_SIZEY_LOG2 + scale_log2),
                                    chipsize << scale_log2,
                                    chipsize << scale_log2,
                                    ((x>>SHIFT)-(chipsize>>1))
                                    <<scale_log2,
                                    ((y>>SHIFT)-(chipsize>>1))
                                    <<scale_log2);
    }
}

inline int _enemy::realize()
{
    if (state == reserved) state = moving;
    return (state == moving);
}

inline int _enemy::is_pipe()
{
    return ((state != notuse) && ((ek == &pipe1) || (ek == &pipe2)));
}


#endif // XKOBO_H_ENEMIES
