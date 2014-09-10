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
	\file dialogConnect.cpp
	\brief Implementeation of the dialogConnect class
	
	\author Thomas Maurice
	\version 0.1
*/

#include <dialogConnect.h>

using namespace std;

/**
	Creates a connection dialog. This will change pointers to strings and int
	representing the connection. Mainly user, host and port. The final values of
	the pointers will be assigned when the dialog will be closed.
	
	\param[in,out] u Pointer on the username
	\param[in,out] pw Pointer on the password
	\param[in,out] h Pointer on the host
	\param[in,out] p Pointer on the port
*/
DialogConnect::DialogConnect(std::string *u, std::string *pw, std::string *h, int *p) {
	user = u;
	host = h;
	port = p;
	pass = pw;
	
	Gtk::VBox *layout = Gtk::manage(new Gtk::VBox);
	
	Glib::RefPtr<Gtk::Adjustment> adj = Gtk::Adjustment::create(*p, 0, 65535, 1);
	portSpinButton = Gtk::manage(new Gtk::SpinButton(adj));
	
	okButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
	
	userEntry = Gtk::manage(new Gtk::Entry());
	userEntry->set_text(*u);
	
	passEntry = Gtk::manage(new Gtk::Entry());
	passEntry->set_visibility(false);
	passEntry->set_text(*pw);
	
	hostEntry = Gtk::manage(new Gtk::Entry());
	hostEntry->set_text(*h);
	
	Gtk::Label* ul = Gtk::manage(new Gtk::Label("Username : "));
	Gtk::Label* pwl = Gtk::manage(new Gtk::Label("Password : "));
	Gtk::Label* hl = Gtk::manage(new Gtk::Label("Hostname : "));
	Gtk::Label* pl = Gtk::manage(new Gtk::Label("Port     : "));
	
	Gtk::HBox *h1 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *h2 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *h3 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *h4 = Gtk::manage(new Gtk::HBox());
	
	h1->add(*ul);
	h1->add(*userEntry);
	
	h4->add(*pwl);
	h4->add(*passEntry);
	
	h2->add(*hl);
	h2->add(*hostEntry);
	
	h3->add(*pl);
	h3->add(*portSpinButton);
	
	layout->pack_start(*h1, Gtk::PACK_SHRINK);
	layout->pack_start(*h4, Gtk::PACK_SHRINK);
	layout->pack_start(*h2, Gtk::PACK_SHRINK);
	layout->pack_start(*h3, Gtk::PACK_SHRINK);
	layout->pack_end(*okButton, Gtk::PACK_SHRINK);
	
	remove();
	add(*layout);
	
	okButton->signal_clicked().connect(sigc::mem_fun(*this, &DialogConnect::onOk));
	passEntry->signal_activate().connect(sigc::mem_fun(*this, &DialogConnect::onOk));
	hostEntry->signal_activate().connect(sigc::mem_fun(*this, &DialogConnect::onOk));
	
	show_all();
}

void DialogConnect::onOk() {
	*user = userEntry->get_text();
	*host = hostEntry->get_text();
	*pass = passEntry->get_text();
	*port = portSpinButton->get_value();
	hide();
	response(Gtk::RESPONSE_OK);
}
