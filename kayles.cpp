/**
The Kayles game.

@author Sol Boucher <slb1566@rit.edu>
@author Kyle Savarese <kms7341@rit.edu>
*/
#include "Solver.h"
#include "KaylesState.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <utility>
#include <sstream>
using namespace std;

int main( int argc, char** argv )
{
	const int MIN_ARGS = 2;
	const char* PLAY = "play";
	//check argument count and switches
	if( argc<MIN_ARGS || ( !isdigit( argv[1][0] ) && strcmp( argv[1], PLAY )!=0 ) )
		//bad arguments
	{
		cerr<<"USAGE: kayles [play] num_pins_1 num_pins_2 ..."<<endl;
		
		return 1; //I have failed, Master
	}
	//collect line counts
	string s = "";
	for ( int i = 1; i < argc; i++ ) {
		s += argv[i];
		s += " ";
	}
	stringstream in( s, ios_base::in );
	vector< int > world;
	int data;
	if( isdigit( argv[1][0] ) ) { //Not in play mode
		while ( in >> data ) {
			world.push_back( data );
		}
	}
	else { //Play mode
		string played;
		unsigned int i;
		for ( i = 0; !isdigit(s[i]); i++ ) {
			played += s[i];
		}
		string num;
		while ( i < s.size() ) {
			num += s[i];
			i++;
		}
		cout << num << endl;
		stringstream in2( num, ios_base::in );
		while( in2 >> data ) {
			world.push_back( data );
		}	
	}

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
					if ( 0 > line || line >= game.getCurrentState().groupsOfPins() )
					{
						line = -1;
						continue;
					}
					cout<<"You take how many ["<<KaylesState::MIN_TAKEN<<','<<
						( game.getCurrentState().pinsInGroup( line )>=
						KaylesState::MAX_TAKEN ? KaylesState::MAX_TAKEN : game
							.getCurrentState().pinsInGroup( line ) )<<"] ? ";
					cout.flush();
					cin>>greed;
				}
				while( line == -1 || !game.supplyNextState( KaylesState( game.
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
