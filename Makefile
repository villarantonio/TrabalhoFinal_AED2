CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c99
SRC     = src/grafo.c src/heap.c src/dijkstra.c src/metricas.c src/hash.c src/main.c
OBJ     = $(SRC:.c=.o)
TARGET  = rotas_goias

.PHONY: all clean valgrind

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)
