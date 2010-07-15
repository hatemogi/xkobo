
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

#ifndef XKOBO_H_RANDOM
#define XKOBO_H_RANDOM

extern "C"{
#include <time.h>
}

class _rand_num{
    static unsigned long seed;
  public:
    static inline void init();
    static inline unsigned long get();
    static inline unsigned long get(unsigned int bit);
};

inline void _rand_num::init()
{
    seed = (unsigned long)time((time_t *)0);
}

inline unsigned long _rand_num::get()
{
    seed *= 1566083941UL;
    seed++;
    seed &= 0xffffffffUL;
    return seed;
}

inline unsigned long _rand_num::get(unsigned int bit)
{
    seed *= 1566083941UL;
    seed++;
    seed &= 0xffffffffUL;
    return (seed >> (32 - bit));
}

extern _rand_num rand_num;

#endif // XKOBO_H_RANDOM
