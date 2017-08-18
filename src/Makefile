# Makefile
C     = gcc -lSDL
CC     = gcc 
DEBUG  = -g -DDEBUG
CFLAGS = -ansi -Wall
LFLAGS = -ansi -Wall  -c

BDIR = ../bin

all: serverfinal client

debug: serverD clientD

test: testgui testgui_two test-comm testgui_map

client: clienttest.c
	$(CC) clienttest.c -o client ; mv client ../bin

clientD: clienttest.c 
	$(CC) clienttest.c -o clientD ; mv clientD ../bin

testgui_map: maptest.o map.o utils.o
	$(CC) maptest.o map.o utils.o -o testgui_map `pkg-config --cflags --libs gtk+-2.0` ; mv testgui_map ../bin

testgui_two: client.c
	$(CC) client.c -o testgui_two `pkg-config --cflags --libs cairo`  `pkg-config --cflags --libs gtk+-2.0`; mv testgui_two ../bin
	
testgui: testgui.c
	$(CC) testgui.c -o testgui `pkg-config --cflags --libs cairo`  `pkg-config --cflags --libs gtk+-2.0`; mv testgui ../bin

test-comm: clienttest.c
	$(CC) clienttest.c -o test-comm -DCOMTEST; mv test-comm ../bin

ServNoGUI: Coordinate.o ParseAid.o Taxi.o Trip.o Route.o Parser.o map.o utils.o Server.o RouteAid.o Money.o Conversion.o
	$(CC) $(DEBUG) Taxi.o Trip.o Coordinate.o Server.o RouteAid.o Route.o Parser.o ParseAid.o map.o utils.o Money.o Conversion.o -o ServNoGUI -lm; mv ServNoGUI ../bin; cp TinyTown.map ../bin

servers: Coordinate.o ParseAid.o Taxi.o Trip.o Route.o Parser.o map.o utils.o RouteAid.o Money.o Conversion.o maptest.o
	$(CC) $(DEBUG) maptest.o Taxi.o Trip.o Coordinate.o RouteAid.o Route.o Parser.o ParseAid.o map.o utils.o Money.o Conversion.o -`pkg-config --cflags --libs gtk+-2.0` -o taxi -lm; mv taxi ../bin; cp TinyTown.map ../bin

serverfinal: Coordinate.o ParseAid.o Taxi.o Trip.o Route.o Parser.o map.o utils.o RouteAid.o Money.o Conversion.o maptest.o serverfinal.o
	$(CC) $(CFLAGS) serverfinal.o Taxi.o Trip.o Coordinate.o RouteAid.o Route.o Parser.o ParseAid.o map.o utils.o Money.o Conversion.o -`pkg-config --cflags --libs gtk+-2.0` -o serverfinal -lm; mv serverfinal ../bin; cp TinyTown.map ../bin

serverD: Coordinate.o ParseAid.o Taxi.o Trip.o Route.o Parser.o map.o utils.o RouteAid.o Money.o Conversion.o maptest.o serverfinal.o
	$(CC) $(DEBUG) serverfinal.o Taxi.o Trip.o Coordinate.o RouteAid.o Route.o Parser.o ParseAid.o map.o utils.o Money.o Conversion.o -`pkg-config --cflags --libs gtk+-2.0` -o serverfinalD -lm; mv serverfinalD ../bin; cp TinyTown.map ../bin

# NEEDED BY TAXI
serverfinal.o: serverfinal.c
	$(CC) $(LFLAGS) -c serverfinal.c -o serverfinal.o `pkg-config --cflags --libs cairo`  `pkg-config --cflags --libs gtk+-2.0` -g

Conversion.o: Conversion.c
	$(CC) $(LFLAGS) -c Conversion.c -o Conversion.o -g 

Server.o: Server.c map.h
	$(CC) $(LFLAGS) -c Server.c -o Server.o -g

map.o: map.c
	$(CC) -c map.c -o map.o -g

map_test.o: map_test.c
	$(CC) $(LFLAGS) map_test.c -o map_test.o -g `pkg-config --cflags --libs cairo`  `pkg-config --cflags --libs gtk+-2.0`

maptest.o: maptest.c
	$(CC) -c maptest.c -o maptest.o  `pkg-config --cflags --libs gtk+-2.0`

Money.o: Money.c
	$(CC) -c Money.c -o Money.o -g

utils.o: utils.c
	$(CC) -c utils.c -o utils.o -g

utils_test.o: utils_test.c
	$(CC) $(LFLAGS) utils_test.c -o utils_test.o -g

config.o: config.c
	$(CC) -c config.c -o config.o -g

Route.o: Route.c
	$(CC) $(LFLAGS) Route.c -o Route.o -g

RouteAid.o: RouteAid.c
	$(CC) $(LFLAGS) RouteAid.c -o RouteAid.o -g

Trip.o: Trip.c
	$(CC) $(LFLAGS) Trip.c -o Trip.o -g

Taxi.o: Taxi.c
	$(CC) $(LFLAGS) Taxi.c -o Taxi.o -g

ParseAid.o: ParseAid.c
	$(CC) $(LFLAGS) ParseAid.c -o ParseAid.o -g

Parser.o: Parser.c
	$(CC) $(LFLAGS) Parser.c -o Parser.o -g

Coordinate.o: Coordinate.c
	$(CC) $(LFLAGS) Coordinate.c -o Coordinate.o -g

clean:
	rm -f *.o; rm -f ../bin/serverfinal; rm -f ../bin/test; rm -f ../bin/test-comm; rm -f ../bin/testgui; rm -f ../bin/TinyTown.map; rm -f ../bin/testgui_two; rm -f ../bin/testgui_map; rm -f ../bin/client; rm -f ../bin/clientD; rm -f ../bin/serverfinalD

#EOF


