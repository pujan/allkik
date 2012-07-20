//      game.h
//      
//      Copyright 2010 Łukasz "Pujan" Pelc <gangster2000@wp.pl>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef _GAME_H_
#define _GAME_H_

#include <allegro.h>
#include <string>
//#include "plansza.hpp"

/* zmienne globalne */

// wersja gry
#define WERSJA "0.1 BETA"

// rozdzielczosc ekranu
#define WIDTH 800
#define HEIGHT 600

// rozmiar pol
#define W_AREA 120
#define H_AREA 120

// pozycja wyswietlenia bierzącego gracza
#define X_TURN 600
#define Y_TURN 215

// ilosc pol na planszy
#define ILOSC_POL 9

// rodzaj pola na planszy
	enum FIELD{ FD_EMPTY, FD_CIRCLE, FD_CROSS };
// rodzaj znaku, ktorymi gracz sie moze poslugiwac
	enum TURN{ TN_CROSS = 'O', TN_CIRCLE = 'X' };
// wynik gry
	enum WYNIK{ WN_KONT, WN_REMIS, WN_CIRCLE, WN_CROSS };
// odpowiedz na pytanie
	enum ANSWER{ AN_YES, AN_NO };
// rodzaj gry
	enum GAME{ GM_COMP_COMP, GM_HUM_COMP, GM_HUM_HUM, GM_EXIT };

/* funkcje */

// funkcja inicjuje biblioteke allegro oraz przygowuje gre do startu
void init_game();

// funkcja zamyka biblioteke allegro i sprzata pamiec
void exit_game();

// losuje gracza, ktory ma zaczynac gre
void wylosuj_gracza();

// funkcja laduje potzrebne pliki graficzne
// zwraca -1 jezeli jakiegos pliku nie moze zaladowac; 0 gdy wszystko OK
int load_graphics();

// funkcja rysujaca; rysuje na ekranie
void draw();

// funkcja odpowiedzialna za dzialnie interfejsu
void logic();

// funkcja wyswietljaca komunikat i zamykajaca program
// przyjmuje wskaznik na lancuch znakow
void error( const char * );

// funkcja odpowiedzialna za grę komputera
// SZTUCZNA INTELIGENCJA
//void AITurn( FIELD board[][ 3 ] );
//void komputer_ai( clPlansza * );

// wyswietla pytanie i czeka na odpowiedź użytkownika
ANSWER question( std::string str );

#endif

