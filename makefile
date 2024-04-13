
main: src/main.c
	gcc src/main.c -o bin/main -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2

main_warnings: src/main.c
	gcc src/main.c -o bin/main -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -Wall
