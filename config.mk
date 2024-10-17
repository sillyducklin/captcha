# dwm version
VERSION = 0.1

# includes and libs
LIBS =  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# flags
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
LDFLAGS  = ${LIBS}

# compiler and linker
CC = cc
