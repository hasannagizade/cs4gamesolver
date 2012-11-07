#ifndef CROSSOUTSTATE_H
#define CROSSOUTSTATE_H

#include <string>
#include <vector>

/**
Represents the Crossout game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
*/
class CrossoutState
{
	public: //gameplay constants
		/** The maximum number of values a player may snatch per turn.
			*/
		static const unsigned int MAX_TAKEN=2; //changing requires code change
		
		/** The minimum number of numbers permissible to eradicate. */
		static const unsigned int MIN_TAKEN=1; //changing requires code change
		
		/** The maximum sum of the stolen values */
		const unsigned int MAX_SUM;
	
	public: //descriptions
		/** The possible "scores" resulting from a complete match */
		enum Score
		{
			LOSS=-1,
			TIE=0, //also known as "incomplete"
			VICTORY=1
		};
	
	private: //state
		/** Position n-1 tells whether n is still uncrossed */
		std::vector< bool > tray;
		
		/** Whether the computer player is up */
		bool ourTurn;
		
		/** Caches the current hash code */
		int hashCode;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@param greedyDivide how large a sum may be taken per turn
		@param highValue the highest-numbered piece to generate
		@param weAreUp whether or not the "good guy" is up
		*/
		explicit CrossoutState( int greedyDivide, int highValue,
			bool weAreUp=true );
		
		/**
		Creates the move resulting from taking away a specified number
			of pins.  This makes the most sense when a positive nu
			mber of them are taken, and preferably a legal number;
			however, this is not required.
		@pre The theft targets are in range.
		@pre The targets haven't yet been crossed out.
		@post The new state reflects the fact that it is now the oppos
			ite player's turn.
		@param baseState the state on which to base this new one
		@param firstTheft the first value to take
		@param secondTheft the (optional) second value to take
		*/
		CrossoutState( const CrossoutState& baseState,
			int firstTheft, int secondTheft=0 );
		
		/**
		Destroys the game state.
		*/
		~CrossoutState( void );
		
		/**
		Judges whether the game is over.
		@return whether there are no pins left
		*/
		bool gameOver( void ) const;
		
		/**
		Devines the match score, which is only meaningful if the game 
			is over.
		@return the score: <tt>Score::VICTORY</tt> for our victory, <t
			t>Score::LOSS</tt> for opponent's victory, or <tt>Scor
			e::TIE</tt> for an unterminated game
		*/
		Score scoreGame( void ) const;
		
		/**
		Determines whether it is our turn.
		@return whether the "good guy" is taking his turn
		*/
		bool computersTurn( void ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		const std::vector< CrossoutState > successors( void ) const;
		
		/**
		Produces a synopsis of this <tt>State</tt>'s particulars.
		@return the <tt>string</tt> representation
		*/
		std::string str( void ) const;
		
		/**
		Hashes the <tt>State</tt>.
		@pre <tt>hashCode</tt> is up to date
		@post The result is nonnegative.
		@return a hash code 
		*/
		int hash( void ) const;
		
		/**
		Checks identity
		@pre <tt>sorted</tt> is up to date
		@param another comparable <tt>State</tt>
		@return whether the turns and pin groups are the same
		*/
		bool operator==( const CrossoutState& another ) const;
		
		/**
		Performs assignment.
		@pre The two instances' constant data match
		@param another a source <tt>State</tt>
		@return the destination object
		*/
		CrossoutState& operator=( const CrossoutState& another );
		
		/**
		Determines whether two game states are subsequent.
		@param first the original state
		@param next the new state
		@return whether they would appear in a game in sequence
		*/
		static bool areSubsequent( const CrossoutState& first, const
			CrossoutState& next );
		
		/**
		Finds the move made to get between two game states.
		@pre The states must be exactly one move apart.
		@param first the original state
		@param next the new state
		@return a <tt>vector</tt> containing the values that were
			lifted, where 0 represents nothing taken, or an empty
			more pins were removed and how many were taken, or an 
			empty <tt>vector</tt> in the case of a poorly-phrased
			question (otherwise, its size is lte <tt>MAX_TAKEN</t
			t>
		*/
		static std::vector< int > diff( const CrossoutState& first,
			const CrossoutState& next );
	
	private: //helpers
		/**
		Recomputes hash code; must be called every time <tt>tray</
			tt> is mutated.
		*/
		void cacheHash( void );
};

#endif
