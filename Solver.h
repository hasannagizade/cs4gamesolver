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
			/** Constructor */
			StatePlusScore(): config(), value( 0 ) {}
			
			/** The state */
			State config;
			
			/** The score */
			int value;
		};
		
		/**
		Determines the ideal end-of-turn state given the state at the beginning of the turn.
		@param state the <tt>State</tt> begin evaluated
		@return the preferred position and score
		*/
		StatePlusScore nextBestState( State& state ) const;
	
	public:
		/**
		Makes a <tt>Solver</tt> over a specific type of <tt>State</tt>.
		@param initial the initial game <tt>State</tt>
		*/
		Solver( State& initial );
		
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
		bool supplyNextState( State& future );
};

#include "Solver.t.h"

#endif
