//      area.hpp
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

#ifndef _AREA_HPP_
#define _AREA_HPP_

#include "game.h"

namespace kik
{
	class clArea
	{
	public:
		clArea( int _x=10, int _y=10, int _w=10, int _h=10 ); // konstruktor
		virtual ~clArea();        // destruktor
		// metody udostepniajace
		int Width() const
		{ return w; }
		
		void Width( int );
		
		int Height() const
		{ return h; }
		
		void Height( int );
		
		int CordX() const
		{ return x; }
		
		void CordX( int );
		
		int CordY() const
		{ return y; }
		
		void CordY( int );
		
		bool Busy() const
		{ return busy; }
		
		void Busy( bool b )
		{ busy = b; }
		
		bool Clicked() const
		{ return clicked; }
		
		void Clicked( bool b )
		{ clicked = b; }
		
		bool LBMPressed() const
		{ return lbmPressed; }
		
		void LBMPressed( bool b )
		{ lbmPressed = b; }
		
		bool MouseOn() const
		{ return mouseon; }
		
		void MouseOn( bool b )
		{ mouseon = b; }
		
		bool Touched() const
		{ return touched; }
		
		void Touched( bool b )
		{ touched = b; }
		
		FIELD Element() const
		{ return element; }
		
		void Element( FIELD elem )
		{ element = elem; }
		
		// funkcje pozostale
		void Operate();
		bool collision_cursor( int, int, int, int );
	protected:
		bool busy; // jezeli zajety nie mozna kliknac; pole wybrane
		bool mouseon; // czy kursor myszy znajduje sie nad polem
		bool clicked; // czy zostalo pole klikniete (naciscnmiecie i zwolnienie przycisku)
		bool touched; // czy przycisk myszy jest przycisniety
		bool lbmPressed; // czy lewy przycisk myszy zostal nacisniety
		int x; // wspolrzedne umieszczenia
		int y; // ...na ekranie
		int w; // szerokosc pola w pikselach
		int h; // wysokosc pola w pikselach
	private:
		FIELD element; // jaki znak zawiera pole
	};
}

#endif

