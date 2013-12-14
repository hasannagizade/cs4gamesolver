/*
 * Copyright (C) 2012 Sol Boucher and Kyle Savarese
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @author Sol Boucher <slb1566@rit.edu> */
#include "Connect3Helper.h"
#include "Connect3State.h"
#include <iostream>
#include <istream>
#include <vector>
using namespace std;

/** @brief Helper function gone method */
bool Connect3Helper::decodeBoard( istream& save, vector< vector< char > >&
	use, int& height )
{
	int width;
	
	save>>width;
	save>>height;
	
	use.clear();
	for( int column=0; column<width; ++column )
		use.push_back( vector< char >( height, 0 ) );
	
	for( int level=height-1; level>=0; --level )
		for( vector< vector< char > >::iterator col=use.begin();
			col!=use.end(); ++col )
		{
			save>>( *col )[level];
			if( ( *col )[level]==Connect3State::PLACEHOLDER )
				col->pop_back(); //turn jagged
			else if( !Connect3State::validChar( ( *col )[level]
				) )
			{
				cerr<<"FATAL: Board contains invalid marker "
					<<( *col )[level]<<endl;
				
				return false;
			}
		}
	
	if( save.fail() )
	{
		cerr<<"FATAL: Syntax error in board description"<<endl;
		
		return false;
	}
	
	return true;
}
