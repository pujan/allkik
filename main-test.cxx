#include <iostream>
#include "game.h"
#include "plansza.hpp"
#include "menu.hpp"

using namespace std;
using namespace kik;

bool wyjscie;
BITMAP *bufor;
clMenu *menu;

int main()
{
	init_game();
	
	while( ! wyjscie )
	{
		if( key[ KEY_ESC ] )
			wyjscie = true;
		
		//draw();
	}
	
	exit_game();
	return 0;
}

void init_game()
{
	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth( 24 );
	set_gfx_mode( GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0 );
	clear_to_color( screen, makecol(0, 0, 255) );
	wyjscie = false;
	bufor = create_bitmap( WIDTH, HEIGHT );
	if( bufor == NULL )
		error( "Nie moge utworzyc buforu." );
	//menu = new clMenu( 3 );
}

void exit_game()
{
	allegro_exit();
	delete menu;
}

void draw()
{
	clear_to_color( bufor, makecol(172, 172, 172) );
	blit( bufor, screen, 0, 0, 0, 0, bufor->w, bufor->h );
}

void error( const char *msg )
{
	set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
	allegro_message( msg );
	allegro_exit();
}
