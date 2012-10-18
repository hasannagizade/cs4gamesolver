/** @author Sol Boucher <slb1566@rit.edu> 
 *  @author Kyle Savarese <kms7341@rit.edu>
 */
//included from "HashTable.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>
using namespace std;

/** @brief Constructor */
template< class Content >
HashTable< Content >::HashTable():
	_size( INITIAL_SIZE ), table( new pair< Content, Content >*[INITIAL_SIZE] )
{
	for( int index=0; index<_size; ++index )
		table[index]=NULL;
}

/** @brief Destructor */
template< class Content >
HashTable< Content >::~HashTable()
{
	purge();
	delete[] table;
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
		if( table[targetIndex]->first==object ) //the supplied object is, in fact, at that index
			return targetIndex;
		else //use open addressing to find it
		{
			for( int _index=(targetIndex+1)%_size; _index!=targetIndex; _index=(_index+1)%_size )
			{
				if( table[_index]==NULL ) //found a spot
					return -_index-1;
				else if( table[_index]->first==object ) //found what we're looking for
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
	pair< Content, Content >** oldTable=table;
	
	_size=oldSize*GROWTH_FACTOR;
	table=new pair< Content, Content >*[_size];
	
	for( int _index=0; _index<_size; ++_index )
		table[_index]=NULL;
	
	for( int oldIndex=0; oldIndex<oldSize; ++oldIndex ) {
		pair< Content, Content >* oldData = oldTable[oldIndex];
		table[-index( oldData->first )-1] = oldData;
	}
	
	delete[] oldTable;
}

/** @brief Adds an element */
template< class Content >
void HashTable< Content >::add( const Content& key, const Content& value )
{
	int _index=index( key );
	std::cout << "INDEX: " << _index << std::endl;	
	assert(_index<0 ); //not already present
	if( _index>=0 ) return;
	
	_index=-_index-1;
	if( _index==_size ) {//out of space
		std::cout<<"old size: "<<_size<<std::endl;
		grow();
		std::cout<<"new size: "<<_size<<std::endl;
		std::cout<<"red size: "<<_index<<std::endl;
		_index = -index( key )-1;
		std::cout<<"blue size: "<<_index<<std::endl;
	}
	table[_index]=new pair< Content, Content >( Content( key ), Content( value ) );
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
	
	return table[_index]->second;
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
	table[_index]=NULL;
	
	//slide everything displaced by this element down:
	for( int checkIndex=( _index+1 )%_size; table[checkIndex]!=NULL && checkIndex!=_index; checkIndex=( checkIndex+1 )%_size )
	{
		int idealLocation=index( table[checkIndex]->first );
		
		if( idealLocation<0 ) //can move to a better place
		{
			table[-idealLocation-1]=table[checkIndex];
			table[checkIndex]=NULL;
		}
	}
	
	return true;
}

/** @brief Hose it all */
template< class Content >
void HashTable< Content >::purge()
{
	for( int _index=0; _index<_size; ++_index )
		if( table[_index]!=NULL )
		{
			std::cout<<"Index "<<_index<<" Key "<<( table[_index]->first ).hash()<<" Value "<<( table[_index]->second ).hash()<<std::endl;
			delete table[_index];
			table[_index]=NULL;
		}
}
