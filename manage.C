
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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
}
#include "xkobo.h"
#include "screen.h"
#include "manage.h"
#include "scenes.h"
#include "score.h"
#include "enemies.h"
#include "myship.h"
#include "radar.h"
#include "key.h"

#define GIGA             1000000000
#define BONUS_FIRSTTIME  2000
#define BONUS_EVERY      3000

_manage_state   _manage::state;
int             _manage::next_state_out;
int             _manage::next_state_next;
int             _manage::highscore = 0;
int             _manage::last_scene = 0;
int             _manage::scene_num;
int             _manage::ships;
int             _manage::level;
int             _manage::count;
int             _manage::score;
int             _manage::score_changed;
int             _manage::bonus_next;
int             _manage::flush_score_count = 0;
int             _manage::flush_ships_count = 0;
int             _manage::delay_count;
int             _manage::rest_cores;
int             _manage::return_value = 0;

void _manage::game_start()
{
    ships = SHIPS;
    level = 0;
    score = 0;
    flush_score_count = 0;
    bonus_next = BONUS_FIRSTTIME;
    screen.init_scene(scene_num);
    _manage::init_resources_to_play();
}

void _manage::next_scene()
{
    scene_num++;
    if (scene_num >= GIGA - 1) scene_num = GIGA - 2;
    screen.init_scene(scene_num);
}

void _manage::retry()
{
    if (!cheat_mode) ships--;
    if (ships <= 0){
        if (last_scene < scene_num)
            last_scene = scene_num;
        if (!cheat_mode){
            if (highscore < score) highscore = score;
            scorefile.record(highscore, last_scene);
        }
        ships = 0;
        _manage::init_resources_title();
        return;
    }
}

void _manage::init_resources_title()
{
    state = title;
    screen.init_scene(-1);
    _manage::put_info();
}

void _manage::init_resources_to_play()
{
    state = playing;
    count = 0;
    delay_count = 0;
    flush_score_count = (flush_score_count) ? -1 : 0;
    flush_ships_count = 0;
    score_changed = 0;
    next_state_out = 0;
    next_state_next = 0;
    
    key.init();
    enemies.init();
    myship.init();
    rest_cores = screen.prepare();
    screen.generate_fixed_enemies();
    _manage::put_info();
    wchip.set_position(myship.get_virtx() << scale_log2,
                       myship.get_virty() << scale_log2);
}

void _manage::put_info()
{
    static char s[1024];
    int posx = MARGIN * 2 + (WSIZE << scale_log2) + 2;
    
    wbase.string(posx, 25, "SCORE");
    sprintf(s, "%09d", score);
    wbase.string(posx, 40, s);
    
    wbase.string(posx, 55, "STAGE");
    sprintf(s, "%09d", scene_num + 1);
    wbase.string(posx, 70, s);
    
    wbase.string(posx, 85, "SHIPS");
    if (!cheat_mode)
        sprintf(s, "%09d", ships);
    else
        sprintf(s, "999999999");
    wbase.string(posx, 100, s);
}

void _manage::put_score()
{
    static char s[1024];
    int posx = MARGIN * 2 + (WSIZE << scale_log2) + 2;
    
    sprintf(s, "%09d", score);
    wbase.string(posx, 40, s);
    score_changed = 0;
}

void _manage::flush_score()
{
    static char s[1024];
    int posx = MARGIN * 2 + (WSIZE << scale_log2) + 2;

    flush_score_count--;
    if (flush_score_count & 1) return;
    if (flush_score_count & 2){
        wbase.string(posx, 25, "            ");
        wbase.string(posx, 40, "                  ");
    }
    else {
        wbase.string(posx, 25, "SCORE");
        sprintf(s, "%09d", score);
        wbase.string(posx, 40, s);
    }
    if (flush_score_count == 0) flush_score_count = -1;
}

void _manage::flush_ships()
{
    static char s[1024];
    int posx = MARGIN * 2 + (WSIZE << scale_log2) + 2;

    flush_ships_count--;
    if (flush_ships_count & 1) return;
    if (flush_ships_count & 2){
        wbase.string(posx, 85, "          ");
        wbase.string(posx, 100, "                  ");
    }
    else {
        wbase.string(posx, 85, "SHIPS");
        sprintf(s, "%09d", ships);
        wbase.string(posx, 100, s);
    }
}

void _manage::eventloop()
{
    while (win::xcheckevent()){
        wbase.eventloop();
        wchip.eventloop();
        wradar.eventloop();
        wscore.eventloop();
    }
}

/****************************************************************************/
void _manage::init()
{
    scorefile.init(&highscore, &last_scene);
    count = 0;
    ships = 0;
    level = 0;
    scene_num = last_scene;
    flush_ships_count = 0;
    delay_count = 0;
    screen.init();
    _manage::init_resources_title();
}

int _manage::mainloop()
{
    _manage::eventloop();
    
    if (state != playing){
        return return_value;
    }
    
    count++;
    if (delay_count) delay_count--;
    
    if (delay_count == 1){
        if (enemies.exist_pipe()){
            delay_count = 2;
        }
        else {
            _manage::put_info();
            if (next_state_out){
                _manage::retry();
                if (ships <= 0){
                    return return_value;
                }
            }
            if (next_state_next){
                _manage::next_scene();
            }
            _manage::init_resources_to_play();
            return return_value;
        }
    }
    
    myship.move();
    enemies.move();
    myship.hit_structure();
    
    myship.put();
    enemies.put();
    if (score_changed) _manage::put_score();
    if (flush_score_count > 0) _manage::flush_score();
    if (flush_ships_count) _manage::flush_ships();
    radar.trace_myship();
    wchip.set_position(myship.get_virtx() << scale_log2,
                       myship.get_virty() << scale_log2);
    win::xflush();
    win::xsync();
    
    return return_value;
}

void _manage::lost_myship()
{
    if (delay_count == 0)
        delay_count = 20;
    next_state_out = 1;
}

void _manage::destroyed_a_core()
{
    rest_cores--;
    if (rest_cores == 0){
        next_state_next = 1;
        delay_count = 50;
    }
    screen.generate_fixed_enemies();
}

void _manage::start_key()
{
    switch (state){
      case title:{
          _manage::game_start();
          break;
      }
      case paused:{
          state = playing;
          break;
      }
      case playing:{
          state = paused;
          wchip.back();
          break;
      }
    }
}

void _manage::exit_key()
{
    if (state == title){
        return_value = 1;
    }
    ships = 0;
    _manage::retry();
    _manage::put_info();
}

void _manage::plus_key()
{
    if (state == title){
        if ((scene_num < last_scene) || cheat_mode)
            scene_num++;
        _manage::put_info();
    }
}

void _manage::minus_key()
{
    if (state == title){
        scene_num--;
        if (scene_num <= 0) scene_num = 0;
        _manage::put_info();
    }
}

void _manage::add_score(int sc)
{
    score += sc;
    if (score >= GIGA) score = GIGA - 1;
    else if (!cheat_mode){
        if (score >= bonus_next){
            bonus_next += BONUS_EVERY;
            ships++;
            flush_ships_count = 50;
        }
        if ((score >= highscore) && (flush_score_count == 0)){
            flush_score_count = 50;
        }
    }
    score_changed = 1;
}

void _manage::get_highscore(char *s)
{
    sprintf(s, "high score : %09d", highscore);
}    
