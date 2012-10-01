/** @author Sol Boucher <slb1566@rit.edu> */
//included from "Solver.h"
#include <vector>
using namespace std;

/** @brief Constructor */
template< typename State >
Solver< State >::Solver( State& initial ):
	current( initial ) {}

/** @brief Destructor */
template< typename State >
Solver< State >::~Solver() {}

/** @brief Current state */
template< typename State >
const State& Solver< State >::getCurrentState() const
{
	return current;
}

/** @brief Solver/bruteforcer */
template< typename State >
typename Solver< State >::StatePlusScore Solver< State >::nextBestState( State& state ) const
{
	StatePlusScore decision;
	
	if( state.gameOver() )
	{
		decision.config=state;
		decision.value=state.scoreGame();
	}
	else //!state.gameOver()
	{
		vector< State > successors=state.successors();
		
		decision.value=0;
		for(typename vector< State >::iterator follower=successors.begin(); follower<successors.end(); ++follower)
		{
			StatePlusScore ofTheMoment=nextBestState( *follower );
			
			if( ofTheMoment.value>decision.value )
			{
				ofTheMoment.config=*follower;
				decision=ofTheMoment;
			}
		}
	}
	
	return decision;
}

/** @brief Solver frontend */
template< typename State >
const State& Solver< State >::nextBestState()
{
	return nextBestState( current ).config;
}

/** @brief Human turn */
template< typename State >
bool Solver< State >::supplyNextState( State& future )
{
	if( State::areSubsequent( current, future ) )
	{
		current=future;
		
		return true;
	}
	else return false;
}
