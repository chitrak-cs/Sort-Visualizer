CC = gcc

SRC = \
src/main.c \
src/array.c \
src/renderer.c \
src/algorithms/bubble_sort.c \
src/algorithm_manager.c \
src/metrics.c

CFLAGS = $(shell pkg-config --cflags raylib)

LIBS = -lraylib -lm -ldl -lpthread -lrt -lGL -lX11

visualizer:
	$(CC) $(SRC) -o visualizer $(CFLAGS) $(LIBS)

clean:
	rm -f visualizer