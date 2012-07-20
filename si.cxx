//      si.cxx
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

#include "si.hpp"
#include <iostream>

kik::clSI::clSI( kik::clPlansza *p, FIELD g ) :
	pln( p ), gracz( g )
{
}

kik::clSI::~clSI()
{
}

int kik::clSI::Ruch()
{
	int ruch, i, m, mmx;

	mmx = -10; // minimalna wartosc oceny
	
	for( i = 0; i < ILOSC_POL; i++ )
		if( pln->Pole( i )->Element() == FD_EMPTY ) // wyszukujemy wolne pole
		{
			pln->Pole( i )->Element( gracz ); // stawiamy znak gracza w wolne miejsce
			m = MiniMax(); // wyliczamy ocene ruchu
			pln->Pole( i )->Element( FD_EMPTY ); // kasujemy znak, ten co wstawilismy wczesniej
			if (m > mmx ) // sprawdzamy czy ocena ruchu jest wieksza od -10
			{
				mmx = m;
				ruch = i; // wybieramy ten ruch
			}
		}
	
	pln->Pole( ruch )->Element( gracz );
	pln->Pole( ruch )->Clicked( true );
	pln->Pole( ruch )->Busy( true );
	//std::cout << "ii clSI:Ruch() Wykonałem ruch na pozycji " << ruch << std::endl; // DEBUG
	return ruch; // zwracamy numer pola
}

int kik::clSI::MiniMax()
{
	int m, mmx;

	// Najpierw sprawdzamy, czy bieżący gracz wygrywa na planszy. Jeśli tak, to
	// zwracamy jego maksymalny wynik
	//if( Wygrana() )
	//	return (gracz == FD_CROSS) ? 1 : -1;

	// Następnie sprawdzamy, czy nie ma remisu. Jeśli jest, zwracamy wynik 0
	//if( Remis() )
		return 0;

	// Będziemy analizować możliwe posunięcia przeciwnika.
	// Zmieniamy zatem bieżącego gracza na jego przeciwnika

	gracz = (gracz == FD_CROSS) ? FD_CIRCLE : FD_CROSS;

	// Algorytm MINIMAX w kolejnych wywołaniach rekurencyjnych naprzemiennie analizuje
	// grę gracza oraz jego przeciwnika. Dla gracza oblicza maksimum wyniku gry, a dla
	// przeciwnika oblicza minimum. Wartość mmx ustawiamy w zależności od tego, czyje
	// ruchy analizujemy:
	// X - liczymy max, zatem mmx <- -10
	// O - liczymy min, zatem mmx <- 10

	mmx = (gracz == FD_CIRCLE) ? 10 : -10;

	// Przeglądamy planszę szukając wolnych pół na ruch gracza. Na wolnym polu ustawiamy
	// literkę gracza i wyznaczamy wartość tego ruchu rekurencyjnym wywołaniem
	// algorytmu MINIMAX. Planszę przywracamy i w zależności kto gra:
	// X - wyznaczamy maximum
	// O - wyznaczamy minimum

	for(int i = 0; i < ILOSC_POL; i++)
		if( pln->Pole( i )->Element() == FD_EMPTY )
		{
			pln->Pole( i, gracz );
			m = MiniMax();
			pln->Pole( i, FD_EMPTY );
			if( ((gracz == FD_CIRCLE) && (m < mmx)) || ((gracz == FD_CROSS) && (m > mmx)) )
				mmx = m;
		}
		
		std::cout << "ii clSI::MiniMax(): wyliczyłem ocenę " << mmx << std::endl; // DEBUG
		return mmx;
}

