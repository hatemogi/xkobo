
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

#ifndef XKOBO_H_TIFF
#define XKOBO_H_TIFF

#include "file.h"
#include "xlchip.h"
#include "xlcmap.h"

#define CM_MIN 0
#define CM_MAX 255

int load_tiff(char *file_name, win_cmap& wcm, win_chip& wch);
int link_tiff(unsigned char *data, win_cmap& wcm, win_chip& wch);

#endif // XKOBO_H_TIFF
