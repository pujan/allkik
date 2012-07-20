//      plansza.cxx
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

#include "plansza.hpp"

kik::clPlansza::clPlansza( /*kik::clArea *area*/ )
{
	plansza = new kik::clArea[ ILOSC_POL ];
	
	kik::clArea area[] =
{
// pierwsza linia
kik::clArea( 90, 90, W_AREA, H_AREA ), 
kik::clArea( 90 + W_AREA + 5, 90, W_AREA, H_AREA ),
kik::clArea( 90 + (W_AREA * 2) + 10, 90, W_AREA, H_AREA ),
// druga linia
kik::clArea( 90, 90 + H_AREA + 5, W_AREA, H_AREA ),
kik::clArea( 90 + W_AREA + 5, 90 + H_AREA + 5, W_AREA, H_AREA ),
kik::clArea( 90 + (W_AREA * 2) + 10, 90 + H_AREA + 5, W_AREA, H_AREA ),
// trzecia linia
kik::clArea( 90, 90 + (H_AREA * 2) + 10, W_AREA, H_AREA ),
kik::clArea( 90 + W_AREA + 5, 90 + (H_AREA * 2) + 10, W_AREA, H_AREA ),
kik::clArea( 90 + (W_AREA * 2) + 10, 90 + (H_AREA * 2) + 10, W_AREA, H_AREA )
};
	
	for( int i = 0; i < ILOSC_POL; i++ )
		plansza[ i ] = area[ i ];
}

kik::clPlansza::~clPlansza()
{
	delete[] plansza;
}

kik::clArea *kik::clPlansza::Pole( int x )
{
	if( x >= ILOSC_POL )
		return NULL;
	
	return &plansza[ x ];
}

void kik::clPlansza::Pole( int x, FIELD pole )
{
	if( x >= ILOSC_POL )
		return;
	
	plansza[ x ].Element( pole );
	plansza[ x ].Busy( true );
}

