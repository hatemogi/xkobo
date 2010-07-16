
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
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
}
#include "xkobo.h"
#include "config.h"
#include "tiff.h"
#include "screen.h"
#include "manage.h"
#include "score.h"
#include "key.h"
#include "random.h"
#include "version.h"

win_cmap        wbase;
win_chip        wchip;
win_backing     wradar;
win_backing     wscore;
int             scale_log2;
int             mouse_x = 0;
int             mouse_y = 0;
int             cheat_mode;

static int      signal_delivered = 1;

static void sig_handle(int)
{
    signal_delivered = 1;
}

void key_press_callback(win&)
{
    KeySym sym;
    sym = XKeycodeToKeysym(win::disp, win::xevt.xkey.keycode, 0);
    key.press(sym);
}

void key_release_callback(win&)
{
    KeySym sym;
    sym = XKeycodeToKeysym(win::disp, win::xevt.xkey.keycode, 0);
    key.release(sym);
}

void motion_callback(win&)
{
    mouse_x = win::xevt.xmotion.x;
    mouse_y = win::xevt.xmotion.y;
    mouse_x -= (WSIZE>>(1-scale_log2));
    mouse_y -= (WSIZE>>(1-scale_log2));
    if ((mouse_x == 0) && (mouse_y == 0)) return;
    key.mouse_position(mouse_x, mouse_y);
}

void leave_callback(win&)
{
    mouse_x = 0;
    mouse_y = 0;
}

void focus_in_callback(win&)
{
    key.clear();
}

void focus_out_callback(win&)
{
    key.clear();
}

void button_press_callback(win&)
{
    key.mouse_press(win::xevt.xbutton.button);
}

void button_release_callback(win&)
{
    key.mouse_release(win::xevt.xbutton.button);
}

void client_message_callback(win&)
{
    Atom wm_protocols, wm_delete_window;
    wm_protocols = XInternAtom(win::disp, "WM_PROTOCOLS", False);
    wm_delete_window = XInternAtom(win::disp, "WM_DELETE_WINDOW", False);
    if ((win::xevt.xclient.message_type == wm_protocols) &&
        (win::xevt.xclient.data.l[0] == wm_delete_window)){
        _manage::exit_key();
        _manage::exit_key();
    }
}

void put_usage()
{
    printf("\nXKOBO %s\n", XKOBO_VERSION);
    printf("Usage: xkobo -hiscores\n");
    printf("       xkobo [-doublesize]\n");
    printf("             [-fast]\n");
    printf("             [-fit]\n");
    printf("             [-cheat]\n");
    printf("             [-display]\n");
    printf("             [-wait XXX]\n");
    printf("\n");
    exit(1);
}

void put_score()
{
    scorefile.show_high_scores(1);
    exit(0);
}

/*           xkobo          */

int main(int argc, char *argv[])
{
    int sprite_policy = 1;
    int cmap_policy = 1;
    int wait_msec = WAIT_MSEC;
    struct itimerval value, ovalue;
    struct sigaction sig_act;
    int i;
    
    scale_log2 = 0;
    cheat_mode = 0;
    for (i=1; i<argc; i++){
        if (!strcmp("-speed", argv[i]))
            sprite_policy = 0;
        else if (!strcmp("-fast", argv[i]))
            sprite_policy = 0;
        else if (!strcmp("-doublesize", argv[i]))
            scale_log2 = 1;
        else if (!strcmp("-private", argv[i]))
            cmap_policy = 0;
        else if (!strcmp("-fit", argv[i]))
            cmap_policy = 0;
        else if (!strcmp("-cheat", argv[i]))
            cheat_mode = 1;
        else if (!strcmp("-display", argv[i]))
            win::set_disp_string(argv[++i]);
        else if (!strcmp("-wait", argv[i]))
            wait_msec = atoi(argv[++i]);
        else if (!strcmp("-hiscores", argv[i]))
            put_score();
        else put_usage();
    }

    wbase .event(ClientMessage, &client_message_callback);
    wscore.event(ClientMessage, &client_message_callback);
    wbase .event(FocusIn,       &focus_in_callback      );
    wbase .event(FocusOut,      &focus_out_callback     );
    wbase .event(KeyPress,      &key_press_callback     );
    wbase .event(KeyRelease,    &key_release_callback   );
    wchip .event(ButtonPress,   &button_press_callback  );
    wchip .event(ButtonRelease, &button_release_callback);
    wchip .event(MotionNotify,  &motion_callback        );
    wchip .event(LeaveNotify,   &leave_callback         );
    
    wbase.make(NULL, 0, 0,
               MARGIN * 3 + ((WSIZE + MAP_SIZEX) << scale_log2),
               MARGIN * 2 + (WSIZE << scale_log2), cmap_policy);
    wchip.make(&wbase, MARGIN, MARGIN,
               WSIZE << scale_log2, WSIZE << scale_log2,
               (SCREEN_SIZEX + WSIZE) << scale_log2,
               (SCREEN_SIZEY + WSIZE) << scale_log2,
               320 << scale_log2, 240 << scale_log2, sprite_policy);
    wradar.make(&wbase,
                MARGIN * 2 + (WSIZE << scale_log2), 
                MARGIN + ((WSIZE - MAP_SIZEY) << scale_log2),
                MAP_SIZEX << scale_log2, MAP_SIZEY << scale_log2);
    wscore.make(NULL, 0, 0, WSCORE_SIZEX, WSCORE_SIZEY);
    
    if (link_tiff(spdata, wbase, wchip)) return 1;
    
    wbase.title("xkobo");
    wbase.set_wm_close();
    wbase.hold_size();
    wbase.font(NORMAL_FONT);
    wbase.foreground(wbase.alloc_color(65535, 65535, 65535));
    wbase.background(wbase.alloc_color(0, 0, 0));
    wbase.map();
    wbase.clear();
    wchip.foreground(wbase.alloc_color(65535, 65535, 65535));
    wchip.background(wbase.alloc_color(0, 0, 0));
    wchip.setborder(wbase.alloc_color(0, 0, 0));
    wchip.erase_cursor();
    wchip.map();
    wradar.foreground(wbase.alloc_color(65535, 65535, 65535));
    wradar.background(wbase.alloc_color(0, 0, 0));
    wradar.setborder(wbase.alloc_color(0, 0, 0));
    wradar.map();
    wscore.title("xkobo-score");
    wscore.set_wm_close();
    wscore.hold_size();
    wscore.font(NORMAL_FONT);
    wscore.foreground(wbase.alloc_color(65535, 65535, 50000));
    wscore.background(wbase.alloc_color(0, 0, 0));
    wscore.map();
    wscore.clear();
    
    rand_num.init();
    manage.init();
    
    memset(&sig_act, 0, sizeof(sig_act));
    sig_act.sa_handler = sig_handle;
    sigaction(SIGALRM, &sig_act, 0);
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = wait_msec * 1000;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = wait_msec * 1000;
    setitimer(ITIMER_REAL, &value, &ovalue);
    
    for (;;){
        if (wait_msec && (signal_delivered == 0)) pause();
        signal_delivered = 0;
        if (manage.mainloop()) break;
    }
    
    return 0;
}
