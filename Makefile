# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lncurses

# Output executable name
TARGET = rogue_game

# Source files
SRCS = main.c map.c player.c combat.c menu.c settings.c save_load.c scoreboard.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
