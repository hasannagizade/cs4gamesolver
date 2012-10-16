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
	cacheHash();
}

/** @brief Advancing constructor */
KaylesState::KaylesState( const KaylesState& baseState, unsigned int position,
	int taken ):
	pins( baseState.pins ), ourTurn( !baseState.ourTurn ), hashCode( 0 )
{
	#ifdef DEBUG
		cout<<"Advancing state w/ pos "<<position<<" , taking "<<taken<<endl;
	#endif
	
	assert( position>=0 && position<baseState.pins.size() );
	pins[position]-=taken;
	cacheHash();
}

/** @brief Destructor */
KaylesState::~KaylesState() {}

/** @brief Are we out of objects? */
bool KaylesState::gameOver() const
{
	for( vector< int >::const_iterator group=pins.begin(); group!=pins.end();
		++group )
		if( *group!=0 ) return false;
	
	return true;
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
	if( group<0 || group>=pins.size() ) return -1;
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
		for( int take=MIN_TAKEN; take<=MAX_TAKEN && take<=pins[group]; ++take
			)
		{
			possibilities.push_back( KaylesState( *this, group, take ) );
			
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
		"'s turn and the pins are: ";
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
	
	return true;
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
	vector< int > sorted;
	
	for( vector< int >::iterator element=pins.begin(); element!=pins.end(); ++element )
		sorted.push_back(*element);
	sort< vector< int >::iterator >( sorted.begin(), sorted.end() );
	
	hashCode=0;
	for( vector< int >::iterator count=sorted.begin(); count!=sorted.end(); ++count )
		hashCode+=*count<<( count-sorted.begin() );
}
