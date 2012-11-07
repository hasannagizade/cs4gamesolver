/**
The Connect-3 game.

@author Sol Boucher <slb1566@rit.edu>
*/
#include "Solver.h"
#include "Connect3State.h"
#include "Connect3Helper.h"
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

int main( int argc, char** argv )
{
	const int MIN_ARGS=2;
	const int PLAY_ARGS=3;
	const char* STDIN="-";
	const char* PLAY="play";
	const int SIG_INDEX=1; //significant index
	const int FAILURE=1; //return code
	
	if( argc<MIN_ARGS || argc>PLAY_ARGS || ( argc==PLAY_ARGS &&
		strcmp( argv[SIG_INDEX], PLAY )!=0 ) )
	{
		cerr<<"USAGE: connect3 [play] -"<<endl;
		
		return FAILURE; //I have failed, Master
	}
	else //valid argument syntax
	{
		vector< vector< char > > board;
		int height;
		
		if( strcmp( argv[argc-1], STDIN )==0 ) //read from stdin
		{
			if( !Connect3Helper::decodeBoard( cin, board, height ))
				return FAILURE;
		}
		else //read from file
		{
			ifstream file( argv[argc-1] );
			
			if( file.fail() )
			{
				cerr<<"FATAL: Unable to open file "<<argv[argc-1]<<endl;
				
				return FAILURE;
			}
			
			if( !Connect3Helper::decodeBoard( file, board, height ))
			{
				file.close();
				
				return FAILURE;
			}
			
			file.close();
		}
		
		if( argc==MIN_ARGS ) //advice hotline
		{
			Connect3State config=Connect3State( board.size(), height, board );
			Solver< Connect3State > game( config );
			
			cout<<config.str()<<endl;
			if( config.gameOver() )
				cout<<"You have no move to make; you have already "
					<<( config.scoreGame()==Connect3State::VICTORY ?
					"won" : "lost" )<<'.'<<endl;
			else
				cout<<"Place a piece in column "<<Connect3State::diff( config,
					game.nextBestState() )<<endl;
		}
		else //interact
		{
			Connect3State current( board.size(), height, board, false );
				 //human's turn
			Solver< Connect3State > game( current );
			
			while( !game.getCurrentState().gameOver() )
			{
				cout<<game.getCurrentState().str()<<endl;
				
				if( game.getCurrentState().computersTurn() )
				{
					current=game.getCurrentState();
					cout<<"Computer: augments column "<<Connect3State::diff
						( current, game.nextBestState() )<<endl;
				}
				else //player's turn
				{
					int target;
					vector< int > nextState;	
					do
					{
						nextState.empty();
						cout<<"To which incomplete column do you add? [0,"
							<<game.getCurrentState().COLUMNS<<") ? ";
						cout.flush();
						cin>>target;
					}
					while( target<0 ||
						unsigned( target )>=game.getCurrentState().COLUMNS ||
						!game.getCurrentState().hasSpaceAt( target ) ||
						!game.supplyNextState( Connect3State
							( game.getCurrentState(), target ) ) ); //tried
								//and failed to make the given move
					
					cout<<"Human: augmented column "<<target<<endl;
				}
			}
			
			cout<<endl;
			cout<<"Game over."<<endl;
			cout<<"=================="<<endl;
			cout<<( game.getCurrentState().scoreGame()==Connect3State::VICTORY
				? "Computer wins" :
				( game.getCurrentState().scoreGame()==Connect3State::LOSS ?
				"You win" : "You tie" ) )<<"!  (Your score was "<<-game.
				getCurrentState().scoreGame()<<".)"<<endl;
		}
	}
}
