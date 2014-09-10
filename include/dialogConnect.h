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
	\file dialogConnect.h
	\brief Definition of the dialogConnect class
	
	\author Thomas Maurice
	\version 0.1
*/

/**
	\class DialogConnect
	\brief Ask the user for connection information
	
	This window look like that :
	![Log Window](netlog.png)
*/

#include <iostream>
#include <gtkmm.h>

class DialogConnect : public Gtk::Dialog {
	public:
		DialogConnect(std::string *user, std::string *pw, std::string *host, int *port); //!< Basic constructor
		void onOk(); //!< Called when the user hits the button
	private:
		// Graphical stuff
		Gtk::Entry *userEntry; //!< Entry for the user's name
		Gtk::Entry *passEntry; //!< Entry for the user's password
		Gtk::Entry *hostEntry; //!< Entry for the server's host
		Gtk::SpinButton *portSpinButton; //!< Spin button port the port
		Gtk::Button *okButton; //!< Button used to validate the info
		
		// Attributes
		std::string *user; //!< Pointer on the username
		std::string *host; //!< Pointer on the host
		std::string *pass; //!< Pointer on the password
		int *port; //!< Pointer on the host
};
