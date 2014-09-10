/*
 *
 *	Copyright (C) 2013 Thomas MAURICE <tmaurice59@gmail.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
 
/**
	\file misc.cpp
	\brief Miscellaneous functions
	
	\author Thomas Maurice
	\version 0.1
*/

#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

/**
	Generates a random string
	\param[in] len Length of the string
	\param[in] majs Do we use majs ?
	\param[in] num Do we use digits ?
*/
std::string random_string(int len, bool majs, bool num) {
	srand(time(NULL));
	string min = "azertyuiopqsdfghjklmwxcvbn";
	string maj = "AZERTYUIOPQSDFGHJKLMWXCVBN";
	string cf = "0123456789";
	string res = "";
	
		if(majs)
		min += maj;
	
		if(num)
		min += cf;
	
    const int MAX = min.length() - 1, MIN = 0;

	for(int i = 0; i < len ; i++) {
		int in = (rand() % (MAX - MIN + 1)) + MIN;
		res += min.at(in);
	}
	
	return res;
}
