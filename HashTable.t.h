/** @author Sol Boucher <slb1566@rit.edu> */
//included from "HashTable.h"

#include <cassert>

/** @brief Constructor */
template< class Content >
HashTable< Content >::HashTable():
	size( INITIAL_SIZE ), table() {}

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
	int hashCode=object.hash(), targetIndex=hashCode%size;
	
	if( hashCode==table[targetIndex]->hash() ) //the object is, in fact, at that index
		return targetIndex;
	else //use open addressing to find it
	{
		for( int index=(targetIndex+1)%size; index!=targetIndex; index=(targetIndex+1)%size )
		{
			if( table[index]==NULL ) //found a spot
				return -index-1;
			else if( hashCode==table[index]->hash() ) //found what were looking for
				return index;
		}
		
		return -size-1;
	}
}

/** @brief Expands the table */
template< class Content >
void HashTable< Content >::grow()
{
	int newSize=size*GROWTH_FACTOR;
	Content* table[newSize];
	
	//TODO resize the array and repopulate the new one
}

/** @brief Adds an element */
template< class Content >
void HashTable< Content >::add( const Content& object )
{
	int index=index( object );
	
	assert( index<0 ); //not already present
	if( index>=0 ) return;
	
	index=-index-1;
	if( index==size ) //out of space
		//TODO call grow, then index again, then place the copy
	else
		table[index]=new Content(object);
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
	int index=index( object );
	
	assert( index>=0 ); //present in table
	
	return *table[index];
}

/** @brief Current *utilized* size */
template< class Content >
unsigned int HashTable< Content >::size( void ) const
{
	unsigned int found=0;
	
	for( int index=0; index<size; ++index )
		if( table[index]!=NULL )
			++found;
	
	return found;
}

/** @brief Nuke our copy */
template< class Content >
bool HashTable< Content >::remove( const Content& object )
{
	int index=index( object );
	
	if( index<0 ) return false; //didn't find it
	
	delete table[index];
	return true;
}

/** @brief Hose it all */
template< class Content >
void HashTable< Content >::purge()
{
	for( unsigned int index=0; index<size; ++index )
		if( table[index]!=NULL )
			delete table[index];
}
