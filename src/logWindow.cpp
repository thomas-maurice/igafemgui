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
	\file logWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the LogWindow class
*/

#include <logWindow.h>

using namespace std;

/**
	Constructor, it will just display some text
*/
LogWindow::LogWindow(std::string title, std::string text) {
	set_title(title);
	//set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
  set_default_size(400,400);
  set_size_request(400,400);
	// Layout
	mainLayout = Gtk::manage(new Gtk::VBox());
	// TextView
	logTextView = Gtk::manage(new Gtk::TextView());
	logBuffer = Gtk::TextBuffer::create();
	logTextView->set_buffer(logBuffer);
	Pango::FontDescription fdesc;
	fdesc.set_family("monospace");
	fdesc.set_size(10 * PANGO_SCALE);
	logTextView->override_font(fdesc);
	logTextView->set_editable(false);
  
  logBuffer->set_text(text);
  
	// Buttons
	stopButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));

	//Création d'un conteneur ayant des barres de défilement.
	scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
	scrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolledWindow->add(*logTextView);
	
	mainLayout->add(*scrolledWindow);
	mainLayout->pack_end(*stopButton, Gtk::PACK_SHRINK);
	
	stopButton->signal_clicked().connect(sigc::mem_fun(*this, &LogWindow::stop));
	this->signal_hide().connect(sigc::mem_fun(*this, &LogWindow::stop));
	
	add(*mainLayout);
	
	show_all();
}

/**
	Destroys the window
*/
LogWindow::~LogWindow() {

}

/**
	Stops displaying the log
*/
void LogWindow::stop() {
	this->~LogWindow();
	hide();
}
