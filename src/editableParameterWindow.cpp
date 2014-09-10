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
	\file editableParameterWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the EditableParameterWindow class
*/

#include <editableParameterWindow.h>

using namespace std;

/**
	Creates a new EditableParameterWindow. This will modify the value
	stored into the a pointer to a Parameter. This correspond to the
	Parameter stored in the EditableParameterInput.
	
	\param[in] p Pointer on a Parameter
*/
EditableParameterWindow::EditableParameterWindow(Parameter *p) {
	set_title("Edition of a parameter");
	
	param = p;
	
	Gtk::Label *name = Gtk::manage(new Gtk::Label(" Name: "));
	Gtk::Label *desc = Gtk::manage(new Gtk::Label(" Description: "));
	Gtk::Label *num  = Gtk::manage(new Gtk::Label(" min/max/increment/value: "));
	mainLayout = Gtk::manage(new Gtk::Table(5,2));
	
	saveButton = Gtk::manage(new Gtk::Button(Gtk::Stock::SAVE));
	saveButton->signal_clicked().connect(sigc::mem_fun(*this, &EditableParameterWindow::save));
	
	ostringstream o("");
	
	nameEntry = Gtk::manage(new Gtk::Entry());
	nameEntry->set_text(p->name);
	descEntry = Gtk::manage(new Gtk::Entry());
	descEntry->set_text(p->description);
	minEntry = Gtk::manage(new Gtk::Entry());
	o << p->min;
	minEntry->set_text(o.str());
	o.str("");
	maxEntry = Gtk::manage(new Gtk::Entry());
	o << p->max;
	maxEntry->set_text(o.str());
	o.str("");
	incEntry = Gtk::manage(new Gtk::Entry());
	o << p->inc;
	incEntry->set_text(o.str());
	o.str("");
	valEntry = Gtk::manage(new Gtk::Entry());
	o << p->value;
	valEntry->set_text(o.str());
	
	mainLayout->attach(*name, 0, 1, 0, 1);
	mainLayout->attach(*nameEntry, 1, 2, 0, 1);
	mainLayout->attach(*desc, 2, 3, 0, 1);
	mainLayout->attach(*descEntry, 3, 4, 0, 1);
	mainLayout->attach(*num, 0, 1, 1, 2);
	mainLayout->attach(*minEntry, 1, 2, 1, 2);
	mainLayout->attach(*maxEntry, 2, 3, 1, 2);
	mainLayout->attach(*incEntry, 3, 4, 1, 2);
	mainLayout->attach(*valEntry, 4, 5, 1, 2);
	mainLayout->attach(*saveButton, 4, 5, 0, 1);
	
	remove();
	add(*mainLayout);
	show_all();
}

/**
	Returns the name of the parameter
	
	\return The name of the parameter
*/
std::string EditableParameterWindow::getName() {
	return nameEntry->get_text();
}

/**
	Saves the value of the parameter. This will change the value in the calling
	EditableParameterInput
*/
void EditableParameterWindow::save() {
	if(!isInputValid()) {
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "An input is not correct", true, Gtk::MESSAGE_ERROR);
			dialogue.set_title("Wrong input");
			dialogue.set_secondary_text("This input is not filled properly, check it.", true);
			dialogue.run();
	} else {
		*param = get();
		response(0);
	}
}

/**
	Check if the input is valid
	
	\return true if the input is valid, false otherwise
*/
bool EditableParameterWindow::isInputValid() {
	if(nameEntry->get_text() == "" ||
	   descEntry->get_text() == "" ||
	   minEntry->get_text() == "" ||
	   maxEntry->get_text() == "" ||
	   valEntry->get_text() == "" ||
	   incEntry->get_text() == "")
	   return false;
	
	if(!isNumber(minEntry->get_text()) ||
	   !isNumber(maxEntry->get_text()) ||
	   !isNumber(valEntry->get_text()) ||
	   !isNumber(incEntry->get_text()))
	   return false;
	
	return true;
	   
}

/**
	Check if the input is a number
	
	\param[in] s The string to test
	\return true if the input is valid, false otherwise
*/
bool EditableParameterWindow::isNumber(std::string s) {
	std::string allowed = "0123456789.,e-+";
	for(unsigned int i = 0; i < s.length(); i++) {
		if(allowed.find(s.at(i)) == string::npos)
			return false;
	}
	return true;
}

/**
	Returns the parameter.
	
	\return the parameter
*/
Parameter EditableParameterWindow::get() {
	Parameter p;
	p.name = nameEntry->get_text();
	p.description = descEntry->get_text();
	istringstream i1(minEntry->get_text());
	i1 >> p.min;
	istringstream i2(maxEntry->get_text());
	i2 >> p.max;
	istringstream i3(incEntry->get_text());
	i3 >> p.inc;
	istringstream i4(valEntry->get_text());
	i4 >> p.value;

	return p;
}
