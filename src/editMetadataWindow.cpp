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
	\file editMetadataWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the EditMetadataWindow class
*/

#include <editMetadataWindow.h>

using namespace std;

/**
	Creates a new EditMetadataWindow based on the supplied base directory and filename
	This will create the file /path/to/basedit/filename
	
	\param[in] basedir  Base directory in which the metadata are stored
	\param[in] filename Filename under which save the script.
*/
EditMetadataWindow::EditMetadataWindow(std::string basedir, std::string filename) {
	set_title("Editing new metadata");
	set_position(Gtk::WIN_POS_CENTER);
	filedir = basedir;
	
	layout = Gtk::manage(new Gtk::VBox());
	layout->set_spacing(5);
	
	currentScript = new ScriptDescriptor();
	
	/*
		The text view
	*/
	scriptDescTextView = Gtk::manage(new Gtk::TextView());
	scriptDescTextView->set_size_request(200, 100);
	scriptDescTextView->set_wrap_mode(Gtk::WRAP_WORD);
	scriptDescTextView->set_justification(Gtk::JUSTIFY_FILL);
	scriptDescBuffer = scriptDescTextView->get_buffer();
	
	/*
		Entries
	*/
	
	Gtk::HBox *hb1 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb2 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb3 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb4 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb5 = Gtk::manage(new Gtk::HBox());
	
	Gtk::Label* script = Gtk::manage(new Gtk::Label("Script name (without .m) : "));
	Gtk::Label* directory = Gtk::manage(new Gtk::Label("Directory : "));
	Gtk::Label* author = Gtk::manage(new Gtk::Label("Author : "));
	Gtk::Label* desc = Gtk::manage(new Gtk::Label("Description of the script :"));
	Gtk::Label* plist = Gtk::manage(new Gtk::Label("Parameters list"));
	Gtk::Label* geom = Gtk::manage(new Gtk::Label("Geometry file (without .m) : "));
	Gtk::Label* geomd = Gtk::manage(new Gtk::Label("Geometry file directory : "));
	
	desc->set_justify(Gtk::JUSTIFY_CENTER);
	
	scriptNameEntry = Gtk::manage(new Gtk::Entry());
	scriptNameEntry->set_text(filename);
	directoryEntry = Gtk::manage(new Gtk::Entry());
	authorEntry = Gtk::manage(new Gtk::Entry());
	authorEntry->set_text(getenv("USER"));
	
	geometryEntry = Gtk::manage(new Gtk::Entry());
	geometryDirEntry = Gtk::manage(new Gtk::Entry());
	
	// Buttons
	saveButton = Gtk::manage(new Gtk::Button(Gtk::Stock::SAVE));
	addButton = Gtk::manage(new Gtk::Button(Gtk::Stock::ADD));
	
	// EPS
	params = Gtk::manage(new EditableParameterSet());
	
	// Layouts
	
	hb1->add(*script);
	hb1->add(*scriptNameEntry);
	
	hb2->add(*directory);
	hb2->add(*directoryEntry);
	
	hb3->add(*author);
	hb3->add(*authorEntry);
	
	hb4->add(*plist);
	hb4->add(*addButton);	
	
	hb5->add(*geomd);
	hb5->add(*geometryDirEntry);
	hb5->add(*geom);
	hb5->add(*geometryEntry);
	
	/*
		Packing
	*/
	
	layout->pack_start(*hb1, Gtk::PACK_SHRINK);
	layout->pack_start(*hb2, Gtk::PACK_SHRINK);
	layout->pack_start(*hb3, Gtk::PACK_SHRINK);
	layout->pack_start(*hb5, Gtk::PACK_SHRINK);
	layout->pack_start(*desc, Gtk::PACK_SHRINK);
	layout->pack_start(*scriptDescTextView, Gtk::PACK_SHRINK);
	layout->pack_start(*hb4, Gtk::PACK_SHRINK);
	layout->pack_start(*params, Gtk::PACK_SHRINK);
	layout->pack_end(*saveButton, Gtk::PACK_SHRINK);
	
	add(*layout);
	
	// Signals !
	addButton->signal_clicked().connect(sigc::mem_fun(*this, &EditMetadataWindow::addNewInput));
	saveButton->signal_clicked().connect(sigc::mem_fun(*this, &EditMetadataWindow::save));
	
	loadIfExists(filename);
	
	show_all();
}

/**
	Deletes the dynamically allocated stuff
*/
EditMetadataWindow::~EditMetadataWindow() {
	delete currentScript;
}

