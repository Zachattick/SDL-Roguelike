CC = cc
CFLAGS = $(shell sdl2-config --cflags) -Wall -Wextra -Iinclude
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lm

TARGET = build/program
SRC = src/main.c src/entity.c src/utils.c src/input.c src/collision.c src/score.c src/player.c src/enemy.c

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)