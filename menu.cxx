//      menu.cxx
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

#include "menu.hpp"
#include "game.h"
#include <iostream>

kik::clMenu::clMenu( int ilosc ) : activeElement( 0 ), numElem( ilosc )
{
	for( int i = 0; i < numElem; i++ )
	{
		elements[ i ] = create_bitmap( 100, 50 );
		if( elements[ i ] == NULL )
			error( "Nie moge utworzyc bitmap!" );
		
		clear_to_color( elements[ i ], makecol(0, 0, 0) );
		//tmp[ i ][ 1 ] = create_bitmap( 100, 50 );
		//clear_to_color( tmp[ i ][ 1 ], makecol(255, 255, 255) );
	}
	
	//elements = tmp;
	
	x = 10;
	y = 10;
	h = 50;
	w = 100;
}

kik::clMenu::~clMenu()
{
	for( int i = 0; i < numElem; i++ )
	{
		destroy_bitmap( elements[ i ] );
		//destroy_bitmap( elements[ i ][ 1 ] );
	}
}

void kik::clMenu::Init()
{
}

/*void kik::clMenu::Operate()
{
	if( collision_cursor(x, y, x + w, y + h) )
		mouseon = true;
	else
		mouseon = false;
	
	if( mouseon && touched && lbmPressed && ! mouse_b & 1 )
		clicked = true;
	else
		clicked = false;
	
	if( mouseon && mouse_b & 1 )
		touched = true;
	else
		touched = false;
}

void kik::clMenu::collision_cursor( int x1, int x2, int y1, int y2 )
{
	if( ((mouse_x >= x1) && (mouse_x <= x2)) && ((mouse_y >= y1) && (mouse_y <= y2)) )
		return true;
	
	return false;
}*/

void kik::clMenu::Draw( BITMAP *bufor )
{
	std::cout << "numElem: " << numElem << std::endl
	<< "x: " << x << std::endl << "y: " << y << std::endl
	<< "w: " << w << std::endl << "h: " << h << std::endl;
	
	
	for( int i = 0; i < numElem; i++ )
	{
		std::cout << "i = " << i << std::endl;
		//if( i == activeElement )
			//blit( elements[ i ], bufor, 0, 0, x, y+(i*50), w, h );
		//else
			//blit( elements[ i ][ 0 ], bufor, 0, 0, x, y, w, h );
	}
}

