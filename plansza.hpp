//      plansza.hpp
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

#ifndef _PLANSZA_HPP_
#define _PLANSZA_HPP_

#include "game.h"
#include "area.hpp"

namespace kik
{
	const int linie[8][3] =
	{
		// linie poziome
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		// linie pionowe
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		// linie ukosne
		{0, 4, 8},
		{2, 4, 6}
	};
	
	class clPlansza
	{
	public:
		clPlansza( /*kik::clArea*/ );
		~clPlansza();
		kik::clArea *Pole( int );
		void Pole( int, FIELD );
	private:
		kik::clArea *plansza;
	};
}

#endif

