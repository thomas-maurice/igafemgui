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
	\file editableParameterSet.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the EditableParameterSet class
*/

#include <gtkmm.h>
#include <vector>
#include <editableParameterInput.h>
#include <scriptDescriptor.h>

/**
 \class EditableParameterSet
 \brief Layout listing all the parameter of a script. Also manage the add and the removing of the parameters
 
 This class is used to list and to manage the parameter lists of a script. It can
 add and remove some parameters, and finally return them all in a vector.
*/
class EditableParameterSet : public Gtk::VBox {
	public:
		EditableParameterSet(); //!< Constructor
		void clear(); //!< Delete all the registered parameters
		void addInput(Parameter p); //!< Add a parameter
		void delInput(int id); //!< Destroys a parameter
		std::vector<Parameter> get(); //!< Returns the parameter list
		bool isValid(); //!< Test if an input is valid **unused now**
	
	private:
		Gtk::VBox *layout; //!< The main layout
		int curId; //!< The current ID which will be used to register the next parameter
		std::vector<EditableParameterInput*> inputs; //!< List of the currently registered parameters
};
