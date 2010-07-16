
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
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
}
#include "xkobo.h"
#include "score.h"
#include "file.h"

int  _scorefile::highscore       = 0;
char _scorefile::save_path[1024];
char _scorefile::login_name[64];

class uint_little_endian {
    unsigned char byte[4];
  public:
    uint_little_endian(unsigned int x = 0);
    operator unsigned int();
};

uint_little_endian::uint_little_endian(unsigned int x)
{
    byte[0] = x & 0xff;
    x >>= 8;
    byte[1] = x & 0xff;
    x >>= 8;
    byte[2] = x & 0xff;
    x >>= 8;
    byte[3] = x;
};

uint_little_endian::operator unsigned int()
{
    return byte[0] | (byte[1]<<8) | (byte[2]<<16) | (byte[3]<<24);
};

struct s_table{
    uint_little_endian score;
    uint_little_endian scene;
    char name[64];
};

int s_table_cmp(s_table *a, s_table *b)
{
    if (a->score < b->score) return 1;
    else if (a->score > b->score) return -1;
    return 0;
}

void _scorefile::init(int *highscorep, int *stagep)
{
    s_table tbl;
    tbl.score = 0;
    tbl.scene = 0;
    strcpy(tbl.name, "foo");
    int i;
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL){
        fprintf(stderr, "xkobo : getpwuid() failed\n");
        sprintf(login_name, "%u", getuid());
    }
    else {
        strcpy(login_name, pw->pw_name);
    }
    sprintf(save_path, "%s/%s/%u", getenv("HOME"), XKOBO_SCORE_DIR, getuid());
    if ((i=load_block(save_path, (char*)&tbl, sizeof(tbl))) < 0){
        fprintf(stderr, "xkobo : (warning) can't read the score-file from %s\n", save_path);
    }
    (*highscorep) = tbl.score;
    (*stagep) = tbl.scene;
    _scorefile::show_high_scores(0);
}

void _scorefile::record(int new_score, int stage)
{
    int write = 0;
    s_table tbl;
    tbl.score = 0;
    tbl.scene = 0;
    strcpy(tbl.name, "foo");
    (void)load_block(save_path, (char *)&tbl, sizeof(tbl));
    if (new_score > tbl.score){
        tbl.score = new_score;
        strcpy(tbl.name, login_name);
        write = 1;
    }
    if (stage > tbl.scene){
        tbl.scene = stage;
        strcpy(tbl.name, login_name);
        write = 1;
    }
    if (write){
        if (save_block(save_path, (char *)&tbl, sizeof(tbl)) < 0){
            fprintf(stderr, "xkobo : (warning) "
                            "can't update the score-file\n");
        }
    }
    _scorefile::show_high_scores(0);
}

void _scorefile::show_high_scores(int to_stdout)
{
    s_table tbl[SCORE_SAVE_MAX];
    char    path[1024];
    int i, j;
    for (i=0; i<SCORE_SAVE_MAX; i++){
        tbl[i].score = 0;
        tbl[i].scene = 0;
        strcpy(tbl[i].name, "foo");
    }

    struct dirent *direntp;
    DIR           *dp;
    struct stat st_buffer;
    sprintf(path, "%s/%s", getenv("HOME"), XKOBO_SCORE_DIR);
    dp = opendir(path);
    if (dp == NULL) return;
    i = 0;
    while ((direntp = readdir(dp)) != NULL){
        sprintf(path, "%s/%s/%s", getenv("HOME"), XKOBO_SCORE_DIR, direntp->d_name);
        if (stat(path, &st_buffer)) continue;
        if (!S_ISREG(st_buffer.st_mode)) continue;
        if (load_block(path, (char *)&tbl[i], sizeof(s_table)) < 0) continue;
        if ((++i) >= SCORE_SAVE_MAX) break;
    }
    closedir(dp);

    qsort(tbl, i, sizeof(s_table), 
          (int (*)(const void *, const void *))s_table_cmp);

    if (to_stdout){
        printf("Name                     Score    Stages\n");
        for (j=0; j<i; j++){
            printf("%-20s %9u %9u\n", tbl[j].name,
                   (unsigned int)tbl[j].score, (unsigned int)tbl[j].scene);
        }
    }
    else {
        wscore.clear();
        char buffer[256];
        if (i > SCORE_SHOW_MAX) i = SCORE_SHOW_MAX;
        wscore.string(10,  24, "name");
        wscore.string(180, 24, "high score");
        wscore.string(80, 24, "cleared stages");
        for (j=0; j<i; j++){
            wscore.string(10, 16*j + 48, tbl[j].name);
            sprintf(buffer, "%09u", (unsigned int)tbl[j].score);
            wscore.string(180, 16*j + 48, buffer);
            sprintf(buffer, "%09u", (unsigned int)tbl[j].scene);
            wscore.string(100, 16*j + 48, buffer);
        }
    }
}
