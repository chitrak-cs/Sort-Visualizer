CC = gcc

TARGET = visualizer

SRC = \
src/main.c \
src/array.c \
src/renderer.c \
src/algorithm_manager.c \
src/metrics.c \
src/algorithms/bubble_sort.c \
src/algorithms/selection_sort.c \
src/algorithms/insertion_sort.c \
src/algorithms/merge_sort.c \
src/algorithms/quick_sort.c \
src/algorithms/heap_sort.c

CFLAGS = \
-I./src \
-I./src/algorithms \
$(shell pkg-config --cflags raylib)

LIBS = \
-lraylib \
-lm \
-ldl \
-lpthread \
-lrt \
-lGL \
-lX11

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean $(TARGET)

.PHONY: clean run rebuild