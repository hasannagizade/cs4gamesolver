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
	_size( INITIAL_SIZE ), table( new Content*[INITIAL_SIZE] ) {}

/** @brief Destructor */
template< class Content >
HashTable< Content >::~HashTable()
{
	purge();
	//delete[] table; //TODO if it grew, it crashes here ... but does it leak without?
	table=NULL;
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
		if( *table[targetIndex]==object ) //the supplied object is, in fact, at that index
			return targetIndex;
		else //use open addressing to find it
		{
			for( int _index=(targetIndex+1)%_size; _index!=targetIndex; _index=(_index+1)%_size )
			{
				if( table[_index]==NULL ) //found a spot
					return -_index-1;
				else if( *table[_index]==object ) //found what were looking for
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
	int oldSize=_size;
	Content** oldTable=table;
	
	_size=oldSize*GROWTH_FACTOR;
	table=new Content*[_size];
	
	for( int oldIndex=0; oldIndex<oldSize; ++oldIndex ) {
		Content* oldData = oldTable[oldIndex];
		table[-index( *oldData )-1] = oldData;
	}
	
	delete[] oldTable;
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
		_index = -index( object )-1;
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
int HashTable< Content >::size( void ) const
{
	int found=0;
	
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
	for( int _index=0; _index<_size; ++_index )
		if( table[_index]!=NULL )
		{
			std::cout<<"Index "<<_index<<" Value "<<table[_index]<<std::endl;
			delete table[_index];
			table[_index]=NULL;
		}
}
