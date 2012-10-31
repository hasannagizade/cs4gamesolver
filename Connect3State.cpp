/** @author Sol Boucher <slb1566@rit.edu> */
#include "Connect3State.h"
#include <cassert>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

/** @brief Board symbols */
const char Connect3State::SYMBOLS[2]={'X', 'O'};

/** @brief Constructor */
Connect3State::Connect3State( unsigned int columnCount, unsigned int elementCount,
	const std::vector< std::vector< char > >& original, bool weAreUp ):
	MY_SYMBOL( weAreUp ? 0 : 1 ), COLUMNS( columnCount ), ELEMENTS( elementCount ),
	board( original ), ourTurn( weAreUp) 
{
	assert( board.size()==COLUMNS );
	for( vector< vector< char > >::iterator col=board.begin(); col!=board.end();
		++col )
		assert( col->size()<=ELEMENTS );
	
	finalOutcome=computeWinner();
	cacheHash();
}

/** @brief Advancing constructor */
Connect3State::Connect3State( const Connect3State& baseState, unsigned int column ):
	MY_SYMBOL( 1-baseState.MY_SYMBOL ),
	COLUMNS( baseState.COLUMNS ), ELEMENTS( baseState.ELEMENTS ),
	board( baseState.board ), ourTurn ( !baseState.ourTurn )
{
	assert( column<board.size() );
	board[column].push_back( SYMBOLS[MY_SYMBOL] );
	assert( board[column].size()<ELEMENTS );
	
	finalOutcome=computeWinner( column );
	cacheHash();
}

/** @brief Destructor */
Connect3State::~Connect3State() {}

/** @brief Are we out of objects? */
bool Connect3State::gameOver() const
{
	for( vector< vector< char > >::const_iterator column=board.begin();
		column!=board.end(); ++column )
		if( column->size()<ELEMENTS ) return false; //column not complete
	
	return finalOutcome!=TIE; //has anyone won?
}

/** @brief Who won? */
Connect3State::Score Connect3State::scoreGame() const
{
	return finalOutcome;
}

/** @brief Is it our turn? */
bool Connect3State::computersTurn() const
{
	return ourTurn;
}

/** @brief What might happen next? */
const vector< Connect3State > Connect3State::successors() const
{
	vector< Connect3State > possibilities;
	for( unsigned int column=0; column<board.size(); ++column )
		if( board[column].size()<ELEMENTS-1 )
			possibilities.push_back( Connect3State( *this, column ) );
	
	return possibilities;
}

/** @brief Textualizes */
string Connect3State::str() const
{
	stringstream assembler;
	
	assembler<<"It is the "<<( ourTurn ? "computer" : "human" )<<
		"'s turn and the board looks like: "<<endl;
	//print the marks contained in the board:
	for( int el=ELEMENTS-1; el>=0; --el )
	{
		assembler<<PRINTVBAR;
		for( vector< vector< char > >::const_iterator col=board.begin();
			col!=board.end();
			++col )
		{
			if( (unsigned)el<col->size() )
				assembler<<(*col)[el];
			else
				assembler<<PRINTHOLDER;
			assembler<<PRINTVBAR;
		}
	}
	//print the footer below the statie info:
	assembler<<PRINTFOOTER;
	for( unsigned int col=0; col<COLUMNS; ++col )
		assembler<<PRINTFOOTER<<PRINTFOOTER;
	//print the column numbers:
	assembler<<' ';
	for( unsigned int col=0; col<COLUMNS; ++col )
		assembler<<setw( 2 )<<col;
	assembler.flush();
	
	return assembler.str();
}

/** @brief Hashing */
int Connect3State::hash() const
{
	return hashCode;
}

/** @brief Same state? */
bool Connect3State::operator==( const Connect3State& another ) const
{
	if( this->MY_SYMBOL!=another.MY_SYMBOL || this->COLUMNS!=another.COLUMNS ||
		this->ELEMENTS!=another.ELEMENTS || this->ourTurn!=another.ourTurn ||
		this->board.size()!=another.board.size() )
		return false;
	
	for( unsigned int group=0; group<COLUMNS; ++group )
		if( this->board[group].size()!=another.board[group].size() ||
			!equal( this->board[group].begin(), this->board[group].end(),
			another.board[group].begin() ) )
			return false;
	
	return true;
}

