include config.mk

SRC = main.c
OBJ = ${SRC:.c=.o}

.c.o:
	${CC} -c ${CFLAGS} $<

main: ${OBJ}
	${CC} -o ./target/$@ ${OBJ} ${LDFLAGS}

web:
	/lib/emscripten/emcc -o ./target/game.html main.c -Os -Wall ./lib/rl/lib/libraylib.a -I. -I./lib/rl/include/ -L. -L./lib/rl/lib/ --preload-file ./resources -s USE_GLFW=3 -s ASYNCIFY --shell-file ./lib/minshell.html -DPLATFORM_WEB

#emcc -o game.html game.c -Os -Wall ./path-to/libraylib.a -I. -Ipath-to-raylib-h -L. -Lpath-to-libraylib-a -s USE_GLFW=3 -s ASYNCIFY --shell-file path-to/shell.html -DPLATFORM_WEB

clean:
	rm *.o
