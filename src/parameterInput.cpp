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
	\file parameterInput.cpp
	\version 0.2
	\author Thomas Maurice
	\brief Implements the ParameterInput class
*/

#include <parameterInput.h>
#include <iostream>

/**
	Contructor. This will build a ParameterInput from a given Parameter
	structure.
*/
ParameterInput::ParameterInput(Parameter p) {
	Glib::RefPtr<Gtk::Adjustment> adj = Gtk::Adjustment::create(p.value, p.min, p.max, p.inc);
	param = p;
	
	value = Gtk::manage(new Gtk::SpinButton(adj));
	value->set_digits(6);
	
	Gtk::Label *name = Gtk::manage(new Gtk::Label(""));
	name->set_markup("<b>" + p.name + "</b>");
	Gtk::Label *desc = Gtk::manage(new Gtk::Label(p.description));
	
	add(*name);
	add(*desc);
	add(*value);
}

/**
	This will return the actual value of the parameter (edited by the user)
*/
Parameter ParameterInput::get() {
	Parameter p = param;
	p.value = value->get_value();
	return p;
}
