/**
The Takeaway game.

@author Sol Boucher <slb1566@rit.edu>
*/
#include "Solver.h"
#include "TakeawayState.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

int main( int argc, char** argv )
{
	//check argument count and switches
	if( argc<2 || argc>3 || ( argc==3 && strcmp( argv[1], "play" )!=0 ) ) //bad arguments
	{
		cerr<<"USAGE: takeaway [play] num_pennies"<<endl;
		
		return 1; //I have failed, Master
	}
	
	int startingNumber=atoi( argv[argc-1] );
	
	//check initial pile count
	if( startingNumber<0 )
	{
		cerr<<"FATAL: argument num_pennies must be a natural number"<<endl;
		
		return 1; //certain death
	}
	
	//all systems go
	if( argc==2 ) //advisory mode
	{
		TakeawayState starting( startingNumber ); //our turn
		Solver< TakeawayState > game( starting );
		
		if( starting.gameOver() )
			cout<<"There are no pennies; you have already won."<<endl;
		else
		{
			cout<<"Take "<<TakeawayState::diff( starting, game.nextBestState() )<<" pennies ";
			cout<<"to leave "<<game.getCurrentState().getPileSize()<<" for the opponent."<<endl;
		}
	}
	else //argc==3 ... interactive mode
	{
		TakeawayState current( startingNumber, false ); //human's turn
		Solver< TakeawayState > game( current );
		
		while( !game.getCurrentState().gameOver() )
		{
			cout<<game.getCurrentState().str()<<endl;
			
			if( game.getCurrentState().computersTurn() )
			{
				current=game.getCurrentState();
				cout<<"Computer: takes "<<TakeawayState::diff( current, game.nextBestState() )<<" pennies"<<endl;
			}
			else //player's turn
			{
				int response;
				
				do
				{
					cout<<"You take how many ["<<TakeawayState::MIN_TAKEN<<','<</*min( TakeawayState::MAX_TAKEN, game.getCurrentState().getPileSize() )*/( game.getCurrentState().getPileSize()>=TakeawayState::MAX_TAKEN ? TakeawayState::MAX_TAKEN : game.getCurrentState().getPileSize() )<<"] ? ";
					cout.flush();
					cin>>response;
				}
				while( !game.supplyNextState( TakeawayState( game.getCurrentState(), response ) ) ); //tried and failed to make the given move
				
				cout<<"Human: took "<<response<<" pennies"<<endl;
			}
		}
		
		cout<<"No pennies remain."<<endl;
		cout<<"=================="<<endl;
		cout<<( game.getCurrentState().scoreGame()==TakeawayState::VICTORY ? "Computer wins" : "You win" )<<"!  (Your score was "<<-game.getCurrentState().scoreGame()<<".)"<<endl;
	}
}
