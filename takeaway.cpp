/**
The Takeaway game.

@author Sol Boucher <slb1566@rit.edu>
@author Kyle Savarese <kms7341@rit.edu>
*/
#include "Solver.h"
#include "TakeawayState.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

int main( int argc, char** argv )
{
	const char* PLAY = "play";
	const int MIN_ARGS = 2;
	const int PLAY_ARGS = 3;
	const int MIN_PENNIES = 0;
	//check argument count and switches
	if( argc<MIN_ARGS || argc>PLAY_ARGS || ( argc==PLAY_ARGS &&
		strcmp( argv[1], PLAY )!=0 ) ) //ba
		//d arguments
	{
		cerr<<"USAGE: takeaway [play] num_pennies"<<endl;
		
		return 1; //I have failed, Master
	}
	string s = "";
	for ( int i = 1; i < argc; i++ ) {
		s+= argv[i];
	}
	stringstream in( s, ios_base::in);
	int startingNumber;
	if ( argc==MIN_ARGS ) { //Not in play mode
		if ( !( in >> startingNumber ) ) {
			cerr << argv[1] << " is an invalid number of pennies."
				<< endl;
			return 0;
		}
	}
	else { //Play mode
		string played;
		unsigned int i;
		for ( i = 0; !isdigit(s[i]) && s[i] != '-'; i++ ) {
			played += s[i];
		}
		string num;
		while ( i < s.size() ) {
			num += s[i];
			i++;
		}
		stringstream in2( num, ios_base::in);
		if ( !( in2 >> startingNumber) ) {
			cerr << argv[2] << " is an invalid number of pennies."
				<< endl;
			return 0;
		}
	}
	
	//check initial pile count
	if( startingNumber< MIN_PENNIES )
	{
		cerr<<"FATAL: argument num_pennies must be a natural number"
			<<endl;
		
		return 1; //certain death
	}
	
	//all systems go
	if( argc==MIN_ARGS ) //advisory mode
	{
		TakeawayState starting( startingNumber ); //our turn
		Solver< TakeawayState > game( starting );
		
		if( starting.gameOver() )
			cout<<"There are no pennies; you have already won."
				<<endl;
		else
		{
			cout<<"Take "<<TakeawayState::diff( starting,
				game.nextBestState() )<<" pennies ";
			cout<<"to leave "<<game.getCurrentState().
				getPileSize()<<" for the opponent."<<endl;
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
				cout<<"Computer: takes "<<TakeawayState::diff(
					current, game.nextBestState() )
					<<" pennies"<<endl;
			}
			else //player's turn
			{
				int response;
				
				do
				{
					cout<<"You take how many ["
						<<TakeawayState::MIN_TAKEN
						<<','<<( game.getCurrentState
						().getPileSize()>=
						TakeawayState::MAX_TAKEN ?
						TakeawayState::MAX_TAKEN :
						game.getCurrentState().
						getPileSize() )<<"] ? ";
					cout.flush();
					cin>>response;
				}
				while( !game.supplyNextState( TakeawayState
					( game.getCurrentState(), response ) )
					); //tried and failed to make the
					//given move
				
				cout<<"Human: took "<<response<<" pennies"
					<<endl;
			}
		}
		
		cout<<"No pennies remain."<<endl;
		cout<<"=================="<<endl;
		cout<<( game.getCurrentState().scoreGame()==
			TakeawayState::VICTORY ? "Computer wins" : "You win" )
			<<"!  (Your score was "<<-game.getCurrentState().
			scoreGame()<<".)"<<endl;
	}
}
