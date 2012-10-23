/** @author Sol Boucher <slb1566@rit.edu> */
#include "KaylesState.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

/** @brief Constructor */
KaylesState::KaylesState( const vector< int >& startingPins, bool weAreUp ):
	pins( startingPins ), ourTurn( weAreUp ), hashCode( 0 )
{
	//eliminate unnecessary zeros:
	for( vector< int >::iterator spot=pins.begin() ; spot!=pins.end(); ++spot )
		if( *spot==0 )
			spot=pins.erase( spot );
	
	cacheHash();
}

/** @brief Advancing constructor */
KaylesState::KaylesState( const KaylesState& baseState, unsigned int position,
	int taken, int which ):
	pins(), ourTurn( !baseState.ourTurn ), hashCode( 0 )
{
	#ifdef DEBUG
		cout<<"Advancing state w/ pos "<<position<<" , taking "<<taken<<endl;
	#endif
	
	assert( position<baseState.pins.size() && which>=0 && taken>=MIN_TAKEN && taken<=MAX_TAKEN && which+taken<=baseState.pins[position] );
	
	for( vector< int >::const_iterator spot=baseState.pins.begin(); spot!=baseState.pins.end(); ++spot )
	{
		if( spot-baseState.pins.begin()==position ) //this is the one to remove
		{
			if( taken!=*spot ) //there'll be survivors
			{
				if( which==0 || which+taken==*spot ) //taking from the edge of the group
					pins.push_back( *spot-taken ); //just deduct the number taken
				else //taking from the middle
				{ //split the group in "half"
					pins.push_back( position-1 );
					pins.push_back( *spot-position-taken );
				}
			}
			//else don't bother to copy anything
		}
		else pins.push_back( *spot );
	}
	
	cacheHash(); //be efficient
}

/** @brief Destructor */
KaylesState::~KaylesState() {}

/** @brief Are we out of objects? */
bool KaylesState::gameOver() const
{
	return pins.size()==0;
}

/** @brief Who won? */
KaylesState::Score KaylesState::scoreGame() const
{
	if( gameOver() )
		if( ourTurn ) return LOSS;
		else /*!ourTurn*/ return VICTORY;
	else /*!gameOver()*/ return TIE;
}

/** @brief Is it our turn? */
bool KaylesState::computersTurn() const
{
	return ourTurn;
}

/** @brief How many groups? */
int KaylesState::groupsOfPins() const
{
	return pins.size();
}

/** @brief How many pins? */
int KaylesState::pinsInGroup( unsigned int group ) const
{
	if( group>=pins.size() ) return -1;
	else return pins[group];
}

/** @brief What might happen next? */
const vector< KaylesState > KaylesState::successors() const
{
	#ifdef DEBUG
		cout<<"Success calculating successors for "<<str()<<'\n';
	#endif
	
	vector< KaylesState > possibilities;
	
	for( unsigned int group=0; group<pins.size(); ++group )
		for( int start=0; start<pins[group]; ++start )
			for( int take=MIN_TAKEN; take<=MAX_TAKEN && start+take<pins[group]; ++take )
			{
				possibilities.push_back( KaylesState( *this, group, start, take ) );
				
				#ifdef DEBUG
					cout<<'\t'<<possibilities.back().str()<<'\n';
				#endif
			}
	
	#ifdef DEBUG
		cout.flush();
	#endif
	
	return possibilities;
}

/** @brief Textualizes */
string KaylesState::str() const
{
	stringstream assembler;
	
	assembler<<"It is the "<<( ourTurn ? "computer" : "human" )<<
		"'s turn and the pins groups are: ";
	for( vector< int >::const_iterator group=pins.begin(); group<pins.end();
		++group )
		assembler<<*group<<' ';
	assembler.flush();
	
	return assembler.str();
}

/** @brief Hashing */
int KaylesState::hash() const
{
	return hashCode;
}

/** @brief Same state? */
bool KaylesState::operator==( const KaylesState& another ) const
{
	return this->ourTurn==another.ourTurn && this->pins.size()==another.pins.size() &&
		equal( this->pins.begin(), this->pins.end(), another.pins.begin() );
}

/** @brief Are these subsequent? */
bool KaylesState::areSubsequent( const KaylesState& first, const KaylesState&
	next )
{
	if( first.ourTurn==next.ourTurn || first.pins.size()!=next.pins.size() )
		return false;
	
	bool seenDifference=false; //whether we've already found the move that was
		//made
	
	for( unsigned int group=0; group<first.pins.size(); ++group )
		if( first.pins[group]!=next.pins[group] )
		{
			if( seenDifference )
				return false; //cannot tolerate any more differences
			else //offset==0
			{
				int difference=first.pins[group]-next.pins[group];
				
				if( difference<MIN_TAKEN || difference>MAX_TAKEN || next.pins[
					group]<0 ) return false;
				else seenDifference=true;
			}
		}
	
	return seenDifference;
}

/** @brief What just happened? */
pair< int, int > KaylesState::diff( const KaylesState& first, const
	KaylesState& next )
{
	#ifdef DEBUG
		cout<<"Diffing "<<first.str()<<" and "<<next.str()<<endl;
	#endif
	
	bool subsequentStates=areSubsequent( first, next );
	
	assert( subsequentStates );
	if( subsequentStates )
	{
		for( unsigned int group=0; group<first.pins.size(); ++group )
			if( first.pins[group]!=next.pins[group] )
				return pair< int, int >( group, first.pins[group]-next.pins
					[group] );
	}
	//else !areSubsequent( first, next )
		return pair< int, int>();
}

/** Sorting */
void KaylesState::cacheHash()
{
	hashCode=( ourTurn ? 1 : 0 )<<pins.size();
	for( vector< int >::iterator count=pins.begin(); count!=pins.end(); ++count )
		hashCode+=*count<<( count-pins.begin() );
}
