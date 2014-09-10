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
	\file parameterSet.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the ParameterSet class
*/

#include <parameterSet.h>

/**
	Creates a new ParameterSet, not much to say about this
*/
ParameterSet::ParameterSet() {
	
}

/**
	Remove all the ParameterInput from the ParameterSet
*/
void ParameterSet::clear() {
	for(unsigned int i = 0; i < inputs.size(); i++) {
		inputs.at(i)->~ParameterInput();
	}
	inputs.clear();
}

/**
	Returns in a std::vector all the Parameter defined and edited by the user
	into the ParameterInput of the window
*/
std::vector<Parameter> ParameterSet::get() {
	std::vector<Parameter> p;
	for(unsigned int i = 0; i < inputs.size(); i++) {
		p.push_back(inputs.at(i)->get());
	}
	return p;
}

/**
	Adds a ParameterInput in the layout, it will be built from the given Parameter
	
	\param[in] p Parameter to use as a pattern
*/
void ParameterSet::addInput(Parameter p) {
	ParameterInput *param = Gtk::manage(new ParameterInput(p));
	inputs.push_back(param);
	add(*(inputs.at(inputs.size()-1)));
}
