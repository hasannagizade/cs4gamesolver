/*
 * Copyright (C) 2012 Sol Boucher and Kyle Savarese
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @author Sol Boucher <slb1566@rit.edu> 
 *  @author Kyle Savarese <kms7341@rit.edu>
 */
//included from "HashTable.h"
#include <cassert>
#include <cstddef>
#include <new>
#include <utility>

/** @brief Constructor */
template< class Content >
HashTable< Content >::HashTable():
	_size( INITIAL_SIZE ), table( new std::pair< Content,
	Content >*[INITIAL_SIZE] )
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
	
	if( table[targetIndex]==NULL ) //the requested index is free for the
		//taking
		return -targetIndex-1;
	else //there's something at the requested location
	{
		if( table[targetIndex]->first==object ) //the supplied object
			//is, in fact, at that index
			return targetIndex;
		else //use open addressing to find it
		{
			for( int _index=(targetIndex+1)%_size;
				_index!=targetIndex; _index=(_index+1)%_size )
			{
				if( table[_index]==NULL ) //found a spot
					return -_index-1;
				else if( table[_index]->first==object )
					 //found what we're looking for
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
	std::pair< Content, Content >** oldTable=table;
	
	_size=oldSize*GROWTH_FACTOR;
	try
	{
		table=new std::pair< Content, Content >*[_size];
	}
	catch( const std::bad_alloc& noExceptions )
	{
		_size=oldSize;
	}
	
	for( int _index=0; _index<_size; ++_index )
		table[_index]=NULL;
	
	for( int oldIndex=0; oldIndex<oldSize; ++oldIndex ) {
		std::pair< Content, Content >* oldData = oldTable[oldIndex];
		table[-index( oldData->first )-1] = oldData;
	}
	
	delete[] oldTable;
}

/** @brief Adds an element */
template< class Content >
bool HashTable< Content >::add( const Content& key, const Content& value )
{
	try
	{
		int _index=index( key );
		assert(_index<0 ); //not already present
		if( _index>=0 ) return false;
		
		_index=-_index-1;
		if( _index==_size ) {//out of space
			grow();
			_index = -index( key )-1;
		}
		table[_index]=new std::pair< Content, Content >(
			 Content( key ), Content( value ) );
	}
	catch( const std::bad_alloc& noExceptions )
	{
		return false;
	}
	
	return true;
}

/** @brief Contains an element? */
template< class Content >
bool HashTable< Content >::contains( const Content& object ) const
{
	return index( object )>=0;
}

/** @brief Retrieves a copy of our own copy */
template< class Content >
void HashTable< Content >::matching( const Content& object, Content& result ) const
{
	int _index=index( object );
	
	assert( _index>=0 ); //present in table
	
	result=table[_index]->second;
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
	for( int checkIndex=( _index+1 )%_size; table[checkIndex]!=NULL &&
		checkIndex!=_index; checkIndex=( checkIndex+1 )%_size )
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
			delete table[_index];
			table[_index]=NULL;
		}
}
