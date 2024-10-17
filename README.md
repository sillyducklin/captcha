this is a bad attempt at a captcha.
its 99% a joke.

compiling on windows was pretty annoying tbh and it barely works
in the compile.bat there is a path you need to set to gcc
then it uses the headers/libraries from the build directory. (if everything works)

if somehow you can compile it (or if the included .exe works)
you need to run the exe from the parent directory of the resources folder in order to properly load textures.

also if you want to wait less on the loading bars you can just hold spacebar down.

if none of that works i also compiled the program to wasm. so you can just cd into the target folder
and host the files in order to properly load the js (or just `python -m http.server 8080`) and then go to localhost:8080/game.html
and it should just work from there. gl 

(written using raylib my code is really bad and just scraped together)
