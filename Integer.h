/**
Immutable <tt>int</tt> wrapper class for testing <tt>HashTable</tt>

@author Sol Boucher <slb1566@rit.edu>
*/
#ifndef INTEGER_H
#define INTEGER_H

class Integer
{
	private:
		/** The value */
		const int value;
		
		/** Special use */
		const int flag;
	
	public:
		/**
		Creates a new <tt>Integer</tt>, setting its value.
		@param value the value to set
		@param flag whether this instance is personally significant
		*/
		Integer( int value, bool flag=false );
		
		/**
		Copy constructor
		@param instance to copy
		*/
		Integer( const Integer& instance );
		
		/**
		Destroys an <tt>Integer</tt>, leaving no survivors
		*/
		~Integer( void );
		
		/**
		The hash function
		@return the <tt>value</tt> itself
		*/
		int hash( void ) const;
		
		/**
		The identity check
		@param another <tt>Integer</tt>
		@return whether the <tt>value</tt>s are the same
		*/
		bool operator==( const Integer& another ) const;
		
		/**
		Check the <tt>flag</tt>.
		@return whether we're <tt>flag</tt>ged
		*/
		bool isFlagged( void ) const;
};

#endif
