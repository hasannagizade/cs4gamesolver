/** @author Sol Boucher <slb1566@rit.edu> */
#include "TakeawayState.h"

/** @brief Constructor */
TakeawayState::TakeawayState( int thingsInPile, bool weAreUp ):
	pileSize(thingsInPile), ourTurn(weAreUp) {}

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
	if(gameOver())
		if(ourTurn) return LOSS;
		else /*!ourTurn*/ return VICTORY;
	else /*!gameOver()*/ return TIE;
}

/** @brief Is it our turn? */
bool TakeawayState::wantToWin() const
{
	return ourTurn;
}

/** @brief What might happen next? */
const vector< TakeawayState > TakeawayState::successors() const
{
	vector< TakeawayState > possibilities;
	
	for(int take=1; take<=MAX_TAKEN; take++)
		possibilities.push_back( TakeawayState( pileSize+1, !ourTurn ) );
	
	return possibilities;
}

/** @brief Are these subsequent? */
bool TakeawayState::areSubsequent( const TakeawayState& first, const TakeawayState& next )
{
	int taken=next.pileSize-first.pileSize;
	
	return first.ourTurn!=next.ourTurn && taken>=MIN_TAKEN && taken<=MAX_TAKEN;
}

/** @brief What just happened? */
int TakeawayState::diff( const TakeawayState& first, const TakeawayState& next )
{
	assert( areSubsequent( first, next ) );
	if( areSubsequent( first, next ) )
		return next.pileSize-first.pileSize;
	else //!areSubsequent( first, next )
		return 0;
}
