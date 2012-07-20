//      menu.hpp
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

#ifndef _MENU_H_
#define _MENU_H_

#include <allegro.h>
#include "area.hpp"

namespace kik
{
	class clMenu : public clArea
	{
	public:
		clMenu( int ilosc = 1 ); // konstruktor
		~clMenu(); // destruktor
		// sprawdza czy został naciśnięty przycisk myszy
		//void Operate();
		// rysuje menu
		void Draw( BITMAP * );
	//protected:
		//void collision_cursor( int, int, int, int );
		// numer aktywnego elementu menu
		int activeElement;
		// wskaźnik na tablicę bitmap
		BITMAP *elements[];
		// ilość elementów w menu
		int numElem;
	private:
		void Init();
	};
}

#endif

