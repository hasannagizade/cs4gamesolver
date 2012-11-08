//Version history in git-log.txt
#ifndef TAKEAWAYSTATE_H
#define TAKEAWAYSTATE_H

#include <cassert>
#include <string>
#include <vector>

/**
Represents the Takeaway game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
*/
class TakeawayState
{
	public: //gameplay constants
		/** The maximum number of items a player may take per turn. */
		static const int MAX_TAKEN=3;
		
		/** The minimum number of items permissible to take. */
		static const int MIN_TAKEN=1;
	
	public: //descriptions
		/** The possible "scores" resulting from a complete match */
		enum Score
		{
			LOSS=-1,
			TIE=0, //also known as "incomplete"
			VICTORY=1
		};
	
	private: //state
		/** The number of objects left in the pile */
		int pileSize;
		
		/** Whether the computer player is up */
		bool ourTurn;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@param thingsInPile the number of things in the pile
		@param weAreUp whether or not the "good guy" is up
		*/
		inline explicit TakeawayState( int thingsInPile=0, bool weAreUp=true
			);
		
		/**
		Creates the move resulting from taking away a specified
			number of objects.  This makes the most sense when a
			positive number of things are taken, and preferably a
			legal number; however, this is not required.
		@post The new state reflects the fact that it is now the
			opposite player's turn.
		@param baseState the starting state
		@param stolen the number of objects that have been taken
		*/
		inline TakeawayState( const TakeawayState& baseState, int stolen );
		
		/**
		Destroys the game state.
		*/
		inline ~TakeawayState( void );
		
		/**
		Judges whether the game is over.
		@return whether there are no objects left in the pile
		*/
		inline bool gameOver( void ) const;
		
		/**
		Devines the match score, which is only meaningful if the game
			is over.
		@return the score: <tt>Score::VICTORY</tt> for our victory,
			<tt>Score::LOSS</tt> for opponent's victory, or
			<tt>Score::TIE</tt> for an unterminated game
		*/
		inline Score scoreGame( void ) const;
		
		/**
		Determines whether it is our turn.
		@return whether the "good guy" is taking his turn
		*/
		inline bool computersTurn( void ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		void successors( std::vector< TakeawayState >& result ) const;
		
		/**
		Retrieves the pile size.
		@return the pile size
		*/
		inline int getPileSize( void ) const;
		
		/**
		Produces a synopsis of this <tt>State</tt>'s particulars.
		@return the <tt>string</tt> representation
		*/
		std::string str( void ) const;
		
		/**
		Hashes the <tt>State</tt>.
		@post The result is nonnegative.
		@return a hash code
		*/
		inline int hash( void ) const;
		
		/**
		Checks identity
		@param another comparable <tt>State</tt>
		@return whether the turns and <tt>pileSize</tt>s are the same
		*/
		inline bool operator==( const TakeawayState& another ) const;
		
		/**
		Determines whether two game states are subsequent.
		@param first the original state
		@param next the new state
		@return whether they would appear in a game in sequence
		*/
		inline static bool areSubsequent( const TakeawayState& first, const
			TakeawayState& next );
		
		/**
		Finds the move made to get between two game states.
		@pre The states must be exactly one move apart.
		@param first the original state
		@param next the new state
		@return the number of stones taken, or <tt>0</tt> if the
			question was invalid
		*/
		inline static int diff( const TakeawayState& first, const
			TakeawayState& next );
};

/** @brief Constructor */
TakeawayState::TakeawayState( int thingsInPile, bool weAreUp ):
	pileSize( thingsInPile ), ourTurn( weAreUp ) {}

/** @brief Advancing constructor */
TakeawayState::TakeawayState( const TakeawayState& baseState, int stolen ):
	pileSize( baseState.pileSize-stolen ), ourTurn( !baseState.ourTurn )
	{}

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

/** @brief How many remain? */
int TakeawayState::getPileSize() const
{
	return pileSize;
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
	return this->ourTurn==another.ourTurn &&
		this->pileSize==another.pileSize;
}

/** @brief Are these subsequent? */
bool TakeawayState::areSubsequent( const TakeawayState& first, const
	TakeawayState& next )
{
	int taken=first.pileSize-next.pileSize;
	
	return first.ourTurn!=next.ourTurn && taken>=MIN_TAKEN &&
		taken<=MAX_TAKEN && next.pileSize>=0;
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

#endif
