/*
 * Copyright (C) 2012 Sol Boucher and Kyle Savarese
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "HashTable.h"

/**
A game tree traverser for two-player, perfect knowledge contests.

@author Sol Boucher <slb1566@rit.edu>
*/
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
			Constructor; assumes that the <tt>State::Score</tt>'s
				default (zero) value indicates a balanced (or
				at least undetermined-as-yet) match.
			@param initial the starting <tt>State</tt>
			*/
			StatePlusScore( State initial );
			
			/**
			Checks whether the player whose turn this is would
				prefer to have the <tt>alternative</tt> score.
				Assumes that higher scores are better for the
				computer player, regardless of who's up.
			@param alternative the other score we're offering the
				player
			@return whether our <tt>State<tt>'s player prefers
				the score it's been offered
			*/
			bool prefersScore( typename State::Score alternative
				) const;
			
			/**
			Retrieves hash code.
			@return our <tt>State</tt>'s hash code
			*/
			int hash( void ) const;
			
			/**
			Verifies equivalent identities.
			@param another <tt>StatePlusScore</tt> instance
			@return whether our <tt>State</tt>s identify
			*/
			bool operator==( const StatePlusScore& another )
				const;
		};
		
		/** Previously-determined states for memoization */
		mutable HashTable< StatePlusScore > remembered;
		
		/**
		Determines the ideal end-of-turn state given the state at the
			beginning of the turn.
		@param state the <tt>State</tt> being evaluated
		@param result the preferred position and score
		*/
		void nextBestState( const State& state, StatePlusScore& result ) const;
	
	public:
		/**
		Makes a <tt>Solver</tt> over a specific type of
			<tt>State</tt>.
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
		Manually make <i>one</i> move and advances the game to the
			specified outcome state.
		@param move the outcome of the move
		@return <tt>true</tt> if only one move was made,
			<tt>false</tt> otherwise
		*/
		bool supplyNextState( const State& future );
};

#include "Solver.t.h"

#endif
