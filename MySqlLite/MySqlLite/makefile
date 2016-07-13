CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=
EXEC=mysqllite

all: $(EXEC)

mysqllite: command.o hashMap.o sql.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)