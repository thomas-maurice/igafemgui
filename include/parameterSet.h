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
	\file parameterSet.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the ParameterSet class
*/

#include <gtkmm.h>
#include <vector>
#include <parameterInput.h>
#include <scriptDescriptor.h>

/**
	\class ParameterSet
	\brief Layout-based class which containts all the ParameterInput
*/

class ParameterSet : public Gtk::VBox {
	public:
		ParameterSet(); //!< Creates a new ParameterSet
		void clear(); //!< Clears the ParameterSet
		void addInput(Parameter p); //!< Add a ParameterInput
		std::vector<Parameter> get(); //!< Return the list of the Parameters
	
	private:
		Gtk::VBox *layout; //!< Main layout
		std::vector<ParameterInput*> inputs; //!< ParameterInput list
};
