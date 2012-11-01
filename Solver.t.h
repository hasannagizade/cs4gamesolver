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
typename Solver< State >::StatePlusScore Solver< State >::nextBestState( const
	State& state ) const
{
	StatePlusScore decision( state );
	State* bestConfig=NULL;
	
	if( state.gameOver() )
	{
		decision.config=state;
		decision.value=state.scoreGame();
	}
	else if( remembered.contains( decision ) ) //we've evaluated this whole case before
	{
		#ifdef DEBUG
			cout<<"Memoization saved us work for "<<state.str()<<endl;
		#endif
		return remembered.matching( decision );
	}
	else //this situation is new to us
	{
		std::vector< State > successors=state.successors();
		
		for( typename std::vector< State >::iterator follower=successors.begin
			(); follower<successors.end(); ++follower )
		{
			StatePlusScore ofTheMoment=nextBestState( *follower );
			
			#if DEBUG
				if( state==current ) //list successors for top-level game state only
				{
					cout<<"Successor: "<<follower->str()<<' '<<follower->COLUMNS<<' '<<follower->ELEMENTS<<' '<<follower->gameOver()<<' '<<follower->scoreGame()<<' '<<ofTheMoment.value<<endl;
					cout<<"\tFollowing: "<<ofTheMoment.config.str()<<' '<<ofTheMoment.config.COLUMNS<<' '<<ofTheMoment.config.ELEMENTS<<' '<<ofTheMoment.config.gameOver()<<' '<<ofTheMoment.config.scoreGame()<<' '<<ofTheMoment.value<<endl;
				}
			#endif
			
			if( bestConfig==NULL || decision.prefersScore( ofTheMoment.value )
				)
			{
				#ifdef DEBUG
					std::cout<<"Deciding on "<<ofTheMoment.config.str()<<std::endl;
				#endif
				
				bestConfig=&*follower;
				decision.value=ofTheMoment.value;
			}
		}
		
		decision.config=*bestConfig;
		remembered.add( StatePlusScore( state ), decision ); //cherish this moment
		
		#ifdef DEBUG
			std::cout<<"Given "<<state.str()<<" chose "<<decision.config.str()<<
				" for victory rating "<<decision.value<<std::endl;
		#endif
	}
	
	return decision;
}

/** @brief Solver frontend */
template< typename State >
const State& Solver< State >::nextBestState()
{
	current=nextBestState( current ).config;
	
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