/**
	Load the scriptname if it already exists. Otherwise all the fields
	will just be blank.
*/
void EditMetadataWindow::loadIfExists(std::string file) {
	string script = filedir + "/" + scriptNameEntry->get_text() + ".xml";
	
	currentScript->reset();
	
	xmlpp::DomParser parser;
	parser.set_throw_messages(false);
	parser.set_substitute_entities(true);
	try {
		parser.parse_file(script);
	} catch (exception &e) {
		return;
	}
	
	if(parser)
	{
		//Walk the tree:
		xmlpp::Node* rootNode = parser.get_document()->get_root_node();
		xmlpp::Node* elem = rootNode->get_first_child();
		
		while(elem) {
			string name = elem->get_name();
			if(name != "text") { // ignore 'text' parasite nodes
				if(name == "param") { // We load the parameters
					const xmlpp::Element* contentNode = dynamic_cast<const xmlpp::Element*>(elem);
					const xmlpp::Attribute* paramName = contentNode->get_attribute("name");
					const xmlpp::Attribute* paramDefault = contentNode->get_attribute("default");
					const xmlpp::Attribute* paramDesc = contentNode->get_attribute("desc");
					const xmlpp::Attribute* paramMin = contentNode->get_attribute("min");
					const xmlpp::Attribute* paramMax = contentNode->get_attribute("max");
					const xmlpp::Attribute* paramInc = contentNode->get_attribute("inc");
					
					if(paramName && paramDefault && paramDesc && paramMin && paramMax && paramInc) { // We load the parameters
						//cout << paramName->get_value() << " : " << paramDefault->get_value() << " : " << paramDesc->get_value() << endl;
						currentScript->addParameter(paramName->get_value(), paramDesc->get_value(), nstrtod(paramDefault->get_value().c_str(), NULL),
						nstrtod(paramMin->get_value().c_str(), NULL),
						nstrtod(paramMax->get_value().c_str(), NULL),
						nstrtod(paramInc->get_value().c_str(), NULL));
						
					}
				} else { // We load the main information about the script
					const xmlpp::ContentNode* nodeText = dynamic_cast<const xmlpp::ContentNode*>(elem->get_first_child());
					if(nodeText) {
						if(name == "name") {
							currentScript->setName(nodeText->get_content());
							scriptNameEntry->set_text(currentScript->getName());
						} else if(name == "author") {
							currentScript->setAuthor(nodeText->get_content());
							authorEntry->set_text(currentScript->getAuthor());
						} else if(name == "description") {
							currentScript->setDescription(nodeText->get_content());
							scriptDescBuffer->set_text(currentScript->getDescription());
						} else if(name == "dir") {
							currentScript->setDirectory(nodeText->get_content());
							directoryEntry->set_text(currentScript->getDirectory());
						} else if(name == "geomdir") {
							currentScript->setGeometryDirectory(nodeText->get_content());
							geometryDirEntry->set_text(currentScript->getGeometryDirectory());
						} else if(name == "geom") {
							currentScript->setGeometry(nodeText->get_content());
							geometryEntry->set_text(currentScript->getGeometry());
						} else {
						
						}
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
		
		vector<Parameter> p = currentScript->getParameter();
		for(unsigned int i = 0; i < p.size(); i++) {
			addInput(p.at(i));
		}
  } else {
  
  }
}

/**
	This function will add a new input to the EditableParameterSet
*/
void EditMetadataWindow::addNewInput() {
	Parameter p;
	p.name = "parameter";
	p.value = 42;
	p.min = 0;
	p.max = 100;
	p.inc = 0.1;
	p.description = "Description";
	
	params->addInput(p);
	set_position(Gtk::WIN_POS_CENTER);
}

/**
	This function will add a new input to the EditableParameterSet
	\param[in] p The parameter to set as a default
*/
void EditMetadataWindow::addInput(Parameter p) {
	params->addInput(p);
	set_position(Gtk::WIN_POS_CENTER);
}

/**
	Save the current scriptinfo to a file.
	If the save cannot be performed then an error message shall be displayed
*/
void EditMetadataWindow::save() {
	if(params->isValid()) {
		// Save
		ofstream o(string(filedir+"/"+scriptNameEntry->get_text()+".xml").c_str(), ios::out);
		if(o) {
			o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
			o << "<octscript>" << endl;
			o << "\t<name>" << scriptNameEntry->get_text() << "</name>" << endl;
			o << "\t<dir>" << directoryEntry->get_text() << "</dir>" << endl;
			o << "\t<geomdir>" << geometryDirEntry->get_text() << "</geomdir>" << endl;
			o << "\t<geom>" << geometryEntry->get_text() << "</geom>" << endl;
			o << "\t<author>" << authorEntry->get_text() << "</author>" << endl;
			o << "\t<description>" << scriptDescBuffer->get_text() << "</description>" << endl;
			vector<Parameter> p = params->get();
			for(unsigned int i = 0; i < p.size(); i++) {
				// <param name="foo" default="42" desc="bar, also : baz" min="0" max="100" inc="0.1"/>
				o << "\t<param ";
				o << "name=\"" << p.at(i).name << "\" ";
				o << "default=\"" << p.at(i).value << "\" ";
				o << "desc=\"" << p.at(i).description << "\" ";
				o << "min=\"" << p.at(i).min << "\" ";
				o << "max=\"" << p.at(i).max << "\" ";
				o << "inc=\"" << p.at(i).inc << "\" />\n";
			}
			o << "</octscript>" << endl;
			o.close();
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "Data was successfully saved :)", true, Gtk::MESSAGE_INFO);
			dialogue.set_title("Saved");
			dialogue.set_secondary_text("You may now close the window.", true);
			dialogue.run();
			this->hide();
			this->~EditMetadataWindow();
		} else {
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "The data file could not be created", true, Gtk::MESSAGE_ERROR);
			dialogue.set_title("Cannot save");
			dialogue.set_secondary_text("The file <b>" + filedir+"/"+scriptNameEntry->get_text()+".xml" + "</b> could not be created.", true);
			dialogue.run();
		}
	}
}
