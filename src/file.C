 
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
}

int load_block(char *file_name, void *t, int len) 
{
    int i, fd, ret = 0;
    char *p = (char *)t;
    fd = open(file_name, O_RDONLY, 0);
    if (fd == -1) return -1;
    do {
        i = read(fd, p, len);
        if ((i < 0) && (errno == EINTR)) continue;
        if (i <= 0){
            ret = -2;
            break;
        }
        p += i;
        len -= i;
    } while (len > 0);
    for (;;){
        i = close(fd);
        if (i == -1){
            if (errno == EINTR) continue;
            else break;
        }
    }
    return ret;
}

int save_block(char *file_name, void *t, int len) 
{
    int i, fd, ret = 0;
    char *p = (char *)t;
    fd = open(file_name, O_CREAT|O_WRONLY, 0644);
    if (fd == -1) return -1;
    do {
        i = write(fd, p, len);
        if ((i < 0) && (errno == EINTR)) continue;
        if (i <= 0){
            ret = -2;
            break;
        }
        p += i;
        len -= i;
    } while (len > 0);
    for (;;){
        i = close(fd);
        if (i == -1){
            if (errno == EINTR) continue;
            else break;
        }
    }
    return ret;
}
