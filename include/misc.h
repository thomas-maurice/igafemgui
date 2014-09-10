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
	\file misc.h
	\author Thomas Maurice
	\version 0.3
	\brief Some miscellaneous functions
*/

#ifndef MISC
#define MISC
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <popen2.h>

extern "C" {
	// Because better locale-independant implementation is better
	// the n stands for "new"
	double nstrtod(const char* string, char** endPtr); //!< Locale-independant strtod
	bool is_process_alive(pid_t pid);
}
int make_directory(std::string path);
std::string random_string(int len, bool majs=true, bool num=true); //!< Generates a random string
bool directory_exists(std::string dir);
#endif
