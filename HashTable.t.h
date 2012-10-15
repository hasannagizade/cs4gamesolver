/** @author Sol Boucher <slb1566@rit.edu> */
//included from "HashTable.h"

#include <cassert>

/** @brief Constructor */
template< class Content >
HashTable< Content >::HashTable():
	size( INITIAL_SIZE ) {}

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
	
	if( hashCode==table[targetIndex].hash() ) //the object is, in fact, at that index
		return targetIndex;
	else //use open addressing to find it
		//TODO loop (wrappnig if necessary) until we see either the object or NULL, or we've returned to targetIndex
			//in the former case, return the index
			//in the intermediate case, return one less than the additive inverse of the current index
			//in the latter case, return -size-1 ... since the table is full, add will have to expand it and copy all the pointers over
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
		//TODO resize the array and repopulate the new one (probably fair game for a separate private member function
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
	
	return table[index];
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
