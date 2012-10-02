/**
The Takeaway game.

@author Sol Boucher <slb1566@rit.edu>
*/
#include "Solver.h"
#include "TakeawayState.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

int main( int argc, char** argv )
{
	if( argc<2 || argc>3 || ( argc==3 && strcmp( argv[2], "play" )!=0 ) ) //bad arguments
	{
		cerr<<"USAGE: takeaway [play] num_pennies"<<endl;
		
		return 1; //I have failed, Master
	}
	else if( argc==2 ) //advisory mode
	{
		int startingNumber=atoi( argv[1] );
		
		if( startingNumber<=0 )
		{
			cerr<<"FATAL: argument num_pennies must be a natural number"<<endl;
			
			return 1; //certain death
		}
		else
		{
			TakeawayState starting( atoi( argv[1] ) );
			Solver< TakeawayState > game( starting );
			
			cout<<"Take "<<TakeawayState::diff( starting, game.nextBestState() )<<" pennies ";
			cout<<"to leave "<<game.getCurrentState().getPileSize()<<" for the opponent."<<endl;
		}
	}
}
