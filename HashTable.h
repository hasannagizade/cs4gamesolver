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

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <utility>

/**
A hash table implementation to store copies of objects providing a
<tt>public</tt> copy constructor and supporting the <tt>public int hash(void)
const</tt> and <tt>public operator==(const Content&) const</tt> methods.

@author Sol Boucher <slb1566@rit.edu>
@author Kyle Savarese <kms7341@rit.edu>
*/
template< class Content >
class HashTable
{
	private:	
		/** The table's initial size */
		static const int INITIAL_SIZE=100;
		
		/** The table's growth factor */
		static const int GROWTH_FACTOR=2;
		
		/** The array's current size */
		int _size;
		
		/** The array of member objects */
		std::pair< Content, Content >** table;
		
		/**
		Finds the index occupied by the specified value.
		@param object the value for which to search
		@return the corresponding index, the additive inverse of one
			more than the would-be index if the object isn't
			present, or <tt>-size-1</tt> if the table is full
		*/
		int index( const Content& object ) const;
	
		/**
		Enlarges the table to hold more elements.
		*/
		void grow( void );
	
	public:
		/**
		Create a <tt>HashTable</tt>.
		*/
		HashTable( void );
		
		/**
		Destroys a <tt>HashTable</tt>.
		@post All the table's copies of the objects have been
			destroyed.
		*/
		~HashTable( void );
		
		/**
		Copies a pair of objects into the table.
		@param key the keying object
		@param value the referred object
		@return whether the operation succeeded
		@pre No identical <tt>key</tt> is already in the table.
		*/
		bool add( const Content& key, const Content& value );
		
		/**
		Checks whether a key is in the table.
		@param object the key for which to search
		@return whether the object was found
		*/
		inline bool contains( const Content& object ) const;
		
		/**
		Retrieves a copy of the table's value corresponding to the
			specified key.
		@param object the caller's copy of a key
		@return result the match
		@pre The table contains a copy of <tt>object</tt>.
		*/
		inline void matching( const Content& object, Content& result ) const;
		
		/**
		Determines the current number of objects stored in the table.
		*/
		int size( void ) const;
		
		/**
		Removes the specified key and the value corresponding to it.
		@param object the key to remove
		@return whether the object was found
		*/
		bool remove( const Content& object );
		
		/**
		Empties the table of all its entries.
		@post All the table's copies of the objects have been
			destroyed.
		*/
		void purge( void );
};

#include "HashTable.t.h"

#endif
