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

/** @brief Solver/bruteforcer */
template< typename State >
State Solver< State >::nextBestState( State& state )
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
State Solver< State >::nextBestState()
{
	return nextBestState( current ).config;
}
