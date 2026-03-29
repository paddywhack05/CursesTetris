CC = gcc
CFLAGS = -DNCURSESW_STATIC
LDFLAGS = -lncursesw

TARGET = tetris
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)