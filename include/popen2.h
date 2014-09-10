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
	\file popen2.h
	\author Thomas Maurice
	\version 0.3
	\brief Nice popen2() implementation header
*/

#ifndef POPEN2_IMPLEM
#define POPEN2_IMPLEM

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define READ 0 //!< Input file descriptor (stdin)
#define WRITE 1 //!< Output file descriptor (stdout)
#define ERR 2 //!< Std error out file descriptor (stderr)

#ifdef __cplusplus
	extern "C" {
		pid_t popen2(const char *command, int *infp, int *outfp = NULL, int* errfp = NULL);
	}
#else
	pid_t popen2(const char *command, int *infp, int *outfp, int* errfp);
#endif

#endif

