/**
The Crossout game.

@author Sol Boucher <slb1566@rit.edu>
*/
#include "Solver.h"
#include "CrossoutState.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

int main( int argc, char** argv )
{
	const char* PLAY = "play";
	const int MIN_ARGS = 3;
	const int PLAY_ARGS = 4;
	const int SIG_INDEX = 1; //the first argument we care about
	const int WHICH_MAX = 0; //where to find board size after parsing
	const int WHICH_SUM = 1; //where to find max sum after parsing
	const int FAILURE = 1;
	
	//check argument count and switches
	if( argc<MIN_ARGS || argc>PLAY_ARGS || ( argc==PLAY_ARGS &&
		strcmp( argv[SIG_INDEX], PLAY )!=0 ) ) //ba
		//d arguments
	{
		cerr<<"USAGE: crossout [play] max_num max_sum"<<endl;
		
		return FAILURE; //I have failed, Master
	}
	
	int index=SIG_INDEX;
	if( argc>MIN_ARGS ) ++index; //play mode
	string s = "";
	for ( int i = index; i < argc; ++i ) {
		s+= argv[i];
		s+=' ';
	}
	
	stringstream in( s, ios_base::in);
	vector< int > descriptors;
	for( ; index<PLAY_ARGS; ++index )
	{
		int data;
		
		while( in>>data )
		{
			descriptors.push_back( data );
			if( data<=0 )
			{
				cerr<<data<<" is not a valid argument."<<endl;
				return FAILURE;
			}
		}
		if( !in.eof() )
		{
			cerr<<in.get()<<" argument unexpected."<<endl;
			return FAILURE;
		}
	}
	
	//all systems go
	if( argc==MIN_ARGS ) //advisory mode
	{
		CrossoutState starting( descriptors[WHICH_SUM],
			descriptors[WHICH_MAX] ); //our turn
		Solver< CrossoutState > game( starting );
		
		if( starting.gameOver() )
			cout<<"There is nothing you can cross out; you have "
				<<"already won."<<endl;
		else
		{
			vector< int > advice=CrossoutState::diff( starting,
				game.nextBestState() );
			cout<<"Cross out:";
			for( vector< int >::iterator piece=advice.begin();
				piece!=advice.end(); ++piece )
				cout<<' '<<*piece;
			cout<<endl;
		}
	}
	else //argc==3 ... interactive mode
	{
		CrossoutState current( descriptors[WHICH_SUM],
			descriptors[WHICH_MAX], false ); //human's turn
		Solver< CrossoutState > game( current );
		
		while( !game.getCurrentState().gameOver() )
		{
			cout<<game.getCurrentState().str()<<endl;
			
			if( game.getCurrentState().computersTurn() )
			{
				current=game.getCurrentState();
				cout<<"Computer: crosses";
				vector< int > action=CrossoutState::diff(
					current, game.nextBestState() );
				for( vector< int >::iterator
					piece=action.begin();
					piece!=action.end(); ++piece )
					cout<<' '<<*piece;
				cout<<endl;
			}
			else //player's turn
			{
				int response, optional;
				
				do
				{
					optional=0; //the second chioce, if
						//opted
					
					cout<<"First uncrossed number you "
						<<"remove ? ";
					cout.flush();
					cin>>response;
					cin.get(); //clear the newline
					cout<<"Second number (enter for none)"
						<<" ? ";
					cout.flush();
					if( cin.peek()!='\n' ) cin>>optional;
				}
				while( response<=0 ||
					response>game.getCurrentState().
					traySize() || optional<0 ||
					optional==response || optional>game.
					getCurrentState().traySize() ||
					!game.supplyNextState( CrossoutState
					( game.getCurrentState(), response,
					optional ) ) ); //tried and failed to
					//make the given move
				
				stringstream nicelyFormatted;
				nicelyFormatted<<' '<<optional;
				cout<<"Human: crosses "<<response<<( optional
					? nicelyFormatted.str() : "" )<<endl;
			}
		}
		
		cout<<endl<<"Nothing more may be crossed out."<<endl;
		cout<<"=================="<<endl;
		cout<<( game.getCurrentState().scoreGame()==
			CrossoutState::VICTORY ? "Computer wins" : "You win" )
			<<"!  (Your score was "<<-game.getCurrentState().
			scoreGame()<<".)"<<endl;
	}
}
