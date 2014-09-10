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
	\file editableParameterSet.cpp
	\author Thomas Maurice
	\version 0..2
	\brief Implement the EditableParameterSet class
*/

#include <editableParameterSet.h>

using namespace std;

/**
	Default constructor, not much to say about it
*/
EditableParameterSet::EditableParameterSet() {
	curId = 0;
}

/**
	Clears all the parameters which are registered
*/
void EditableParameterSet::clear() {
	for(unsigned int i = 0; i < inputs.size(); i++) {
		inputs.at(i)->~EditableParameterInput();
	}
	inputs.clear();
}

/**
	Delete an input, this is called when a signal_delete is emitted
	by one of the children.
	
	\param[in] id The parameter to delete
*/
void EditableParameterSet::delInput(int id) {
	for(unsigned int i = 0; i < inputs.size(); i++) {
		if(inputs.at(i)->getId() == id) {
			inputs.at(i)->~EditableParameterInput();
			inputs.erase(inputs.begin()+i);
			i--;
		}
	}
}

/**
	Check if the inputs are valids, but unused at the moment
*/
bool EditableParameterSet::isValid() {
	std::vector<Parameter> p;
	for(unsigned int i = 0; i < inputs.size(); i++) {
		/*if(!inputs.at(i)->isInputValid()) {
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "An input is not correct", true, Gtk::MESSAGE_ERROR);
			dialogue.set_title("Wrong input");
			dialogue.set_secondary_text("The input <b>" + inputs.at(i)->getName() + "</b> is not filled properly, check it.", true);
			dialogue.run();
			return false;
		}*/
	}
	return true;
}

/**
	Return all the parameters registered in a vector of Parameter.
*/
std::vector<Parameter> EditableParameterSet::get() {
	std::vector<Parameter> p;
	for(unsigned int i = 0; i < inputs.size(); i++) {
		//if(inputs.at(i)->isInputValid())
			p.push_back(inputs.at(i)->get());
	}
	return p;
}

/**
	Add an input to register a new parameter. This appends an EditableParameterInput
	to the layout and enables the user to edit it.
	
	\param[in] p The default parameter
*/
void EditableParameterSet::addInput(Parameter p) {
	EditableParameterInput *param = Gtk::manage(new EditableParameterInput(p, curId));
	param->signal_delete.connect(sigc::bind<int>(mem_fun(*this, &EditableParameterSet::delInput), param->getId()));
	curId++;
	inputs.push_back(param);
	add(*(inputs.at(inputs.size()-1)));
	show_all();
}
