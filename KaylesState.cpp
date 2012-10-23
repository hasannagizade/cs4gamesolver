/** @author Sol Boucher <slb1566@rit.edu>
 *  @author Kyle Savarese <kms7341@rit.edu>
 */
#include "KaylesState.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#define DEBUG
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
	
	assert( position<baseState.pins.size() );
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
	for( unsigned int group=0; group<pins.size(); ++group ) {
		for( int pos = 0; pos < pins[group]; pos++ ) {
			vector< int > nextChances( pins );
			
			nextChances[group] = pos;
			nextChances.insert(nextChances.begin() + group + 1, pins[group] - 1 - pos);

			possibilities.push_back( KaylesState( nextChances, !ourTurn ) );

			#ifdef DEBUG
				cout<<'\t'<<possibilities.back().str()<<'\n';
			#endif

			if ( pos < pins[group] - 1 ) {
				nextChances[group + 1]--;
				if( nextChances[group + 1] == 0 )
					nextChances.erase(nextChances.begin() + group + 1);
				possibilities.push_back( KaylesState( nextChances, !ourTurn ) );

				#ifdef DEBUG
					cout << '\t'<<possibilities.back().str()<<'\n';
				#endif
			}
		}
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
	if( first.ourTurn==next.ourTurn || ( first.pins.size()!=next.pins.size() + 1 
		&& next.pins.size() != first.pins.size() + 1 && first.pins.size()!=next.pins.size() ) )
		return false;
	
	int diff = 0;

	bool seenDifference=false; //whether we've already found the move that was
		//made
	if ( first.pins.size() > next.pins.size()) {
	for( unsigned int group=0; group<next.pins.size(); ++group ) {
		if( first.pins[group + diff]!=next.pins[group] )
		{
			if( seenDifference )
				return false; //cannot tolerate any more differences
			else //offset==0
			{
				int difference=next.pins[group] - (first.pins[group] + first.pins[group + 1]);
				
				#ifdef DEBUG
					cout << "difference: " << difference << endl;
				#endif

				if( difference<MIN_TAKEN || difference>MAX_TAKEN || next.pins[group]<0 ) 
					return false;
				else seenDifference=true;

				diff = 1;
			}
		}
	}  	}
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
				int difference = first.pins[group] - (next.pins[group] + next.pins[group + 1]);

				#ifdef DEBUG
					cout << "difference: " << difference << endl;
				#endif

				if ( difference < MIN_TAKEN || difference > MAX_TAKEN || next.pins[group] < 0 )
					return false;
				else seenDifference=true;

				diff = 1;
			}
		}
	}	}
	else {
	for ( unsigned int group = 0; group < first.pins.size(); ++group) {
		if( first.pins[group] != next.pins[group] ) {
			if ( seenDifference ) 
				return false;
			else {
				int difference = first.pins[group] - next.pins[group];

				if( difference < MIN_TAKEN || difference > MAX_TAKEN || next.pins[group] < 0 )
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
		for( unsigned int group=0; group<first.pins.size(); ++group ) {
			if( first.pins[group]!=next.pins[group] ) {
				diffs.push_back( group );
				diffs.push_back( first.pins[group] );
				diffs.push_back( next.pins[group]-(first.pins[group]+first.pins[group+1]));
				return diffs;
			}
		}
		}
		else {
		for ( unsigned int group = 0; group<next.pins.size(); ++group ) {
			if( first.pins[group]!=next.pins[group] ) {
				diffs.push_back( group );
				diffs.push_back( next.pins[group] );
				diffs.push_back( first.pins[group]-(next.pins[group]+next.pins[group+1]));
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
	for( vector< int >::iterator count=pins.begin(); count!=pins.end(); ++count )
		hashCode+=*count<<( count-pins.begin() );
}
