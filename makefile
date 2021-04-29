CC := gcc
CFLAGS := -O3 -Wall -Wextra -Wshadow

.PHONY: all
all: runSUS.o src.a runSUS.exe tags

runSUS.o:
	$(CC) $(CFLAGS) -c runSUS.c

src.a: creditos.o game.o menu.o placar.o sair.o utils.o
	ar cr src.a *.o

runSUS.exe: runSUS.o src.a
	$(CC) $(CFLAGS) runSUS.o src.a -o runSUS.exe

creditos.o:
	$(CC) $(CFLAGS) -c Headers\creditos.c

game.o:
	$(CC) $(CFLAGS) -c Headers\game.c

menu.o:
	$(CC) $(CFLAGS) -c Headers\menu.c

placar.o:
	$(CC) $(CFLAGS) -c Headers\placar.c

sair.o:
	$(CC) $(CFLAGS) -c Headers\sair.c

utils.o:
	$(CC) $(CFLAGS) -c Headers\utils.c

tags:
	ctags -R

.PHONY: clean
clean:
	del *.o
	del *.a
	del *.exe
