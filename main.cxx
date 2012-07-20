#include "game.h"
#include "area.hpp"
#include "plansza.hpp"
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include "si.hpp"
#include <unistd.h>

// bitmapy wykorzystane w grze
BITMAP *plansza = NULL;
BITMAP *kolko = NULL;
BITMAP *krzyzyk = NULL;
BITMAP *kursor = NULL;
BITMAP *bufor = NULL;
// tablica bitmap, osobna dla każdego pola na planszy
BITMAP *tmpbitmaps[ ILOSC_POL ];

// zmienna jest straznikiem dzialania programu
bool wyjscie;

// strażnik zakończenia wykonywania ruchu
bool wykonajRuch;

// wyswietlenie menu
bool menu;

// czy gra sztuczna inteligencja
bool ai;

// makrodefinicję dla tablicy graczy
#define GRACZ_1    0
#define GRACZ_2    1
#define KOMPUTER   2
#define KOMPUTER_2 3
#define LICZBA_GRACZY 4

// aktywny gracz
bool aktywny_gracz[ LICZBA_GRACZY ];

// wskaznik na sztuczna inteligencje
kik::clSI *si;

// ruch gracza
TURN gracz;

// wskaznik do planszy
kik::clPlansza *pln;

// zmienna zegara
volatile long ticks = 0;

void timerek()
{
	ticks++;
}

LOCK_VARIABLE( ticks );
LOCK_FUNCTION( timerek );

//  inicjalizacja allegro i gry
void init_game()
{
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();
	install_int_ex( timerek, BPS_TO_TIMER(100) );
	set_color_depth( 24 );
	set_gfx_mode( GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0 );
	clear_to_color( screen, makecol(0, 0, 255) );
	load_graphics();
	wyjscie = false;
	wykonajRuch = true;
	menu = true;
	si = NULL;
	pln = new kik::clPlansza( /*area*/ );
	wylosuj_gracza();
	//aktywny_gracz[ GRACZ_1 ] = false;
	aktywny_gracz[ GRACZ_2 ] = false;
	//aktywny_gracz[ KOMPUTER ] = false;
	
}

void exit_game()
{
	destroy_bitmap( plansza );
	destroy_bitmap( kolko );
	destroy_bitmap( krzyzyk );
	destroy_bitmap( kursor );
	destroy_bitmap( bufor );
	// usuwamy tablicę bitmap
	for( int i = 0; i < ILOSC_POL; i++ )
		destroy_bitmap( tmpbitmaps[ i ] );
	remove_int( timerek );
	allegro_exit();
	delete pln;
}



void wylosuj_gracza()
{
	srand( static_cast<unsigned>( time( NULL ) ) );
	gracz = ( rand() % 2 == 0 ? TN_CIRCLE : TN_CROSS );
	//std::cout << "Rozpoczynają: " << (gracz == TN_CIRCLE ? "Krzyżyki" : "Kółka") << std::endl;
}

int load_graphics()
{
	plansza = load_tga( "plansza.tga", NULL );
	if( plansza == NULL )
	{
		error( "Nie moge zaladowac pliku plansza.tga!" );
		return -1;
	}
	kolko = load_tga( "kolko.tga", NULL );
	if( kolko == NULL )
	{
		error( "Nie moge zaladowac pliku kolko.tga!" );
		return -1;
	}
	krzyzyk = load_tga( "krzyzyk.tga", NULL );
	if( krzyzyk == NULL )
	{
		error( "Nie moge zaladowac pliku krzyzyk.tga!" );
		return -1;
	}
	kursor = load_tga( "kursor.tga", NULL );
	if( kursor == NULL )
	{
		error( "Nie moge zaladowac pliku kursor.tga!" );
		return -1;
	}
	bufor = create_bitmap( WIDTH, HEIGHT );
	if( bufor == NULL )
	{
		error( "Nie moge utworzyc buforu!" );
		return -1;
	}
	
	for( int i = 0; i < ILOSC_POL; i++ )
	{
		tmpbitmaps[ i ] = create_bitmap( H_AREA, W_AREA );
		if( tmpbitmaps[ i ] == NULL )
		{
			error( "Nie moge utworzyc potrzebnych bitmap" );
			return -1;
		}
	}
	
	return 0;
}

void error( const char *msg )
{
	set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
	allegro_message( msg );
	allegro_exit();
}

