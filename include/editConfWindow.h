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
	\file editConfWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the EditConfWindow class
*/

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <libxml++/libxml++.h>
#include <misc.h>

/**
	\class EditConfWindow
	\brief This class enables the user to edit the software configuration.
	
	The window looks like ![EditConfWindow window](confwindow.png)
*/

class EditConfWindow : public Gtk::Window {
	public:
		EditConfWindow(); //!< Constructor
		void loadCurrent(); //!< Loads the current configuration
		void save(); //!< Save the edited configuration
		~EditConfWindow(); //!< Destroys the window
		
	private:
		Gtk::VBox* layout; //!< Main layout
		Gtk::Entry* scriptDirEntry; //!< Entry for the Octave code
		Gtk::Entry* filesDirEntry; //!< Entry for the scriptinfo dir
		Gtk::Entry* hostEntry; //!< Entry for the remote host connection
		Gtk::Entry* userEntry; //!< Entry for the remote user connection
		Gtk::Button *saveButton; //!< Save button
		std::string homedir; //!< Home directory for the user
};
