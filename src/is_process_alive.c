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
	\file is_process_alive.c
	\brief System call based function to determine if a pid is alive
	
	\author Thomas Maurice
	\version 0.2
*/

#ifdef __cplusplus
extern "C" {
#	include <stdlib.h>
#	include <signal.h>
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/wait.h>
}
#else
#	include <stdlib.h>
#	include <signal.h>
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/wait.h>
#	include <stdbool.h>
#endif

/**
	\brief System call based function to determine if a pid is alive
	
	\param[in] pid the pid to check
	
	\return true if the process id alive, false otherwise
*/
bool is_process_alive(pid_t pid) {
	// If we are the parent, let's reap the children if it's dead
	waitpid(pid, NULL, WNOHANG);
	if(kill(pid, 0) == 0) {
		return true;
	} else {
		return false;
	}
}