WYNIK check_winner( kik::clPlansza *p )
{
	// sprawdzamy czy ktos wygral
	FIELD znakWygrany, znak;
	unsigned iloscZnakow, i;
	
	for( i = 0; i < 8; i++ ) // 8 sposobów na wygraną
	{
		// zerujemy zmienne
		znakWygrany = znak = FD_EMPTY;
		iloscZnakow = 0;
		for( unsigned j = 0; j < 3; j++ )
		{
			znak = p->Pole( kik::linie[ i ][ j ] )->Element();
			// DEBUG
			/*std::cout << "ii sprawdzany znak: "
				<< (znak == FD_CIRCLE ? "Kółko" : (znak == FD_CROSS ? "Krzyżyk" : "Pusto"))
				<< std::endl << "ii zmienna i = " << i << std::endl << "ii zmienna j = " << j << std::endl;*/
			// DEBUG
			if( znak != znakWygrany )
			{
				znakWygrany = znak;
				iloscZnakow = 1;
			}
			else
				++iloscZnakow;
		}
		
		if( (iloscZnakow == 3) && (znakWygrany != FD_EMPTY) )
		{
			if( znakWygrany == FD_CIRCLE )
				return WN_CIRCLE;
			else
				if( znakWygrany == FD_CROSS )
					return WN_CROSS;
		}
	}
	
	iloscZnakow = 0;
	
	// moze zremisowano?
	for( i = 0; i < ILOSC_POL; i++ )
		if( p->Pole( i )->Element() != FD_EMPTY )
			++iloscZnakow;
	
	if( iloscZnakow == ILOSC_POL )
		return WN_REMIS;
	
	return WN_KONT;
}

void restart()
{
	std::cout << "ii Restart gry." << std::endl;
	
	for( int i = 0; i < ILOSC_POL; i++ )
	{
		pln->Pole( i )->Element( FD_EMPTY );
		pln->Pole( i )->Busy( false );
		pln->Pole( i )->Clicked(  false );
		pln->Pole( i )->LBMPressed( false );
	}
		
	wykonajRuch = true;
	wylosuj_gracza();
	
	if( ai && (gracz == TN_CIRCLE) )
	{
		aktywny_gracz[ KOMPUTER ] = true;
		aktywny_gracz[ GRACZ_1 ] = false;
		aktywny_gracz[ GRACZ_2 ] = false;
	}
	else
		if( ai && (gracz == TN_CROSS) )
		{
			aktywny_gracz[ KOMPUTER ] = false;
			aktywny_gracz[ GRACZ_1 ] = true;
			aktywny_gracz[ GRACZ_2 ] = false;
		}
		else
			{
				aktywny_gracz[ KOMPUTER ] = false;
				aktywny_gracz[ GRACZ_1 ] = true;
				aktywny_gracz[ GRACZ_2 ] = false;
			}
}

ANSWER question( std::string str )
{
	BITMAP *msg = create_bitmap( str.size() * 8 - 13, 30 );
	clear_to_color( msg, makecol(0, 0, 0) );
	textout_ex( msg, font, str.c_str(), 10, 12, makecol(255, 255, 255), -1 );
	blit( msg, screen, 0, 0, (WIDTH / 2 - 200), (HEIGHT / 2), msg->w, msg->h );
	
	while( 1 )
	{
		if( key[ KEY_T ] || key[ KEY_Y ] )
			return AN_YES;
		else
			if( key[ KEY_N ] || key[ KEY_ESC ] )
				return AN_NO;
	}
}

