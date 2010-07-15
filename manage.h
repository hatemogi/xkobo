
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

#ifndef XKOBO_H_MANAGE
#define XKOBO_H_MANAGE

enum _manage_state {title, playing, paused};
class _manage {
    static _manage_state state;
    static int next_state_out;
    static int next_state_next;
    static int highscore;
    static int last_scene;
    static int scene_num;
    static int ships;
    static int level;
    static int count;
    static int score;
    static int score_changed;
    static int bonus_next;
    static int flush_score_count;
    static int flush_ships_count;
    static int delay_count;
    static int rest_cores;
    static int return_value;
    static void game_start();
    static void next_scene();
    static void retry();
    static void init_resources_to_play();
    static void init_resources_title();
    static void put_info();
    static void put_score();
    static void flush_ships();
    static void flush_score();
    static void eventloop();
  public:
    static void init();
    static int  mainloop();
    static void lost_myship();
    static void destroyed_a_core();
    static void start_key();
    static void exit_key();
    static void plus_key();
    static void minus_key();
    static void add_score(int sc);
    static void get_highscore(char *s);
    static inline int state_playing(){return (state == playing);}
};

extern _manage manage;

#endif // XKOBO_H_MANAGE
