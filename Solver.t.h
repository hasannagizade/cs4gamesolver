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

/** @author Sol Boucher <slb1566@rit.edu> */
//included from "Solver.h"
#include <iostream>
#include <vector>

/** @brief Constructor */
template< typename State >
Solver< State >::Solver( const State& initial ):
	current( initial ), remembered() {}

/** @brief Destructor */
template< typename State >
Solver< State >::~Solver() {}

/** @brief ConSTRUCTor */
template< typename State >
Solver< State >::StatePlusScore::StatePlusScore( State initial ):
	config( initial ), value()
{
	#ifdef DEBUG
		std::cout<<"New SPS w/ score of "<<value<<std::endl;
	#endif
}

/** @brief What would the current player say? */
template< typename State >
bool Solver< State >::StatePlusScore::prefersScore( typename State::Score
	alternative ) const
{
	if( config.computersTurn() )
		return alternative>value; //the machine accepts a machine win
	else //human's turn
		return alternative<value; //the human rejects a machine win
}

/** @brief Configuration's hash code */
template< typename State >
int Solver< State >::StatePlusScore::hash() const
{
	return config.hash();
}

/** @brief Configuration's identity verification */
template< typename State >
bool Solver< State >::StatePlusScore::operator==( const StatePlusScore&
	another ) const
{
	return this->config==another.config;
}

/** @brief Current state */
template< typename State >
const State& Solver< State >::getCurrentState() const
{
	return current;
}

/** @brief Solver/bruteforcer */
template< typename State >
void Solver< State >::nextBestState( const
	State& state, StatePlusScore& decision ) const
{
	State* bestConfig=NULL;
	
	if( state.gameOver() )
	{
		decision.config=state;
		decision.value=state.scoreGame();
	}
	else if( remembered.contains( decision ) ) //we've evaluated this
		//whole case before
	{
		#ifdef DEBUG
			std::cout<<"Memoization saved us work for "
				<<state.str()<<std::endl;
		#endif
		remembered.matching( decision, decision );
	}
	else //this situation is new to us
	{
		std::vector< State > successors;
		state.successors( successors );
		
		for( typename std::vector< State >::iterator
			follower=successors.begin();
			follower<successors.end(); ++follower )
		{
			StatePlusScore ofTheMoment( *follower );
			nextBestState( *follower, ofTheMoment );
			
			if( bestConfig==NULL || decision.prefersScore(
				ofTheMoment.value ) )
			{
				#ifdef DEBUG
					std::cout<<"Deciding on "
						<<ofTheMoment.config.str()
						<<std::endl;
				#endif
				
				bestConfig=&*follower;
				decision.value=ofTheMoment.value;
			}
		}
		
		decision.config=*bestConfig;
		remembered.add( StatePlusScore( state ), decision );
			//cherish this
			//moment
		
		#ifdef DEBUG
			std::cout<<"Given "<<state.str()<<" chose "
				<<decision.config.str()
				<<" for victory rating "<<decision.value
				<<std::endl;
		#endif
	}
}

/** @brief Solver frontend */
template< typename State >
const State& Solver< State >::nextBestState()
{
	StatePlusScore outcome( current );
	nextBestState( current, outcome );
	current=outcome.config;
	
	return current;
}

/** @brief Human turn */
template< typename State >
bool Solver< State >::supplyNextState( const State& future )
{
	if( State::areSubsequent( current, future ) )
	{
		current=future;
		
		return true;
	}
	else return false;
}
