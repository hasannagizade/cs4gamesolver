/**
The Kayles game.

@author Sol Boucher <slb1566@rit.edu>
*/
#include "Solver.h"
#include "KaylesState.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

int main( int argc, char** argv )
{
	//check argument count and switches
	if( argc<2 || ( !isdigit( argv[1][0] ) && strcmp( argv[1], "play" )!=0 ) )
		//bad arguments
	{
		cerr<<"USAGE: kayles [play] num_pins_1 num_pins_2 ..."<<endl;
		
		return 1; //I have failed, Master
	}
	
	vector< int > world;
	
	//collect line counts
	for( unsigned int line=isdigit( argv[1][0] ) ? 1 : 2; line<(unsigned)argc;
		++line)
		world.push_back( atoi( argv[line] ) );
	
	//all systems go
	if( isdigit( argv[1][0] ) ) //advisory mode
	{
		KaylesState starting( world ); //our turn
		Solver< KaylesState > game( starting );
		
		if( starting.gameOver() )
			cout<<"There are no pins; you have already lost."<<endl;
		else
		{
			KaylesState outcome=game.nextBestState();
			pair< int, int > advice=KaylesState::diff( starting, outcome );
			
			cout<<"Target "<<advice.second<<" pins from line "<<advice.first<<
				endl;
		}
	}
	else //interactive mode
	{
		KaylesState current( world, false ); //human's turn
		Solver< KaylesState > game( current );
		
		while( !game.getCurrentState().gameOver() )
		{
			cout<<game.getCurrentState().str()<<endl;
			
			if( game.getCurrentState().computersTurn() )
			{
				current=game.getCurrentState();
				pair< int, int > delta=KaylesState::diff( current, game.
					nextBestState() );
				
				cout<<"Computer: downs "<<delta.second<<" pins from line "<<
					delta.first<<endl;
			}
			else //player's turn
			{
				int line, greed;
				
				do
				{
					cout<<"For which nonempty line do you bowl? [0,"<<game.
						getCurrentState().groupsOfPins()<<") ? ";
					cout.flush();
					cin>>line;
					cout<<"You take how many ["<<KaylesState::MIN_TAKEN<<','<<
						( game.getCurrentState().pinsInGroup( line )>=
						KaylesState::MAX_TAKEN ? KaylesState::MAX_TAKEN : game
							.getCurrentState().pinsInGroup( line ) )<<"] ? ";
					cout.flush();
					cin>>greed;
				}
				while( !game.supplyNextState( KaylesState( game.
					getCurrentState(), line, greed ) ) ); //tried and failed t
					//o make the given move
				
				cout<<"Human: downed "<<greed<<" pins from line "<<line<<endl;
			}
		}
		
		cout<<"No pins remain."<<endl;
		cout<<"=================="<<endl;
		cout<<( game.getCurrentState().scoreGame()==KaylesState::VICTORY ?
			"Computer wins" : "You win" )<<"!  (Your score was "<<-game.
				getCurrentState().scoreGame()<<".)"<<endl;
	}
}
