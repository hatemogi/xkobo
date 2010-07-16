#  SPEED OF THE GAME  
WAIT_MSEC   = 30

#  NUMBER OF SHIPS  
SHIPS       = 5

#  RECORDS  
HSCORE_DIR  = Library/Preferences/com.hatemogi.xkobo/scores

.SUFFIXES: .C $(SUFFIXES)

SRCS1= xkobo.C tiff.C file.C xlwin.C xlbacking.C xlcmap.C \
       xlscroll.C xlchip.C enemy.C enemies.C \
       myship.C key.C map.C radar.C screen.C manage.C \
       scenes.C score.C random.C

SRCS2= getvclass.c xkp256.c

SRCS = $(SRCS1) $(SRCS2)

DEFINES = -DWAIT_MSEC=$(WAIT_MSEC) -DSHIPS=$(SHIPS) \
          -DXKOBO_SCORE_DIR=\"$(HSCORE_DIR)\"

CFLAGS=$(DEFINES)
CCOPTIONS = #-Wall -g -pg

LOCAL_LIBRARIES = -L/usr/X11/lib -lX11

OBJS = $(SRCS1:.C=.o) $(SRCS2:.c=.o)

xkobo: $(OBJS) $(DEPLIBS)
	$(CXX) -o $@ $(OBJS) $(LDOPTIONS) $(LOCAL_LIBRARIES) $(LDLIBS) $(EXTRA_LOAD_FLAGS)

.C.o:
	$(CXX) $(CFLAGS) -c $< -o $@


xkp256.c: xkp256.tif
	$(RM) xkp256.c
	echo '/*'                                            > xkp256.c
	echo ' * XKOBO, a video-oriented game'              >> xkp256.c
	echo ' * Copyright (C) 1995,1996  Akira Higuchi'    >> xkp256.c
	echo ' *     a-higuti@math.hokudai.ac.jp'           >> xkp256.c
	echo ' *'                                           >> xkp256.c
	echo ' */'                                          >> xkp256.c
	echo 'unsigned char spdata[] = {'                   >> xkp256.c
	hexdump -v -e '"\t" 8/1 "0x0%02x," "\n"' xkp256.tif >> xkp256.c
	echo '};'                                           >> xkp256.c

install:
	if [ -d $(HSCORE_DIR) ]; then chmod 777 $(HSCORE_DIR); \
	else mkdirhier $(HSCORE_DIR); chmod 777 $(HSCORE_DIR); fi


clean:
	$(RM) xkobo *.o 