void draw()
{
	// aktualne pole
	kik::clArea *area;
	
	// czyscimy bufor
	clear_to_color( bufor, makecol(0, 0, 255) );
	
	// umieszczamy plansze na ekranie
	blit( plansza, bufor, 0, 0, 50, 50, plansza->w, plansza->h );
	
	// wyświetlamy aktualny znak gracza
	gracz == TN_CIRCLE ? 
		blit( kolko, bufor, 0, 0, X_TURN, Y_TURN, kolko->w, kolko->h )
		: blit( krzyzyk, bufor, 0, 0, X_TURN, Y_TURN, krzyzyk->w, krzyzyk->h );
	textout_ex( bufor, font, "Teraz gracz posiadający:", X_TURN-50, Y_TURN-20,
		makecol(255, 255, 255), -1 );
	
	// kolorujemy bitmapy i umieszczay na buforze
	for( int i = 0; i < ILOSC_POL; i++ )
	{
		area = pln->Pole( i );
		
		if( area->MouseOn() && (! area->Busy()) )
			clear_to_color( tmpbitmaps[ i ], makecol(205, 205, 205) );
		else
			clear_to_color( tmpbitmaps[ i ], makecol(172, 172, 172) );
		
		// wstawiamy kolko i krzyzyk na plansze
		if( area->Busy() )
		{
			//std::cout << "ii Pole " << i << " jest zajęte." << std::endl;
			switch( area->Element() )
			{
				case FD_CIRCLE:
					blit( kolko, tmpbitmaps[ i ], 0, 0, 0, 0,
						  kolko->w, kolko->h );
						//textout_ex( bufor, font, "Kolko", 500, i+10, 0, -1 );
						//std::cout << "ii Kółko na polu " << i << std::endl;
				break;
				case FD_CROSS:
					blit( krzyzyk, tmpbitmaps[ i ], 0, 0, 0, 0,
						  krzyzyk->w, krzyzyk->h );
						//textout_ex( bufor, font, "Krzyzyk", 500, i+25, 0, -1 );
						//std::cout << "ii Krzyżyk na polu " << i << std::endl;
				break;
				case FD_EMPTY:
				break;
			}
		}
		
		blit( tmpbitmaps[ i ], bufor, 0, 0, area->CordX(), area->CordY(),
			  tmpbitmaps[ i ]->w, tmpbitmaps[ i ]->h ); //pln->Pole( i ).Width(), pln->Pole( i ).Height() );
	}
	
	// kopiujemy kursosr na bufor
	draw_sprite( bufor, kursor, mouse_x, mouse_y );
	
	// kopiujemy bufor na ekran
	blit( bufor, screen, 0, 0, 0, 0, bufor->w, bufor->h );
}

bool game_over()
{
	switch( check_winner(pln) )
	{
		case WN_KONT:
			// jezeli gra nadal sie toczy zmieniamy gracza
			gracz = ( gracz == TN_CIRCLE ? TN_CROSS : TN_CIRCLE );
		break;
		case WN_CIRCLE:
			std::cout << "ii Wygrały kółka! Koniec gry!" << std::endl;
							
			draw(); // wyświetlenie ostatniego umieszczonego znaku
			if( question("Wygrały kółka! Czy chcesz rozpocząć nową grę? [t/n]") == AN_YES )
				//restart();
				return true;
			else
				{
					wykonajRuch = false;
					menu = true;
					//return false;
				}
		break;
		case WN_CROSS:
			std::cout << "ii Wygrały krzyżyki! Koniec gry!" << std::endl;
							
			draw(); // wyświetlenie ostatniego umieszczonego znaku
			if( question("Wygrały Krzyżyki! Czy chcesz rozpocząć nową grę? [t/n]") == AN_YES )
				//restart();
				return true;
			else
				{
					wykonajRuch = false;
					menu = true;
					//return false;
				}
		break;
		case WN_REMIS:
			std::cout << "ii Remis! Koniec gry!" << std::endl;
							
			draw(); // wyświetlenie ostatniego umieszczonego znaku
			if( question("Remis! Czy chcesz rozpocząć nową grę? [t/n]") == AN_YES )
				//restart();
				return true;
			else
				{
					wykonajRuch = false;
					menu = true;
					//return false;
				}
		break;
	} // switch
	// oświeżamy planszę
	draw();
	
	return false;
}

void logic()
{
	// sprawdzamy czy ma byc wyswietlne menu, jezeli tak to wychodzimy
	if( menu )
		return;
	
	if( key[ KEY_ESC ] )
	{
		menu = true;
		clear_keybuf();
		sleep( 1 ); // czekamy 1 sekundę
		return;
	}
	else
		if( key[ KEY_R ] )
		{
			restart();
			ticks = -1;
			return;
		}
	
	if( wykonajRuch )
	{
		//FIELD komp = ( (gracz == TN_CIRCLE) ? FD_CIRCLE : FD_CROSS );
		if( /*(komp == si->Gracz()) &&*/ aktywny_gracz[ KOMPUTER ] && ai /*&& (gracz == TN_CIRCLE)*/ )
		{
			//int r = si->Ruch();
			int r = si->Ruch2();
			std::cout << "ii komputer wybrał pole " << r << std::endl;
			//gracz = ( gracz == TN_CIRCLE ? TN_CROSS : TN_CIRCLE );
			aktywny_gracz[ KOMPUTER ] = false;
			aktywny_gracz[ GRACZ_1 ] = true;
			if( game_over() )
				restart();
			draw();
		}
		
		for( int i = 0; i < ILOSC_POL; i++ )
		{
			
			if( aktywny_gracz[ GRACZ_1 ] || aktywny_gracz[ GRACZ_2 ] )
			{
				pln->Pole( i )->Operate();
				if( mouse_b & 1 )
				{
					pln->Pole( i )->LBMPressed( true );
					//std::cout << "ii Naciśnięto prawy przycisk myszy." << std::endl;
				}
				else
					pln->Pole( i )->LBMPressed( false );
					
				}
			
			if( pln->Pole( i )->Clicked() )
			{
				std::cout << "ii Pole " << i << " kliknięto." << std::endl;
				if( ! pln->Pole( i )->Busy() )
				{
					pln->Pole( i, ( gracz == TN_CIRCLE ? FD_CIRCLE : FD_CROSS ) );
					if( ai && (gracz == TN_CROSS) )
					{
						aktywny_gracz[ KOMPUTER ] = true;
						aktywny_gracz[ GRACZ_1 ] = false;
					}
					else
					if( ! ai )
						{
							aktywny_gracz[ GRACZ_1 ] = ! aktywny_gracz[ GRACZ_1 ];
							aktywny_gracz[ GRACZ_2 ] = ! aktywny_gracz[ GRACZ_2 ];
						}
					if( game_over() )
						restart();
				}
			}
		} // for
	}
	else draw();
}

