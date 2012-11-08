/** @author Sol Boucher <slb1566@rit.edu> */
#include "TakeawayState.h"
#include <iostream>
#include <sstream>
using namespace std;

/** @brief What might happen next? */
void TakeawayState::successors( vector< TakeawayState >& possibilities ) const
{
	#ifdef DEBUG
		cout<<"Success calculating successors for "<<str()<<'\n';
	#endif
	
	for( int take=MIN_TAKEN; take<=MAX_TAKEN && take<=pileSize; ++take )
	{
		possibilities.push_back( TakeawayState( pileSize-take,
			!ourTurn ) );
		
		#ifdef DEBUG
			cout<<'\t'<<possibilities.back().str()<<'\n';
		#endif
	}
	
	#ifdef DEBUG
		cout.flush();
	#endif
}

/** @brief Textualizes */
string TakeawayState::str() const
{
	stringstream assembler;
	
	assembler<<"It is the "<<( ourTurn ? "computer" : "human" )<<
		"'s turn and "<<pileSize<<" pennies remain.";
	assembler.flush();
	
	return assembler.str();
}
