/**
Represents the Kayles game state at some fixed point in time.

@author Sol Boucher <slb1566@rit.edu>
*/
#ifndef KAYLESSTATE_H
#define KAYLESSTATE_H

#include <string>
#include <utility>
#include <vector>
using namespace std;

class KaylesState
{
	public: //gameplay constants
		/** The maximum number of pins a player may topple per turn. */
		static const int MAX_TAKEN=2;
		
		/** The minimum number of pins permissible to topple. */
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
		vector<int> pins;
		bool ourTurn;
	
	public: //behavior
		/**
		Creates a new game given its initial circumstances.
		@param startingPins the initial pin arrangement
		@param weAreUp whether or not the "good guy" is up
		*/
		explicit KaylesState( const vector<int>& startingPins=vector<int>(), bool weAreUp=true );
		
		/**
		Creates the move resulting from taking away a specified number of pins.  This makes the most sense when a positive number of them are taken, and preferably a legal number; however, this is not required.
		@pre the <tt>position</tt> is in range
		@post The new state reflects the fact that it is now the opposite player's turn.
		@param baseState the state on which to base this new one
		@param position the group of pins affected
		@param taken the number of from that group that were knocked down
		*/
		KaylesState( const KaylesState& baseState, unsigned int position, int taken );
		
		/**
		Destroys the game state.
		*/
		~KaylesState( void );
		
		/**
		Judges whether the game is over.
		@return whether there are no pins left
		*/
		bool gameOver( void ) const;
		
		/**
		Devines the match score, which is only meaningful if the game is over.
		@return the score: <tt>Score::VICTORY</tt> for our victory, <tt>Score::LOSS</tt> for opponent's victory, or <tt>Score::TIE</tt> for an unterminated game
		*/
		Score scoreGame( void ) const;
		
		/**
		Determines whether it is our turn.
		@return whether the "good guy" is taking his turn
		*/
		bool computersTurn( void ) const;
		
		/**
		Counts the pin groups.
		@return how many groups exist
		*/
		int groupsOfPins( void ) const;
		
		/**
		Counts the pins in a group.
		@param group the group to examine
		@return how many pins occupy it, or <tt>-1</tt> if it doesn't exist
		*/
		int pinsInGroup( unsigned int group ) const;
		
		/**
		Returns all possible successor states.
		@return whatever might happen next
		*/
		const vector< KaylesState > successors( void ) const;
		
		/**
		Produces a synopsis of this <tt>State</tt>'s particulars.
		@return the <tt>string</tt> representation
		*/
		string str( void ) const;
		
		/**
		Determines whether two game states are subsequent.
		@param first the original state
		@param next the new state
		@return whether they would appear in a game in sequence
		*/
		static bool areSubsequent( const KaylesState& first, const KaylesState& next );
		
		/**
		Finds the move made to get between two game states.
		@pre The states must be exactly one move apart.
		@param first the original state
		@param next the new state
		@return a <tt>pair</tt> containing the group from which one or more pins were removed and how many were taken, or an empty <tt>pair</tt> in the case of a poorly-phrased question
		*/
		static pair< int, int > diff( const KaylesState& first, const KaylesState& next );
};

#endif
