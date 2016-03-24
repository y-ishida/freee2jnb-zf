ifeq ($(OS),Windows_NT)
	ext = .exe
else
	ext =
endif

all: freee2jnb-zf$(ext)

freee2jnb-zf$(ext): main.c
	gcc -o $@ $^



