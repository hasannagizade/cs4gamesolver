/** @author Sol Boucher <slb1566@rit.edu> 
 *  @author Kyle Savarese <kms7341@rit.edu>
 */
//included from "HashTable.h"

#include <cassert>
#include <cstddef>
#include <iostream>

/** @brief Constructor */
template< class Content >
HashTable< Content >::HashTable():
	_size( INITIAL_SIZE ), table() {}

/** @brief Destructor */
template< class Content >
HashTable< Content >::~HashTable()
{
	purge();
}

/** @brief Find the index or intended index */
template< class Content >
int HashTable< Content >::index( const Content& object ) const
{
	int hashCode=object.hash(), targetIndex=hashCode%_size;
	
	if( table[targetIndex]==NULL ) //the requested index is free for the taking
		return -targetIndex-1;
	else //there's something at the requested location
	{
		if( hashCode==table[targetIndex]->hash() ) //the supplied object is, in fact, at that index
			return targetIndex;
		else //use open addressing to find it
		{
			for( int _index=(targetIndex+1)%_size; _index!=targetIndex; _index=(targetIndex+1)%_size )
			{
				if( table[_index]==NULL ) //found a spot
					return -_index-1;
				else if( hashCode==table[_index]->hash() ) //found what were looking for
					return _index;
			}
			
			return -_size-1;
		}
	}
}

/** @brief Expands the table */
template< class Content >
void HashTable< Content >::grow()
{
	unsigned int oldSize=_size;
	Content* oldTable=*table;
	
	_size=oldSize*GROWTH_FACTOR;
	table[_size];
	
	for( int oldIndex=0; oldIndex<oldSize; ++oldIndex ) {
		Content oldData = (oldTable[oldIndex]);
		table[-index( ( oldTable[oldIndex] ) )-1] = new Content(oldData);
	}
}

/** @brief Adds an element */
template< class Content >
void HashTable< Content >::add( const Content& object )
{
	int _index=index( object );
	std::cout << "INDEX: " << _index << std::endl;	
	assert(_index<0 ); //not already present
	if( _index>=0 ) return;
	
	_index=-_index-1;
	if( _index==_size ) {//out of space
		grow();
		_index = index( object );
	}
	table[_index]=new Content(object);
}

/** @brief Contains an element? */
template< class Content >
bool HashTable< Content >::contains( const Content& object ) const
{
	return index( object )>=0;
}

/** @brief Retrieves a copy of our own copy */
template< class Content >
Content HashTable< Content >::matching( const Content& object ) const
{
	int _index=index( object );
	
	assert( _index>=0 ); //present in table
	
	return *table[_index];
}

/** @brief Current *utilized* size */
template< class Content >
unsigned int HashTable< Content >::size( void ) const
{
	unsigned int found=0;
	
	for( int _index=0; _index<_size; ++_index )
		if( table[_index]!=NULL )
			++found;
	
	return found;
}

/** @brief Nuke our copy */
template< class Content >
bool HashTable< Content >::remove( const Content& object )
{
	int _index=index( object );
	
	if( _index<0 ) return false; //didn't find it
	
	delete table[_index];
	return true;
}

/** @brief Hose it all */
template< class Content >
void HashTable< Content >::purge()
{
	for( unsigned int _index=0; _index<_size; ++_index )
		if( table[_index]!=NULL )
			delete table[_index];
}
