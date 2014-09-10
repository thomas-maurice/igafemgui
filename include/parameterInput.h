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
	\file parameterInput.h
	\version 0.2
	\author Thomas Maurice
	\brief Defines the ParameterInput class
*/

#include <gtkmm.h>
#include <scriptDescriptor.h>

/**
	\class ParameterInput
	\brief This class's purpose is to diplay a parameter and enable the user to change it's value
*/

class ParameterInput : public Gtk::HBox {
	public:
		ParameterInput(Parameter p); //!< Creates a new ParameterInput from a Parameter
		Parameter get(); //!< Returns the current Parameter (edited)
	
	private:
		Gtk::SpinButton* value; //!< Current value of the Parameter
		Parameter param; //!< Current Parameter
};
