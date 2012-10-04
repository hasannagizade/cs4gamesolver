/**
A game tree traverser for two-player, perfect knowledge contests.

@author Sol Boucher <slb1566@rit.edu>
*/
#ifndef SOLVER_H
#define SOLVER_H

template< typename State > class Solver
{
	private:
		/** The current game state */
		State current;
		
		/**
		Combines a state and its score.
		*/
		struct StatePlusScore
		{
			/** The state */
			State config;
			
			/** The score */
			typename State::Score value;
			
			/**
			Constructor; assumes that the <tt>State::Score</tt>'s default (zero) value indicates a balanced (or at least undetermined-as-yet) match.
			@param initial the starting <tt>State</tt>
			*/
			StatePlusScore( State initial );
			
			/**
			Checks whether the player whose turn this is would prefer to have the <tt>alternative</tt> score.  Assumes that higher scores are better for the computer player, regardless of who's up.
			@param alternative the other score we're offering the player
			@return whether our <tt>State<tt>'s player prefers the score it's been offered
			*/
			bool prefersScore( typename State::Score alternative ) const;
		};
		
		/**
		Determines the ideal end-of-turn state given the state at the beginning of the turn.
		@param state the <tt>State</tt> being evaluated
		@return the preferred position and score
		*/
		StatePlusScore nextBestState( const State& state ) const;
	
	public:
		/**
		Makes a <tt>Solver</tt> over a specific type of <tt>State</tt>.
		@param initial the initial game <tt>State</tt>
		*/
		Solver( const State& initial );
		
		/**
		Destroys the <tt>Solver</tt>.
		*/
		~Solver( void );
		
		/**
		Queries for the current state.
		@return the current <tt>State</tt>
		*/
		const State& getCurrentState( void ) const;
		
		/**
		Advances the game to the most favorable state.
		@return the new <tt>State</tt>
		*/
		const State& nextBestState( void );
		
		/**
		Manually make <i>one</i> move and advances the game to the specified outcome state.
		@param move the outcome of the move
		@return <tt>true</tt> if only one move was made, <tt>false</tt> otherwise
		*/
		bool supplyNextState( const State& future );
};

#include "Solver.t.h"

#endif
