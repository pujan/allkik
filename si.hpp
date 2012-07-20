//      si.hpp
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

#ifndef _SI_HPP_
#define _SI_HPP_

#include "plansza.hpp"
#include "game.h"

namespace kik
{
	class clSI
	{
	public:
		clSI( kik::clPlansza*, FIELD );
		virtual ~clSI();
		// metody udostepniajace
		FIELD Gracz() const
		{ return gracz; }
		void Gracz( FIELD g )
		{ gracz = g; }
		int Ruch();
		int Ruch2();
	protected:
		int MiniMax();
		bool Wygrana();
		bool Remis();
	private:
		kik::clPlansza *pln; // plansza gry
		FIELD gracz; // gracz, ktory jest sterowany przez SI
	};
}

#endif