GAME proste_menu()
{
	// czyscimy bufor
	clear_to_color( bufor, makecol(0, 0, 255) );
	
	// tytuł gry i wersja
	textout_ex( bufor, font, "allkik " WERSJA, 300, 50, makecol(255, 255, 255), -1 );
	
	// menu
	textout_ex( bufor, font, "F2  - Człowiek konta Człowiek", 150, (WIDTH / 3) - 30, makecol(255, 255, 255), -1 );
	textout_ex( bufor, font, "F3  - Człowiek konta Komputer", 150, (WIDTH / 3) - 20, makecol(255, 255, 255), -1 );
	//textout_ex( bufor, font, "F4  - Komputer konta Komputer", 150, (WIDTH / 3) - 10, makecol(255, 255, 255), -1 );
	textout_ex( bufor, font, "ESC - Wyjście", 150, (WIDTH / 3), makecol(255, 255, 255), -1 );
	
	// info
	textout_ex( bufor, font, "autor: Łukasz 'Pujan' Pelc", 280, 500, makecol(255, 255, 255), -1 );
	textout_ex( bufor, font, "Wyprodukowane w Polsce w 2010 r.", 255, 510, makecol(255, 255, 255), -1 );
	
	// kopiujemy bufor na ekran
	blit( bufor, screen, 0, 0, 0, 0, bufor->w, bufor->h );
	
	do
	{
		if( key[ KEY_F2 ] )
			return GM_HUM_HUM;
		else
			if( key[ KEY_F3 ] )
				return GM_HUM_COMP;
			/*else
				if( key[ KEY_F3 ] )
					return GM_COMP_COMP;*/
				else
					if( key[ KEY_ESC ] )
						return GM_EXIT;
	} while( 1 );
}

int main()
{
	init_game();
	
	while( ! wyjscie )
	{
		if( menu )
		{
			switch( proste_menu() )
			{
				case GM_HUM_HUM:
					std::cout << "ii Wybrano tryb 'Człowiek kontra Człowiek'" << std::endl;
					menu = false;
					ai = false;
					aktywny_gracz[ GRACZ_1 ] = true;
					aktywny_gracz[ GRACZ_2 ] = false;
					aktywny_gracz[ KOMPUTER ] = false;
					restart();
				break;
				case GM_HUM_COMP:
					std::cout << "ii Wybrano tryb 'Człowiek kontra Komputer'" << std::endl;
					ai = true;
					si = new kik::clSI( pln, FD_CIRCLE );
					menu = false;
					if( gracz == TN_CIRCLE )
					{
						aktywny_gracz[ KOMPUTER ] = true;
						aktywny_gracz[ GRACZ_1 ] = false;
						aktywny_gracz[ GRACZ_2 ] = false;
					}
					else
						{
							aktywny_gracz[ KOMPUTER ] = false;
							aktywny_gracz[ GRACZ_1 ] = true;
							aktywny_gracz[ GRACZ_2 ] = false;
						}
					restart();
				break;
				case GM_COMP_COMP:
					std::cout << "ii Wybrano tryb 'Komputer kontra Komputer'" << std::endl;
					restart();
				break;
				case GM_EXIT:
					wyjscie = true;
				break;
			}
		}
		else
		{
			while( ticks > 0 )
			{
				logic();
				ticks--;
			}
			
			draw();
		}
	}
	
	exit_game();
	std::cout << "Do następnego uruchomienia. :)" << std::endl;
	return 0;
}
END_OF_MAIN();
