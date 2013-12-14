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
