/** 
 * @author: Kyle savarese <kms7341@rit.edu>
 IntTest: Test class for the hashtable memoization
 */

#include "HashTable.h"
#include "Integer.cpp"
#include <cstddef>
#include <iostream>

int main() {
	HashTable<Integer> table;
	std::cout << table.size() << std::endl;
	for ( int i = 0; i < 10; i++ ) {
		Integer next( 2 * i, true );
		std::cout << next.hash() << std::endl;
		table.add( next );
	}
	for ( int i = 0; i < 10; i++ ) {
		Integer next( i );
		std::cout << table.contains( next ) << std::endl;
	}
	//std::cout << "SIZE: " << table.size() << std::endl;
	for ( int i = 0; i < 5; i++ ) {
		Integer next( 2 * i );
		table.remove( next );
	}
	//std::cout << "SIZE: " << table.size() << std::endl;
	std::cout << "FOUND MATCH FOR 18: " << (table.matching( Integer(18)).hash() == 18) << std::endl;
	table.purge();
	std::cout << "PURGED" << table.size() << std::endl;
}