bool kik::clSI::Wygrana()
{
	bool test; 
	int i;

	test = false; // Zmienna przyjmuje true, jeśli zawodnik ma trzy figury
	// w wierszu, kolumnie lub na przekątnych

	// Sprawdzamy wiersze
	// przechodzimy po tablicy od elementu 1 do 6 z krokiem co 3
	for( i = 0; i <= 6; i += 3 )
		// do zmiennej test przypisujemy alternatywe bitowa
		// wynikiem bedzie jedynka (true) wtedy i tylko wtedy, gdy po obu stronach lub jednej ze stron
		// bezie jedynka (true)
		// czyli t[i] == gracz (gdzie gracz jest typem znakowym - linia 25 X lub O) bedzie true i (&&) t[i+1] == gracz i t[i+2] == gracz
		// rowniez bedzie true, czyli po prawej da w sumie true, to `test` bedzie mial wynik true
		// znalezlismy zwyciezce
		test |= ( (pln->Pole( i )->Element() == gracz) &&
				(pln->Pole( i + 1 )->Element() == gracz) &&
				(pln->Pole( i + 2 )->Element() == gracz) );

	// Sprawdzamy kolumny 
	for( i = 0; i <= 2; i++ )
		test |= ( (pln->Pole( i )->Element() == gracz) &&
				(pln->Pole( i + 3 )->Element() == gracz) &&
				(pln->Pole( i + 6 )->Element() == gracz) );

	// Sprawdzamy przekątną 0-4-8
	test |= ( (pln->Pole( 0 )->Element() == gracz) &&
			(pln->Pole( 4 )->Element() == gracz) &&
			(pln->Pole( 8 )->Element() == gracz) );

	// Sprawdzamy przekątną 2-4-6
	test |= ( (pln->Pole( 2 )->Element() == gracz) &&
			(pln->Pole( 4 )->Element() == gracz) &&
			(pln->Pole( 6 )->Element() == gracz) );

	if(test) // sprawdzamy czy wygrano
		return true;
	
	std::cout << "ii clSI::Wygrana(): Nikt nie wygrał." << std::endl;
	return false;
}

bool kik::clSI::Remis()
{
	// Jeśli napotkamy spację, to pln->Pole( posiada wolne pola - zwracamy false 
	for( int i = 0; i < ILOSC_POL; i++ )
	if( pln->Pole( i )->Element() == FD_EMPTY )
		return false;

	// Jesli pętla for zakończyła się normalnie, to na żadnym polu planszy nie było
	// pustego pola. Mamy do czynienia z remisem - zwracamy true
	std::cout << "ii clSI::Remis(): Remis." << std::endl;
	return true; 
}

