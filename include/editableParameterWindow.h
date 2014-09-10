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
	\file editableParameterWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the EditableParameterWindow class
*/
#include <gtkmm.h>
#include <scriptDescriptor.h>
#include <iostream>
#include <sstream>
/**
	\class EditableParameterWindow
	\brief This window enables the user to set every option in the parameter
*/
class EditableParameterWindow : public Gtk::Dialog {
	public:
		EditableParameterWindow(Parameter *p); //!< Default constructor
		Parameter get(); //!< Returns the parameter
		bool isNumber(std::string s); //!< Test if a string is a number
		bool isInputValid(); //!< Test if an input is valid
		void save(); //!< Save the current parameter
		std::string getName(); //!< Returns the parameter name
		
	private:
		Gtk::Table *mainLayout; //!< Main layout
		Gtk::Button *saveButton; //!< --
		sigc::signal<void> signal_save; //!< Save signal
		Gtk::Entry  *nameEntry; //!< Entry
		Gtk::Entry  *descEntry; //!< Entry
		Gtk::Entry  *minEntry; //!< Entry
		Gtk::Entry  *maxEntry; //!< Entry
		Gtk::Entry  *incEntry; //!< Entry
		Gtk::Entry  *valEntry; //!< Entry
		Parameter *param; //!< Parameter
};
