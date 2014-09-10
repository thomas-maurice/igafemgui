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
	\file aboutWindow.cpp
	\author Thomas Maurice
	\version 0.1
	
	Implements the AboutWindow dialog.
*/
#include <aboutWindow.h>

/**
	A simple constructor that displays information about
	the authors and run the window.
*/
AboutWindow::AboutWindow() {
	set_program_name("igafemgui");
	set_version("0.2");
	set_copyright("© 2013");
	set_comments("igafemgui is a program aimed to make more easy the use of \
Octave code under Linux. It enables you to edit parameters that will have \
an influence on the simulation without actually edit the files, preventing \
you from damaging them.");
	
	set_license("igafemgui  Copyright (C) 2013 Thomas Maurice <tmaurice59@gmail.com>\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it\n\
under certain conditions; see the GNU/GPL license for details.\n");

	std::vector<Glib::ustring> listAuthors;
	listAuthors.push_back("Thomas Maurice <tmaurice59@gmail.com>");
	set_authors(listAuthors);
	
	show_all();
}
