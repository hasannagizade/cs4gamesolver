/** @author Sol Boucher <slb1566@rit.edu>
 *  @author Kyle Savarese <kms7341@rit.edu>
 */
#include "KaylesState.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

/** @brief Advancing constructor */
KaylesState::KaylesState( const KaylesState& baseState, unsigned int position,
	int taken, int target ):
	ourTurn( !baseState.ourTurn ), hashCode( 0 )
{
	assert( position<baseState.pins.size() );
	for ( int pos = 0; pos < baseState.groupsOfPins(); pos++) {
		if ( unsigned( pos ) == position ) {
			if( target != 0 )
				pins.push_back( target );
			if( baseState.pinsInGroup( pos ) - target - taken ) {
				pins.push_back( baseState.
					pinsInGroup( pos ) - target - taken );
			}
		}
		else {
			if ( baseState.pinsInGroup( pos ) ) 
				pins.push_back( baseState.pinsInGroup( pos )
					);
		}
	}

	#ifdef DEBUG
		cout<<"Advancing state w/ pos "<<position<<" , taking "<<taken
			<<endl;
	#endif
	
	assert( position<baseState.pins.size() );
	cacheHash();
}

/** @brief Are we out of objects? */
bool KaylesState::gameOver() const
{
	for( vector< int >::const_iterator group=pins.begin();
		group!=pins.end(); ++group )
		if( *group!=0 ) return false;
	
	return true;
}

/** @brief What might happen next? */
void KaylesState::successors( vector< KaylesState >& possibilities ) const
{
	#ifdef DEBUG
		cout<<"Success calculating successors for "<<str()<<'\n';
	#endif
	
	for( unsigned int group=0; group<pins.size(); ++group ) {
		for( int pos = 0; pos < pins[group]; pos++ ) {
			for ( int taken = 1; pos + taken <= pins[group] &&
				taken <=2; taken++ ) {
				possibilities.push_back( KaylesState
					( KaylesState( pins, ourTurn ), group,
					taken, pos ) );

				#ifdef DEBUG
					cout<<'\t'<<possibilities.back().str()
						<<'\n';
				#endif
			}
		}
	}
	#ifdef DEBUG
		cout.flush();
	#endif
}

/** @brief Textualizes */
string KaylesState::str() const
{
	stringstream assembler;
	
	assembler<<"It is the "<<( ourTurn ? "computer" : "human" )<<
		"'s turn and the pins are: ";
	for( vector< int >::const_iterator group=pins.begin();
		group<pins.end(); ++group )
		assembler<<*group<<' ';
	assembler.flush();
	
	return assembler.str();
}

/** @brief Are these subsequent? */
bool KaylesState::areSubsequent( const KaylesState& first, const KaylesState&
	next )
{
	if( first.ourTurn==next.ourTurn ||
		( first.pins.size()!=next.pins.size() + 1 &&
		next.pins.size() != first.pins.size() + 1 &&
		first.pins.size()!=next.pins.size() ) )
		return false;
	
	int diff = 0;

	bool seenDifference=false; //whether we've already found the move
		//that was made
	if ( first.pins.size() > next.pins.size()) {
	for( unsigned int group=0; group<next.pins.size(); ++group ) {
		if( first.pins[group + diff]!=next.pins[group] )
		{
			if( seenDifference )
				return false; //cannot tolerate any more
					//differences
			else //offset==0
			{
				int difference = first.pins[group];
				
				#ifdef DEBUG
					cout << "difference: " << difference
						<< endl;
				#endif

				if( difference<MIN_TAKEN ||
					difference>MAX_TAKEN ||
					next.pins[group]<0 ) 
					return false;
				else seenDifference=true;

				diff = 1;
			}
		}
	}
	if ( diff != 1 ) {
		int difference = first.pins[first.pins.size() - 1];
		#ifdef DEBUG
			cout << "difference: " << difference << endl;
		#endif
		if( difference < MIN_TAKEN || difference > MAX_TAKEN )
			return false;
		else seenDifference = true;
	}
	}
	else if ( next.pins.size() > first.pins.size() ) { 
	for ( unsigned int group = 0; group < first.pins.size(); ++group ) {
		if( first.pins[group] != next.pins[group + diff] ) {
			if ( seenDifference ) {
				#ifdef DEBUG
					cout << "TWODIFFS" << endl;
				#endif
				return false;
			}
			else {
				int difference = first.pins[group]
					- (next.pins[group]
					+ next.pins[group + 1]);

				#ifdef DEBUG
					cout << "difference: " << difference
						<< endl;
				#endif

				if ( difference < MIN_TAKEN ||
					difference > MAX_TAKEN ||
					next.pins[group] < 0 )
					return false;
				else seenDifference=true;

				diff = 1;
			}
		}
	}
	if ( diff != 1 ) {
		int difference = next.pins[next.pins.size() - 1];
		if( difference < MIN_TAKEN || difference > MAX_TAKEN )
			return false;
		else seenDifference = true;
	}
	}
	else {
	for ( unsigned int group = 0; group < first.pins.size(); ++group) {
		if( first.pins[group] != next.pins[group] ) {
			if ( seenDifference ) 
				return false;
			else {
				int difference = first.pins[group]
					- next.pins[group];

				if( difference < MIN_TAKEN ||
					difference > MAX_TAKEN ||
					next.pins[group] < 0 )
					return false;
				else seenDifference = true;
			}
		}
	}	}
	
	return seenDifference;
}

/** @brief What just happened? */
vector< int > KaylesState::diff( const KaylesState& first, const
	KaylesState& next )
{
	#ifdef DEBUG
		cout<<"Diffing "<<first.str()<<" and "<<next.str()<<endl;
	#endif
	
	bool subsequentStates=areSubsequent( first, next );
	
	assert( subsequentStates );
	vector< int > diffs;
	if( subsequentStates )
	{
		if ( first.pins.size() > next.pins.size() ) {
		for( unsigned int group=0; group<next.pins.size(); ++group ) {
			if( first.pins[group]!=next.pins[group] ) {
				//removed non-final line
				diffs.push_back( group );
				diffs.push_back( 0 );
				diffs.push_back( first.pins[group] );
				return diffs;
			}
		}
		//removed final line
		diffs.push_back( next.pins.size() );
		diffs.push_back( 0 );
		diffs.push_back( first.pins[next.pins.size()] );
		return diffs;
		}
		else if ( first.pins.size() < next.pins.size() ){
		for ( unsigned int group = 0; group<first.pins.size();
			++group ) {
			if( first.pins[group]!=next.pins[group] ) {
				//split up a line
				diffs.push_back( group );
				diffs.push_back( next.pins[group] );
				diffs.push_back( first.pins[group]
					- next.pins[group]
					- next.pins[ group+1 ] );
				return diffs;
			}
		}
		}
		else {
		for ( unsigned int group = 0; group<first.pins.size();
			++group ) {
			if( first.pins[group]!=next.pins[group] ) {
				//shrank a line
				diffs.push_back( group );
				diffs.push_back( next.pins[group] );
				diffs.push_back( first.pins[group]
					- next.pins[group] );
				return diffs;
			}
		}
		}
	}
	//else !areSubsequent( first, next )
		return vector< int >();
}

/** Sorting */
void KaylesState::cacheHash()
{
	hashCode=( ourTurn ? 1 : 0 )<<pins.size();
	for( vector< int >::iterator count=pins.begin(); count!=pins.end();
		++count )
		hashCode+=*count<<( count-pins.begin() );
	hashCode=abs( hashCode );
	assert( hashCode>=0 );
}
