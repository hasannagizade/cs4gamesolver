/** @author Sol Boucher <slb1566@rit.edu> */
#include "TakeawayState.h"
#include <cassert>
#include <iostream>
#include <sstream>
using namespace std;

/** @brief Constructor */
TakeawayState::TakeawayState( int thingsInPile, bool weAreUp ):
	pileSize( thingsInPile ), ourTurn( weAreUp ) {}

/** @brief Advancing constructor */
TakeawayState::TakeawayState( const TakeawayState& baseState, int stolen ):
	pileSize( baseState.pileSize-stolen ), ourTurn( !baseState.ourTurn ) {}

/** @brief Destructor */
TakeawayState::~TakeawayState() {}

/** @brief Are we out of objects? */
bool TakeawayState::gameOver() const
{
	return pileSize==0;
}

/** @brief Who won? */
TakeawayState::Score TakeawayState::scoreGame() const
{
	if( gameOver() )
		if( ourTurn ) return VICTORY;
		else /*!ourTurn*/ return LOSS;
	else /*!gameOver()*/ return TIE;
}

/** @brief Is it our turn? */
bool TakeawayState::computersTurn() const
{
	return ourTurn;
}

/** @brief What might happen next? */
const vector< TakeawayState > TakeawayState::successors() const
{
	#ifdef DEBUG
		cout<<"Success calculating successors for "<<str()<<'\n';
	#endif
	
	vector< TakeawayState > possibilities;
	
	for( int take=MIN_TAKEN; take<=MAX_TAKEN && take<=pileSize; ++take )
	{
		possibilities.push_back( TakeawayState( pileSize-take, !ourTurn ) );
		
		#ifdef DEBUG
			cout<<'\t'<<possibilities.back().str()<<'\n';
		#endif
	}
	
	#ifdef DEBUG
		cout.flush();
	#endif
	
	return possibilities;
}

/** @brief How many remain? */
int TakeawayState::getPileSize() const
{
	return pileSize;
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

/** @brief Hashing */
int TakeawayState::hash() const
{
	int res=( ( ourTurn ? 1 : 0 )<<1 )+pileSize;
	assert( res>=0 );
	return res;
}

/** @brief Same state? */
bool TakeawayState::operator==( const TakeawayState& another ) const
{
	return this->ourTurn==another.ourTurn && this->pileSize==another.pileSize;
}

/** @brief Are these subsequent? */
bool TakeawayState::areSubsequent( const TakeawayState& first, const
	TakeawayState& next )
{
	int taken=first.pileSize-next.pileSize;
	
	return first.ourTurn!=next.ourTurn && taken>=MIN_TAKEN && taken<=MAX_TAKEN
		&& next.pileSize>=0;
}

/** @brief What just happened? */
int TakeawayState::diff( const TakeawayState& first, const TakeawayState& next
	)
{
	#ifdef DEBUG
		cout<<"Diffing "<<first.str()<<" and "<<next.str()<<endl;
	#endif
	
	bool subsequentStates=areSubsequent( first, next );
	
	assert( subsequentStates );
	if( subsequentStates )
		return first.pileSize-next.pileSize;
	else //!areSubsequent( first, next )
		return 0;
}
