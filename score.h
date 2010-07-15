
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

#ifndef XKOBO_H_SCORE
#define XKOBO_H_SCORE

#include "config.h"

#define SCORE_SHOW_MAX      10
#define SCORE_SAVE_MAX      100

class _scorefile{
    static int highscore;
    static char save_path[1024];
    static char login_name[64];
  public:
    static void init(int *highscorep, int *stagep);
    static void record(int new_score, int stage);
    static void show_high_scores(int to_stdout);
};

extern _scorefile scorefile;

#endif // XKOBO_H_SCORE
