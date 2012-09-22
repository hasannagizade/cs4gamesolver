/**
Represents the Takeaway game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
*/
#ifndef TAKEAWAYSTATE_H
#define TAKEAWAYSTATE_H

#include <vector>
using namespace std;

class TakeawayState
{
	private: //gameplay constants
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
		int pileSize;
		bool ourTurn;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@param thingsInPile the number of things in the pile
		@param weAreUp whether or not the "good guy" is up
		*/
		explicit TakeawayState( int thingsInPile, bool weAreUp=true );
		
		/**
		Destroys the game state.
		*/
		~TakeawayState();
		
		/**
		Judges whether the game is over.
		@return whether there are no objects left in the pile
		*/
		bool gameOver( void ) const;
		
		/**
		Devines the match score, which is only meaningful if the game is over.
		@return the score: <tt>Score::VICTORY</tt> for our victory, <tt>Score::LOSS</tt> for opponent's victory, or <tt>Score::TIE</tt> for an unterminated game
		*/
		Score scoreGame( void ) const;
		
		/**
		Determines whether we are trying to win.
		@return whether the "good guy" is taking his turn
		*/
		bool wantToWin( void ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		const vector< TakeawayState > successors( void ) const;
};

#endif