/** @brief Assignment */
Connect3State& Connect3State::operator=( const Connect3State& another )
{
	if( this!=&another ) //don't copy over ourself
	{
		this->board=another.board;
		this->ourTurn=another.ourTurn;
		this->finalOutcome=another.finalOutcome;
		this->hashCode=another.hashCode;
	}
	
	return *this;
}

/** @brief Are these subsequent? */
bool Connect3State::areSubsequent( const Connect3State& first, const Connect3State&
	next )
{
	if( first.MY_SYMBOL==next.MY_SYMBOL || first.COLUMNS!=next.COLUMNS ||
		first.ELEMENTS!=next.ELEMENTS || first.ourTurn==next.ourTurn ||
		first.board.size()!=next.board.size() )
		return false;
	
	bool seenDifference=false;
	for( unsigned int group=0; group<first.COLUMNS; ++group )
	{
		if( first.board[group]>next.board[group] ||
			!equal( first.board[group].begin(), first.board[group].end(),
			next.board[group].begin() ) )
			return false;
		else if( first.board[group]<next.board[group] )
		{
			if( !seenDifference ) seenDifference=true;
			else return false; //more than one move was taken
		}
		//else they're exactly the same
	}
	
	return seenDifference;
}

/** @brief What just happened? */
int Connect3State::diff( const Connect3State& first, const
	Connect3State& next )
{
	bool subsequentStates=areSubsequent( first, next );
	
	assert( subsequentStates );
	for( unsigned int group=0; group<first.COLUMNS; ++group )
		if( first.board[group].size()!=next.board[group].size() )
			return group;
	
	assert( true ); //shouldn't have gotten here!
	return -1;
}

/** @brief Board-building check */
bool Connect3State::validChar( char character )
{
	return character==SYMBOLS[0] || character==SYMBOLS[1];
}

/** @brief Sorting */
void Connect3State::cacheHash()
{
	hashCode=( ourTurn ? 1 : 0 )<<COLUMNS*ELEMENTS;
	for( vector< vector< char > >::iterator col=board.begin(); col!=board.end(); ++col )
		for( vector< char >::iterator el=col->begin(); el!=col->end(); ++el )
			hashCode+=( *el )<<( ELEMENTS*( col-board.begin() )+( el-col->begin() ) );
}

/** @brief Compute winner */
Connect3State::Score Connect3State::computeWinner( int baseCol, int baseEl )
{
	if( baseEl==-1 ) baseEl=board[baseCol].size()-1;
	char match=board[baseCol][baseEl];
	
	if( baseCol!=-1 ) //only check things related to the move that was just made
	{
		for( int deltaCol=-1; deltaCol<=1; ++deltaCol )
			for( int deltaEl=-1; deltaEl<=1; ++deltaEl )
				for( int advance=1; advance<CONNECTABLE; ++advance )
				{
					int colIndex=baseCol+deltaCol*advance;
					int elIndex=baseEl+deltaEl*advance;
					
					if( colIndex<0 || (unsigned)colIndex>=board.size() || elIndex<0 || (unsigned)elIndex>=board[colIndex].size() || board[colIndex][elIndex]!=match )
						break; //give up moving in this direction
					else if( advance==CONNECTABLE-1 ) //&&board[colIndex][elIndex]==match
					{
						if( ourTurn ^ (match!=SYMBOLS[MY_SYMBOL]) )
							return LOSS;
						else
							return VICTORY;
					}
					//else we need more matches to make CONNECTABLE of them
				}
	}
	else //check the entire board
		for( unsigned int col=0; col<board.size(); ++col )
			for( unsigned int el=0; el<board[col].size(); ++el )
			{
				const Score& result=computeWinner( col, el );
				
				if( result!=TIE ) return result;
			}
	
	return TIE;
}
