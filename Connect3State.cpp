/** @author Sol Boucher <slb1566@rit.edu> */
#include "Connect3State.h"
#include <cassert>
#include <climits>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
using namespace std;

/** @brief Board symbols */
const char Connect3State::SYMBOLS[2]={'X', 'O'};

/** @brief Constructor */
Connect3State::Connect3State( unsigned int columnCount,
	unsigned int elementCount,
	const std::vector< std::vector< char > >& original, bool weAreUp ):
	COLUMNS( columnCount ), ELEMENTS( elementCount ),
	mySymbol( 0 ), board( original ), ourTurn( weAreUp) 
{
	assert( board.size()==COLUMNS );
	for( vector< vector< char > >::iterator col=board.begin();
		col!=board.end(); ++col )
		assert( col->size()<=ELEMENTS );
	
	finalOutcome=computeWinner();
	cacheHash();
}

/** @brief Advancing constructor */
Connect3State::Connect3State( const Connect3State& baseState,
	unsigned int column ):
	COLUMNS( baseState.COLUMNS ), ELEMENTS( baseState.ELEMENTS ),
	mySymbol( 1-baseState.mySymbol ), board( baseState.board ),
	ourTurn ( !baseState.ourTurn )
{
	assert( column<board.size() );
	board[column].push_back( SYMBOLS[1-mySymbol] ); //the other player placed
		//it!
	assert( board[column].size()<=ELEMENTS );
	
	finalOutcome=computeWinner( column );
	cacheHash();
}

/** @brief Destructor */
Connect3State::~Connect3State() {}

/** @brief Are we out of objects? */
bool Connect3State::gameOver() const
{
	bool boardFull=true;
	
	for( vector< vector< char > >::const_iterator column=board.begin();
		column!=board.end(); ++column )
		if( column->size()<ELEMENTS )
		{
			boardFull=false; //column not complete
			break;
		}
	
	return boardFull || finalOutcome!=TIE; //has anyone won?
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

/** @brief Will it fit? */
bool Connect3State::hasSpaceAt( unsigned int column ) const
{
	assert( column<COLUMNS );
	
	return board[column].size()<ELEMENTS; //room to grow
}

/** @brief What might happen next? */
const vector< Connect3State > Connect3State::successors() const
{
	vector< Connect3State > possibilities;
	for( unsigned int column=0; column<board.size(); ++column )
		if( board[column].size()<ELEMENTS )
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
			if( unsigned( el )<col->size() )
				assembler<<(*col)[el];
			else
				assembler<<PRINTHOLDER;
			assembler<<PRINTVBAR;
		}
		assembler<<endl;
	}
	//print the footer below the statie info:
	for( unsigned int col=0; col<COLUMNS; ++col )
		assembler<<PRINTFOOTER<<PRINTFOOTER;
	assembler<<PRINTFOOTER<<endl;
	//print the column numbers:
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
	if( this->mySymbol!=another.mySymbol || this->COLUMNS!=another.COLUMNS ||
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
	assert( this->COLUMNS==another.COLUMNS );
	assert( this->ELEMENTS==another.ELEMENTS );
	
	if( this!=&another ) //don't copy over ourself
	{
		this->mySymbol=another.mySymbol;
		this->board=another.board;
		this->ourTurn=another.ourTurn;
		this->finalOutcome=another.finalOutcome;
		this->hashCode=another.hashCode;
	}
	
	return *this;
}

/** @brief Are these subsequent? */
bool Connect3State::areSubsequent( const Connect3State& first,
	const Connect3State& next )
{
	if( first.mySymbol==next.mySymbol || first.COLUMNS!=next.COLUMNS ||
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
	for( vector< vector< char > >::iterator col=board.begin();
		col!=board.end(); ++col )
		for( vector< char >::iterator el=col->begin(); el!=col->end(); ++el )
			hashCode+=( *el )<<( ELEMENTS*( col-board.begin() )
				+( el-col->begin() ) );
	hashCode=abs( hashCode );
	assert( hashCode>=0 );
}

/** @brief Compute winner */
Connect3State::Score Connect3State::computeWinner( int baseCol, int baseEl )
{
	if( baseCol!=-1 ) //only check things related to the move that was just
		//made
	{
		if( baseEl==-1 ) baseEl=board[baseCol].size()-1;
		char match=board[baseCol][baseEl];
		
		for( int deltaCol=-1; deltaCol<=1; ++deltaCol )
			for( int deltaEl=-1; deltaEl<=1; ++deltaEl )
				if( deltaCol!=0 || deltaEl!=0 )
					{
						int inARow=0;
						for( int advance=-CONNECTABLE+1; advance<CONNECTABLE;
							++advance )
						{
							int colIndex=baseCol+deltaCol*advance;
							int elIndex=baseEl+deltaEl*advance;
							
							#ifdef DEBUG
								cout<<'('<<colIndex<<','<<elIndex<<')'<<endl;
							#endif
							
							++inARow;
							
							if( colIndex<0 || unsigned( colIndex )>=board.size()
								|| elIndex<0 ||
								unsigned( elIndex )>=board[colIndex].size() ||
								board[colIndex][elIndex]!=match )
								inARow=0; //here's a discontinuity
							else if( inARow==CONNECTABLE )
								//&& board[colIndex][elIndex]==match
							{
								#ifdef DEBUG
									cout<<"Game ovah, bitches!"<<endl;
								#endif
								
								if( ourTurn ^ (match!=SYMBOLS[mySymbol]) )
									return VICTORY;
								else
									return LOSS;
							}
							//else we need more matches to make CONNECTABLE of
								//them
						}
					}
	}
	else //check the entire board
		for( unsigned int col=0; col<board.size(); ++col )
			for( unsigned int el=0; el<board[col].size(); ++el )
			{
				#ifdef DEBUG
					cout<<'<'<<col<<','<<el<<'>'<<endl;
				#endif
				
				const Score& result=computeWinner( col, el );
				
				if( result!=TIE ) return result;
			}
	
	return TIE;
}
