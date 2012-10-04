/** @author Sol Boucher <slb1566@rit.edu> */
//included from "Solver.h"
#include <iostream>
#include <vector>
using namespace std;

/** @brief Constructor */
template< typename State >
Solver< State >::Solver( const State& initial ):
	current( initial ) {}

/** @brief Destructor */
template< typename State >
Solver< State >::~Solver() {}

/** @brief ConSTRUCTor */
template< typename State >
Solver< State >::StatePlusScore::StatePlusScore( State initial ):
	config( initial ), value()
{
	#ifdef DEBUG
		cout<<"New SPS w/ score of "<<value<<endl;
	#endif
}

/** @brief What would the current player say? */
template< typename State >
bool Solver< State >::StatePlusScore::prefersScore( typename State::Score alternative ) const
{
	if( config.computersTurn() )
		return alternative>value; //the machine accepts a machine win
	else //human's turn
		return alternative<value; //the human rejects a machine win
}

/** @brief Current state */
template< typename State >
const State& Solver< State >::getCurrentState() const
{
	return current;
}

/** @brief Solver/bruteforcer */
template< typename State >
typename Solver< State >::StatePlusScore Solver< State >::nextBestState( const State& state ) const
{
	StatePlusScore decision( state );
	State* bestConfig=NULL;
	
	if( state.gameOver() )
	{
		decision.config=state;
		decision.value=state.scoreGame();
	}
	else //!state.gameOver()
	{
		vector< State > successors=state.successors();
		
		for( typename vector< State >::iterator follower=successors.begin(); follower<successors.end(); ++follower )
		{
			StatePlusScore ofTheMoment=nextBestState( *follower );
			
			if( bestConfig==NULL || decision.prefersScore( ofTheMoment.value ) )
			{
				#ifdef DEBUG
					cout<<"Deciding on "<<ofTheMoment.config.str()<<endl;
				#endif
				
				bestConfig=&*follower;
				decision.value=ofTheMoment.value;
			}
		}
		decision.config=*bestConfig;
	
	#ifdef DEBUG
		cout<<"Given "<<state.str()<<" chose "<<decision.config.str()<<" for victory rating "<<decision.value<<endl;
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
