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
