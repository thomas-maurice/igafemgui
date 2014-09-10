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
	\file editableParameterInput.h
	\version 0.2
	\author Thomas Maurice
	\brief Defines the EditableParameterInput class
*/

#include <gtkmm.h>
#include <scriptDescriptor.h>
#include <editableParameterWindow.h>
#include <iostream>
#include <sstream>

/**
	\class EditableParameterInput
	\brief This field represents an interface between the user and the Parameter structure
	
	It enables the user to edit every aspect of a Parameter variable by setting its
	name, its description, its boundaries, increment and default value. As you can see
	it possesses a signal_delete which is very conveinient to adjust the number of
	parameters you can find in an EditableParameterSet.
	
	The edition is done by opening an EditableParameterWindow and getting its value.
*/

class EditableParameterInput : public Gtk::HBox {
	public:
		EditableParameterInput(Parameter p, int i); //!< Constructor
		Parameter get(); //!< Returns the current value of the parameter.
		void edit(); //!< Calls the EditableParameterWindow
		int getId(); //!< Returns the ID of the parameter
		sigc::signal<void> signal_delete; //!< The delete signal used by its parent
		std::string getName(); //!< The parameter name
		
	private:
		Gtk::Button *delButton; //!< "delete" button
		Gtk::Button *editButton; //!< "edit" button
		Gtk::Label *name; //!< Name label
		Gtk::Label *desc; //!< Description label
		int id; //!< Parameter ID, used only by its parent
		Parameter param; //!< Parameter object
};
