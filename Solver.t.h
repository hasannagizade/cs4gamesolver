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
	StatePlusScore decision;
	
	if( state.gameOver() )
	{
		decision.config=state;
		decision.value=state.scoreGame();
	}
	else //!state.gameOver()
	{
		vector< State > successors=state.successors();
		
		bool decisionMade=false;
		for( typename vector< State >::iterator follower=successors.begin(); follower<successors.end(); ++follower )
		{
			StatePlusScore ofTheMoment=nextBestState( *follower );
			
			if( !decisionMade || ofTheMoment.value>decision.value )
			{
				decision.config=*follower;
				decisionMade=true;
			}
			decision.value+=ofTheMoment.value;
		}
	}
	
	#ifdef DEBUG
		cout<<"Victory rating for "<<state.str()<<" is "<<decision.value<<endl;
	#endif
	
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