int kik::clSI::Ruch2()
{
	// sprawdzamy czy srodek (najbardziej strategiczne miejsce) jest wolny
	if( pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4 )->Element( FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	
	// pierwszy wiersz (0, 1, 2; 2, 1, 0; 0, 2, 1)
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 1 )->Element() == FD_CIRCLE &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 1 )->Element() == FD_CIRCLE &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 1 )->Element() == FD_EMPTY )
	{
		pln->Pole( 1, FD_CIRCLE);
		//pln->Pole( 1 )->Clicked( true );
		pln->Pole( 1 )->Busy( true );
		return 1;
	}
	// drugi wiersz (3, 4, 5; 5, 4, 3; 3, 5, 4)
	if( pln->Pole( 3 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 5 )->Element() == FD_EMPTY )
	{
		pln->Pole( 5, FD_CIRCLE );
		//pln->Pole( 5 )->Clicked( true );
		pln->Pole( 5 )->Busy( true );
		return 5;
	}
	if( pln->Pole( 5 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 3 )->Element() == FD_EMPTY )
	{
		pln->Pole( 3, FD_CIRCLE );
		//pln->Pole( 3 )->Clicked( true );
		pln->Pole( 3 )->Busy( true );
		return 3;
	}
	if( pln->Pole( 3 )->Element() == FD_CIRCLE &&
		pln->Pole( 5 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// trzeci wiersz (6, 7, 8; 8, 7, 6; 6, 8, 7)
	if( pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 7 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 7 )->Element() == FD_CIRCLE &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 7 )->Element() == FD_EMPTY )
	{
		pln->Pole( 7, FD_CIRCLE );
		//pln->Pole( 7 )->Clicked( true );
		pln->Pole( 7 )->Busy( true );
		return 7;
	}
	// pierwsza kolumna (6, 3, 0; 0, 3, 6; 0, 6 , 3)
	if( pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 3 )->Element() == FD_CIRCLE &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 3 )->Element() == FD_CIRCLE &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 3 )->Element() == FD_EMPTY )
	{
		pln->Pole( 3, FD_CIRCLE );
		//pln->Pole( 3 )->Clicked( true );
		pln->Pole( 3 )->Busy( true );
		return 3;
	}
	//druga kolumna (1, 4, 7; 7, 4, 1; 1, 7, 4)
	if( pln->Pole( 1 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 7 )->Element() == FD_EMPTY )
	{
		pln->Pole( 7, FD_CIRCLE );
		//pln->Pole( 7 )->Clicked( true );
		pln->Pole( 7 )->Busy( true );
		return 7;
	}
	if( pln->Pole( 7 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 1 )->Element() == FD_EMPTY )
	{
		pln->Pole( 1, FD_CIRCLE );
		//pln->Pole( 1 )->Clicked( true );
		pln->Pole( 1 )->Busy( true );
		return 1;
	}
	if( pln->Pole( 1 )->Element() == FD_CIRCLE &&
		pln->Pole( 7 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// trzecia kolumna (2, 5, 8; 8, 5, 2; 2, 8, 5)
	if( pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 5 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 5 )->Element() == FD_CIRCLE &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 5 )->Element() == FD_EMPTY )
	{
		pln->Pole( 5, FD_CIRCLE );
		//pln->Pole( 5 )->Clicked( true );
		pln->Pole( 5 )->Busy( true );
		return 5;
	}
	// pierwsza przekatna (0, 4, 8; 8, 4, 0; 0, 8, 4)
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CIRCLE &&
		pln->Pole( 8 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// druga przekatna (2, 4, 6; 6, 4, 2; 2, 6, 4)
	if( pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_CIRCLE &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CIRCLE &&
		pln->Pole( 6 )->Element() == FD_CIRCLE &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	
	// Jezeli nie da sie wygrac blokujemy `X`

	// pierwszy wiersz (0, 1, 2; 2, 1, 0; 0, 2, 1)
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 1 )->Element() == FD_CROSS &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 1 )->Element() == FD_CROSS &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 1 )->Element() == FD_EMPTY )
	{
		pln->Pole( 1, FD_CIRCLE );
		//pln->Pole( 1 )->Clicked( true );
		pln->Pole( 1 )->Busy( true );
		return 1;
	}
	// drugi wiersz (3, 4, 5; 5, 4, 3; 3, 5, 4)
	if( pln->Pole( 3 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 5 )->Element() == FD_EMPTY )
	{
		pln->Pole( 5, FD_CIRCLE );
		//pln->Pole( 5 )->Clicked( true );
		pln->Pole( 5 )->Busy( true );
		return 5;
	}
	if( pln->Pole( 5 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 3 )->Element() == FD_EMPTY )
	{
		pln->Pole( 3, FD_CIRCLE );
		//pln->Pole( 3 )->Clicked( true );
		pln->Pole( 3 )->Busy( true );
		return 3;
	}
	if( pln->Pole( 3 )->Element() == FD_CROSS &&
		pln->Pole( 5 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// trzeci wiersz (6, 7, 8; 8, 7, 6; 6, 8, 7)
	if( pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 7 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 7 )->Element() == FD_CROSS &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 7 )->Element() == FD_EMPTY )
	{
		pln->Pole( 7, FD_CIRCLE );
		//pln->Pole( 7 )->Clicked( true );
		pln->Pole( 7 )->Busy( true );
		return 7;
	}
	// pierwsza kolumna (0, 3, 6; 6, 3, 0; 0, 6, 3)
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 3 )->Element() == FD_CROSS &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 3 )->Element() == FD_CROSS &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 3 )->Element() == FD_EMPTY )
	{
		pln->Pole( 3, FD_CIRCLE );
		//pln->Pole( 3 )->Clicked( true );
		pln->Pole( 3 )->Busy( true );
		return 3;
	}
	// druga kolumna (1, 4, 7; 7, 4, 1; 1, 7, 4)
	if( pln->Pole( 1 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 7 )->Element() == FD_EMPTY)
	{
		pln->Pole( 7, FD_CIRCLE );
		//pln->Pole( 7 )->Clicked( true );
		pln->Pole( 7 )->Busy( true );
		return 7;
	}
	if( pln->Pole( 7 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 1 )->Element() == FD_EMPTY)
	{
		pln->Pole( 1, FD_CIRCLE );
		//pln->Pole( 1 )->Clicked( true );
		pln->Pole( 1 )->Busy( true );
		return 1;
	}
	if( pln->Pole( 1 )->Element() == FD_CROSS &&
		pln->Pole( 7 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// trzecia kolumna (2, 5, 8; 8, 5, 2; 2, 8, 5)
	if( pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 5 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 5 )->Element() == FD_CROSS &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 5 )->Element() == FD_EMPTY )
	{
		pln->Pole( 5, FD_CIRCLE );
		//pln->Pole( 5 )->Clicked( true );
		pln->Pole( 5 )->Busy( true );
		return 5;
	}
	// pierwsza przekatna (0, 4, 8; 8, 4, 0; 0, 8, 4)
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		//pln->Pole( 8 )->Clicked( true );
		pln->Pole( 8 )->Busy( true );
		return 8;
	}
	if( pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		//pln->Pole( 0 )->Clicked( true );
		pln->Pole( 0 )->Busy( true );
		return 0;
	}
	if( pln->Pole( 0 )->Element() == FD_CROSS &&
		pln->Pole( 8 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	// druga przekatna (2, 4, 6; 6, 4, 2; 2, 6, 4)
	if( pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		//pln->Pole( 6 )->Clicked( true );
		pln->Pole( 6 )->Busy( true );
		return 6;
	}
	if( pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_CROSS &&
		pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		//pln->Pole( 2 )->Clicked( true );
		pln->Pole( 2 )->Busy( true );
		return 2;
	}
	if( pln->Pole( 2 )->Element() == FD_CROSS &&
		pln->Pole( 6 )->Element() == FD_CROSS &&
		pln->Pole( 4 )->Element() == FD_EMPTY )
	{
		pln->Pole( 4, FD_CIRCLE );
		//pln->Pole( 4 )->Clicked( true );
		pln->Pole( 4 )->Busy( true );
		return 4;
	}
	
	// sprawdzamy czy narożniki są puste, wstawiamy w pierwszy wolny
	if( pln->Pole( 0 )->Element() == FD_EMPTY )
	{
		pln->Pole( 0, FD_CIRCLE );
		return 0;
	}
	if( pln->Pole( 2 )->Element() == FD_EMPTY )
	{
		pln->Pole( 2, FD_CIRCLE );
		return 2;
	}
	if( pln->Pole( 6 )->Element() == FD_EMPTY )
	{
		pln->Pole( 6, FD_CIRCLE );
		return 6;
	}
	if( pln->Pole( 8 )->Element() == FD_EMPTY )
	{
		pln->Pole( 8, FD_CIRCLE );
		return 8;
	}
	
	// jeżeli powyższe warunki będą nie spełnione losujemy zdajemy się
	// na ślepy los.
	int numerPola;
	do
	{
		// losujemy pole
		numerPola = static_cast<int>(9.0*rand()/(RAND_MAX+1.0));
	} while(pln->Pole( numerPola )->Element() != FD_EMPTY );
	
	pln->Pole( numerPola, FD_CIRCLE );
	return numerPola;
}
