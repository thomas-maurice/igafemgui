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
	\file editMetadataWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the EditMetadataWindow class
*/

#include <gtkmm.h>
#include <fstream>
#include <libxml++/libxml++.h>
#include <scriptDescriptor.h>
#include <editableParameterSet.h>
#include <misc.h>

/**
	\class EditMetadataWindow
	\brief Window used to edit the scriptinfo
*/

class EditMetadataWindow : public Gtk::Window {
	public:
		EditMetadataWindow(std::string basedir, std::string file); //!< Constructor
		~EditMetadataWindow();//!< Destructor
		void loadIfExists(std::string file); //!< Loads a scriptinfo file
		void addInput(Parameter p); //!< Add a Parameter
		void addNewInput(); //!< Add a new Parameter
		void save(); //!< Save the current scriptinfo
		
	private:
		EditableParameterSet *params; //!< Set of EditableParameterInput
		Gtk::VBox *layout; //!< Main layout
		ScriptDescriptor *currentScript; //!< Pointer on the current script
		Gtk::Entry *scriptNameEntry; //!< Entry for the script name
		Gtk::Entry *authorEntry; //!< Entry for the author name
		Gtk::Entry *geometryEntry; //!< Entry for the grometry file
		Gtk::Entry *geometryDirEntry; //!< Entry for the geometry file directory
		Gtk::TextView *scriptDescTextView; //!< Textview for the description
		Glib::RefPtr<Gtk::TextBuffer> scriptDescBuffer; //!< Buffer for the textview
		Gtk::Entry *directoryEntry; //!< Directory entry
		Gtk::Button* saveButton; //!< Save button
		Gtk::Button *addButton; //!< Add button
		std::string filedir; //!< Directory in which the files are stored
};
