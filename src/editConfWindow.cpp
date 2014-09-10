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
	\file editConfWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the EditConfWindow class
*/

#include <editConfWindow.h>

using namespace std;

/**
	Default constructor of the object.
	This will load the current config to set the defaults fields of the window.
	If by any chance no configuration is availiable, then it will create a
	sample one.
*/
EditConfWindow::EditConfWindow() {
	set_title("Configuration editor");
	layout = Gtk::manage(new Gtk::VBox());
	set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
	
	set_default_size(500,50);
	
	homedir = getenv("HOME");
  // Checking if the config dir exists
  if(!directory_exists(homedir + "/.igafemgui")) {
  	cout << "Creating " << homedir + "/.igafemgui" << endl;
  	make_directory(homedir + "/.igafemgui");
  	cout << "Creating sample config..." << endl;
  	ofstream f(string(homedir + "/.igafemgui/config.xml").c_str(), ios::out | ios::trunc);
  	if(f) {
  		f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<config>\n\
\t<basefem>" << homedir << "/IsogeometricalAnalysis</basefem>\n\
\t<basedat>" << homedir << "/.igafemgui/files</basedat>\n\
</config>";
			f.close();
  	} else {
  		cout << "Cannot open config file" << endl;
  	}
  }
	
	saveButton = Gtk::manage(new Gtk::Button(Gtk::Stock::SAVE));
	
	
	Gtk::HBox *hb1 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb2 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb3 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb4 = Gtk::manage(new Gtk::HBox());
	
	Gtk::Label *s = Gtk::manage(new Gtk::Label("Scripts directory : "));
	Gtk::Label *f = Gtk::manage(new Gtk::Label("Scripts info (XML) directory : "));
	Gtk::Label *h = Gtk::manage(new Gtk::Label("Default remote host : "));
	Gtk::Label *u = Gtk::manage(new Gtk::Label("Default username for remote host : "));
	
	scriptDirEntry = Gtk::manage(new Gtk::Entry());
	filesDirEntry = Gtk::manage(new Gtk::Entry());
	hostEntry = Gtk::manage(new Gtk::Entry());
	userEntry = Gtk::manage(new Gtk::Entry());
	
	hb1->add(*s);
	hb1->add(*scriptDirEntry);
	
	hb2->add(*f);
	hb2->add(*filesDirEntry);
	
	hb3->add(*h);
	hb3->add(*hostEntry);
	
	hb4->add(*u);
	hb4->add(*userEntry);
	
	layout->set_spacing(5);
	
	layout->pack_start(*hb1, Gtk::PACK_SHRINK);
	layout->pack_start(*hb2, Gtk::PACK_SHRINK);
	layout->pack_start(*hb4, Gtk::PACK_SHRINK);
	layout->pack_start(*hb3, Gtk::PACK_SHRINK);
	
	layout->pack_end(*saveButton, Gtk::PACK_SHRINK);
	
	add(*layout);
	
	saveButton->signal_clicked().connect(sigc::mem_fun(*this, &EditConfWindow::save));
	
	loadCurrent();
	
	show_all();
}

/**
	Loads the current configuration files.
*/
void EditConfWindow::loadCurrent() {
	xmlpp::DomParser parser;
	parser.set_throw_messages(true);
	parser.set_substitute_entities(true);
	parser.parse_file(homedir + "/.igafemgui/config.xml");
	
	if(parser)
	{
		//Walk the tree:
		xmlpp::Node* rootNode = parser.get_document()->get_root_node();
		xmlpp::Node* elem = rootNode->get_first_child();
		
		while(elem) {
			string name = elem->get_name();
			if(name != "text") { // ignore 'text' parasite nodes
				const xmlpp::ContentNode* nodeText = dynamic_cast<const xmlpp::ContentNode*>(elem->get_first_child());
				if(nodeText) {
					if(name == "basefem") {
						scriptDirEntry->set_text(nodeText->get_content());
					} else if(name == "basedat") {
						filesDirEntry->set_text(nodeText->get_content());
					} else if (name == "defhost") {
						hostEntry->set_text(nodeText->get_content());
					} else if (name == "defuser") {
						userEntry->set_text(nodeText->get_content());
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
  } else {
  	cout << "Unable to open config file " << homedir + "/.igafemgui/config.xml" << endl;
  	cout << "Creating sample config..." << endl;
  	ofstream f(string(homedir + "/.igafemgui/config.xml").c_str(), ios::out | ios::trunc);
  	if(f) {
  		f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<config>\n\
\t<basefem>" << homedir << "/IsogeometricalAnalysis</basefem>\n\
\t<basedat>" << homedir << "/.igafemgui/files</basedat>\n\
\t<defhost>" << "example.fr" << "</defhost>\n\
\t<defuser>" << "user" << "</defuser>\n\
</config>";
			f.close();
			loadCurrent();
  	}
	}
}

/**
	Destructor. Actually this does nothing.
*/
EditConfWindow::~EditConfWindow() {
	/*
		Nothing, but as a compensation, here is a quote :
		
		Quidquid latine dictum sit, altum sonatur.
		- Whatever is said in Latin sounds profound.
	*/
}

/**
	Save the current config from the fields. If the configuration
	cannot be savec for any reason, then the program will pop up
	an error window to warn the user.
*/
void EditConfWindow::save() {
	cout << "Saving configuration" << endl;
	ofstream f(string(homedir + "/.igafemgui/config.xml").c_str(), ios::out | ios::trunc);
  if(f) {
  	f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<config>\n\
\t<basefem>" << scriptDirEntry->get_text() << "</basefem>\n\
\t<basedat>" << filesDirEntry->get_text() <<"</basedat>\n\
\t<defhost>" << hostEntry->get_text() <<"</defhost>\n\
\t<defuser>" << userEntry->get_text() <<"</defuser>\n\
</config>";
		f.close();
		Gtk::MessageDialog dialogue(*this, "Configuration saved", true, Gtk::MESSAGE_INFO);
		dialogue.set_title("Done.");
		dialogue.set_secondary_text("The configuration has been successfully saved !", true);
		dialogue.run();
		cout << "Configuration saved !" << endl;
		this->hide();
		this->~EditConfWindow();
  } else {
  	Gtk::MessageDialog dialogue(*this, "Configuration failed to be saved", true, Gtk::MESSAGE_ERROR);
		dialogue.set_title("Error while saving");
		dialogue.set_secondary_text("The configuration could not be created, please check that ~/.igafem exist and is writable.", true);
		dialogue.run();
  }
}
