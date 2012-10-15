/**
A hash table implementation to store copies of objects providing a <tt>public</tt> copy constructor and supporting the <tt>public int hash(void) const</tt> method.

@author Sol Boucher <slb1566@rit.edu>
*/
#ifndef HASHTABLE_H
#define HASHTABLE_H

template< class Content >
class HashTable
{
	private:
		/** The array's current size */
		unsigned int size;
		
		/** The table of member objects */
		Content* table[];
		
		/**
		Finds the index occupied by the specified object.
		@param object the instance for which to search
		@return the corresponding index, the additive inverse of one more than the would-be index if the object isn't present, or <tt>-size-1</tt> if the table is full
		*/
		int index( const Content& object ) const;
	
	public:
		/** The table's initial size */
		static const unsigned int INITIAL_SIZE;
		
		/**
		Create a <tt>HashTable</tt>.
		*/
		HashTable( void );
		
		/**
		Destroys a <tt>HashTable</tt>.
		@post All the table's copies of the objects have been destroyed.
		*/
		~HashTable( void );
		
		/**
		Copies an object into the table.
		@param object the instance to add
		@pre No version of this instance is already in the table.
		*/
		void add( const Content& object );
		
		/**
		Checks whether an object is in the table.
		@param object the instance for which to search
		@return whether the object was found
		*/
		bool contains( const Content& object ) const;
		
		/**
		Retrieves a copy of the table's version of the specified object.
		@param object the caller's instance
		@pre The table contains a version of <tt>object</tt>.
		*/
		Content matching( const Content& object ) const;
		
		/**
		Determines the current number of objects stored in the table.
		*/
		unsigned int size( void ) const;
		
		/**
		Removes the table's copy of the specified object.
		@param object the object to remove
		@return whether the object was found
		*/
		bool remove( const Content& object );
		
		/**
		Empties the table of all its entries.
		@post All the table's copies of the objects have been destroyed.
		*/
		void purge( void );
};

#include "HashTable.t.h"

#endif
