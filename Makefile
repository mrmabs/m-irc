CC = gcc
CFLAGS = -Wall
LIBS =
OBJS = inet.o irc.o main.o display.o

all: m-irc

m-irc: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o m-irc $(OBJS)

#inet.o: inet.c m-irc.h
#	$(CC) $(CFLAGS) -c -o inet.o inet.c
#
#irc.o: irc.c m-irc.h
#	$(CC) $(CFLAGS) -c -o irc.o irc.c
#
#main.o: main.c m-irc.h
#	$(CC) $(CFLAGS) -c -o main.o main.c
#
#display.o: display.c m-irc.h
#	$(CC) $(CFLAGS) -c -o display.o display.c

clean:
	rm -f *.o
	rm -f m-irc
