//Version history in git-log.txt
#ifndef CONNECT3HELPER_H
#define CONNECT3HELPER_H

#include <istream>
#include <vector>

/**
Contains ONLY ONE helper method for the Connect3 main.  This is considered by
	some theorists as a general waste of time, effort, and space.

@author Sol Boucher <slb1566@rit.edu>
*/
class Connect3Helper
{
	public:
		/**
		Reads the encoded board <tt>save</tt> into <tt>use</tt>,
			determining its height in the process.
		@param save the encoded board
		@param use the decoded board
		@param height the number of elements per row
		@return whether the process succeeded
		*/
		static bool decodeBoard( std::istream& save, std::vector<
			std::vector< char > >& use, int& height );
};

#endif
