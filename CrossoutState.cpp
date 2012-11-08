/** @author Sol Boucher <slb1566@rit.edu> */
#include "CrossoutState.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

/** @brief Constructor */
CrossoutState::CrossoutState( int greedyDivide, int highValue, bool weAreUp ):
	MAX_SUM( greedyDivide ), tray(), ourTurn( weAreUp )
{
	for( int index=0; index<highValue; ++index )
		tray.push_back( true );
	
	cacheHash();
}

/** @brief Advancing constructor */
CrossoutState::CrossoutState( const CrossoutState& baseState,
	int firstTheft, int secondTheft ):
	MAX_SUM( baseState.MAX_SUM ), tray( baseState.tray ),
	ourTurn( !baseState.ourTurn ), hashCode( 0 )
{
	--firstTheft; //switch to 0-based indexing
	--secondTheft; //likewise
	assert( firstTheft>=0 && unsigned( firstTheft )<tray.size() );
	assert( secondTheft<signed( tray.size() ) );
	
	tray[firstTheft]=false;
	
	if( secondTheft>-1 )
		tray[secondTheft]=false;
	
	cacheHash();
}

/** @brief Are we out of objects? */
bool CrossoutState::gameOver() const
{
	for( unsigned int location=1; location<=tray.size() &&
		location<=MAX_SUM; ++location )
		if( tray[location-1] ) return false;
	
	return true;
}

/** @brief What might happen next? */
void CrossoutState::successors( vector< CrossoutState >& possibilities ) const
{
	#ifdef DEBUG
		cout<<"Success calculating successors for "<<str()<<'\n';
	#endif
	
	for( unsigned int first=1; first<=tray.size() && first<=MAX_SUM;
		++first )
		if( tray[first-1] )
		{
			possibilities.push_back( CrossoutState( *this, first
				) );
			assert( possibilities.size() );
			for( unsigned int second=first+1; second<=tray.size()
				&& first+second<=MAX_SUM; ++second )
				if( tray[second-1] ) possibilities.push_back(
					CrossoutState( *this, first, second )
						);
		}
}

/** @brief Textualizes */
string CrossoutState::str() const
{
	stringstream assembler;
	
	assembler<<"It is the "<<( ourTurn ? "computer" : "human" )<<
		"'s turn and the pins are: ";
	for( vector< bool >::const_iterator num=tray.begin();
		num<tray.end(); ++num )
		if( *num )
			assembler<<( num-tray.begin()+1 )<<' ';
	assembler.flush();
	
	return assembler.str();
}

/** @brief Assignment */
CrossoutState& CrossoutState::operator=( const CrossoutState& another )
{
	assert( this->MAX_SUM==another.MAX_SUM );
	
	if( this!=&another ) //no funny business
	{
		this->tray=another.tray;
		this->ourTurn=another.ourTurn;
		this->hashCode=another.hashCode;
	}
	
	return *this;
}

/** @brief Are these subsequent? */
bool CrossoutState::areSubsequent( const CrossoutState& first, const
	CrossoutState& next )
{
	if( first.MAX_SUM!=next.MAX_SUM ||
		first.tray.size()!=next.tray.size() ||
		first.ourTurn==next.ourTurn )
		return false;
	
	unsigned int count=0, sum=0;
	for( unsigned int num=0; num<first.tray.size(); ++num )
	{
		if( next.tray[num] && !first.tray[num] ) //UNcrossed
			//something!
			return false;
		else if( first.tray[num] && !next.tray[num] )
		{
			++count;
			sum+=num+1;
		}
	}
	
	return count>=MIN_TAKEN && count<=MAX_TAKEN && sum<=first.MAX_SUM;
}

/** @brief What just happened? */
vector< int > CrossoutState::diff( const CrossoutState& first, const
	CrossoutState& next )
{
	bool subsequentStates=areSubsequent( first, next );
	
	assert( subsequentStates );
	vector< int > diffs;
	for( unsigned int num=0; num<first.tray.size(); ++num )
		if( first.tray[num]!=next.tray[num] )
			diffs.push_back( num+1 );
	
	return diffs;
}

/** Sorting */
void CrossoutState::cacheHash()
{
	hashCode=( ourTurn ? 1 : 0 )<<tray.size();
	for( vector< bool >::iterator count=tray.begin(); count!=tray.end();
		++count )
		hashCode+=( *count ? 1 : 0 )<<( count-tray.begin() );
	hashCode=abs( hashCode );
	assert( hashCode>=0 );
}
