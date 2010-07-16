
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

#ifndef XKOBO_H_CONFIG
#define XKOBO_H_CONFIG

#ifndef XKOBO_SCORE_DIR
#define XKOBO_SCORE_DIR  "./xkobo-scores"
#endif

#ifndef WAIT_MSEC
#define WAIT_MSEC        30
#endif

#ifndef SHIPS
#define SHIPS            5
#endif

#define BEAM_MAX         10
#define ENEMY_MAX        1024

#define WSIZE            226
#define MARGIN           12

#define HIT_MYSHIP       5
#define HIT_BEAM         5

#define WSCORE_SIZEX     250
#define WSCORE_SIZEY     240

#define NORMAL_FONT \
"-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

////////////////////////////////////////////////////////////////////////////

#define MAP_SIZEX_LOG2    6
#define MAP_SIZEY_LOG2    7
#define CHIP_SIZEX_LOG2   4
#define CHIP_SIZEY_LOG2   4
#define SCREEN_SIZEX_LOG2 (MAP_SIZEX_LOG2+CHIP_SIZEX_LOG2)
#define SCREEN_SIZEY_LOG2 (MAP_SIZEY_LOG2+CHIP_SIZEY_LOG2)
#define CHIP_SIZEX        (1<<CHIP_SIZEX_LOG2)
#define CHIP_SIZEY        (1<<CHIP_SIZEY_LOG2)
#define MAP_SIZEX         (1<<MAP_SIZEX_LOG2)
#define MAP_SIZEY         (1<<MAP_SIZEY_LOG2)
#define SCREEN_SIZEX      (1<<SCREEN_SIZEX_LOG2)
#define SCREEN_SIZEY      (1<<SCREEN_SIZEY_LOG2)

#endif // XKOBO_H_CONFIG
