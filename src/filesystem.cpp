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
	\file filesystem.cpp
	\brief Some file system management functions
	
	\author Thomas Maurice
	\version 0.2
*/

#include <misc.h>
#include <dirent.h>
#ifdef linux
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
#elif defined WIN32
	#include <windows.h>
#endif

using namespace std;

/**
	\brief Creates a directory
	Portable function to create a directory, will wotk on Linux/Mac/Windows
	
	\param[in] path the path to the directory to create
	
	\return The return value of the call to mkdir/CreateDirectory
*/
int make_directory(string path) {
		#ifdef linux
		return mkdir(path.c_str(), 0755);
	#elif defined WIN32
		return CreateDirectory(path.c_str(), NULL);
	#endif
}

/**
	\brief Check the existence of a directory
	
	\param[in] dir the path to the directory to check
	
	\return True if it exists, false otherwise
*/
bool directory_exists(string dir) {
	DIR* d = opendir(dir.c_str());
	if(d ==NULL) {
		closedir(d);
		return false;
	} else {
		closedir(d);
		return true;
	}
}
