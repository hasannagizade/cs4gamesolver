/** 
 * IntTest: Test class for the hashtable memoization
 *
 * @author Kyle Savarese <kms7341@rit.edu>
 * @author Sol Boucher <slb1566@rit.edu>
 */
#include "HashTable.h"
#include "Integer.cpp"
#include <cstddef>
#include <iostream>

int main() {
	HashTable<Integer> table;
	std::cout << table.size() << std::endl;
	for ( int i = 0; i < 102; i++ ) {
		Integer next( 2 * i, true );
		std::cout << next.hash() << std::endl;
		table.add( next, Integer( 2*i+1, true ) );
	}
	for ( int i = 0; i < 10; i++ ) {
		Integer next( i );
		std::cout << table.contains( next ) << std::endl;
	}
	std::cout << "SIZE: " << table.size() << std::endl;
	for ( int i = 0; i < 5; i++ ) {
		Integer next( 2 * i );
		table.remove( next );
	}
	std::cout << "SIZE: " << table.size() << std::endl;
	std::cout << "FOUND MATCH FOR 18: " << (table.matching( Integer(18)).hash() == 19) << std::endl;
	std::cout << "LITERAL 18: " << Integer( 18 ).isFlagged() << "\tSTORED 19: "<< table.matching( Integer( 18 ) ).isFlagged() << std::endl;
	std::cout << "200 NOT MISSING? " << table.contains( Integer(200) ) << std::endl;
	std::cout << "200 DEREF? " << table.matching( Integer(200) ).hash() << std::endl;
	table.purge();
	std::cout << "PURGED" << table.size() << std::endl;
}
