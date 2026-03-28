CC = gcc
CFLAGS = -DNCURSES_STATIC
LDFLAGS = -lncurses

TARGET = tetris
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)