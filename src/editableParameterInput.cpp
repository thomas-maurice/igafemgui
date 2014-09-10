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
	\file editableParameterInput.cpp
	\version 0.2
	\author Thomas Maurice
	\brief Implements the EditableParameterInput class
*/

#include <editableParameterInput.h>

using namespace std;

/**
	Constructor of the parameter, it's been passed a Parameter element and
	and index i which will be used by the parent widget to know which widget
	calls the signal_delete event when it will have to be suppressed
	
	\param[in] p The default parameter, this one will be edited
	\param[in] i The index
*/
EditableParameterInput::EditableParameterInput(Parameter p, int i) {
	param = p;
	id = i;
	set_spacing(5);
	name = Gtk::manage(new Gtk::Label());
	desc = Gtk::manage(new Gtk::Label());
	
	name->set_markup("<b>"+param.name+"</b>");
	desc->set_markup(param.description);
	
	editButton = Gtk::manage(new Gtk::Button(Gtk::Stock::EDIT));
	editButton->signal_clicked().connect(sigc::mem_fun(*this, &EditableParameterInput::edit));
	
	delButton = Gtk::manage(new Gtk::Button(Gtk::Stock::DELETE));
	delButton->signal_clicked().connect(sigc::mem_fun(signal_delete, &sigc::signal<void>::emit));

	pack_start(*name, Gtk::PACK_SHRINK);
	pack_start(*desc, Gtk::PACK_SHRINK);
	pack_end(*editButton, Gtk::PACK_SHRINK);
	pack_end(*delButton, Gtk::PACK_SHRINK);
}

/**
	Opens an EditableParameterWindow to edit the default parameter
*/
void EditableParameterInput::edit() {
	EditableParameterWindow e(&param);
	e.run();
}

/**
	Returns the ID

	\return The ID of the field
*/
int EditableParameterInput::getId() {
	return id;
}

/**
	Returns the name of the parameter
	
	\return The name of the parameter
*/
std::string EditableParameterInput::getName() {
	return param.name;
}

/**
	Returns the edited parameter
	
	\return the parameter.
*/
Parameter EditableParameterInput::get() {
	return param;
}
