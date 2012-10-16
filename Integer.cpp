/** @author Sol Boucher <slb1566@rit.edu> */
#include "Integer.h"

/** Implicit constructor */
Integer::Integer( int value, bool flag ):
	value( value ), flag( flag ) {}

/** Copy constructor */
Integer::Integer( const Integer& instance ):
	value( instance.value ), flag( instance.flag ) {}

/** Destructor */
Integer::~Integer() {}

/** Hasher */
int Integer::hash() const
{
	return value;
}

/** Identity */
bool Integer::operator==( const Integer& another ) const
{
	return this->value==another.value;
}

/** Flag check */
bool Integer::isFlagged() const
{
	return flag;
}
