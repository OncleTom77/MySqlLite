CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=
EXEC=mysqllite

all: $(EXEC)

mysqllite: command.o hashmap_entry.o hashMap.o json.o sql.o utils.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)