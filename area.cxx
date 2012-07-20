//      area.cxx
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

#include "area.hpp"

kik::clArea::clArea( int _x, int _y, int _w, int _h ) :
	busy( false ), mouseon( false ), clicked( false ), touched( false ),
	lbmPressed( false ), x( _x ), y( _y ), w( _w ), h( _h ), element( FD_EMPTY )
{}

kik::clArea::~clArea()
{}

void kik::clArea::Width( int width )
{
	// sprawdzenie i przypisanie szerokosci obiektu
	if( width <= WIDTH )
		w = width;
	else
		if( (width == 0) || (width > WIDTH) )
			w = WIDTH;
}

void kik::clArea::Height( int height )
{
	// sprawdzenie i przypisanie wysokosci obiektu
	if( height <= HEIGHT )
		h = height;
	else
		if( (height == 0) || (height > HEIGHT) )
			h = HEIGHT;
}

void kik::clArea::CordX( int _x )
{
	if( _x < WIDTH )
		x = _x;
	else
		if( _x > WIDTH )
			x = _x - w - WIDTH;
}

void kik::clArea::CordY( int _y )
{
	if( y < HEIGHT )
		y = _y;
	else
		if( _y > HEIGHT )
			y = _y - h - HEIGHT;
}

void kik::clArea::Operate()
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

bool kik::clArea::collision_cursor( int x1, int y1, int x2, int y2 )
{
	if( ((mouse_x >= x1) && (mouse_x <= x2)) && ((mouse_y >= y1) && (mouse_y <= y2)) )
		return true;
	
	return false;
}

