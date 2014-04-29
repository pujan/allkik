# Makefile dla allKiK v. 1.0
CC = g++
LDFLAGS = `allegro-config --libs`
CXXFLAGS = `allegro-config --cppflags` -Wall -g -ggdb3
OBJS = main.o area.o plansza.o si.o
PROGRAM = allkik

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
		$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

main.o:  main.cxx game.h
		$(CC) $(CXXFLAGS) -c main.cxx

area.o: area.cxx area.hpp game.h
		$(CC) $(CXXFLAGS) -c area.cxx

plansza.o: plansza.cxx plansza.hpp game.h
		$(CC) $(CXXFLAGS) -c plansza.cxx

si.o: si.cxx si.hpp game.h plansza.hpp
		$(CC) $(CXXFLAGS) -c si.cxx

clean:
		rm -f $(PROGRAM) $(OBJS)

