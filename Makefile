all: freee2jnb-zf.exe

freee2jnb-zf.exe: main.c
	gcc -o freee2jnb-zf.exe $^

