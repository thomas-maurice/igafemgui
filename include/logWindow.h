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
	\file logWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the LogWindow class
*/

#include <gtkmm.h>

/**
	\class LogWindow
	\brief Window showing textual logs
*/

class LogWindow : public Gtk::Window {
	public:
		LogWindow(std::string title, std::string text); //!< Constructor of the window
		~LogWindow(); //!< Destroys the window
		void stop(); //!< Stop the window
		
	private:
		Gtk::TextView *logTextView; //!< Log output
    Glib::RefPtr<Gtk::TextBuffer> logBuffer; //!< Buffer of the log
    Gtk::Button *stopButton; //!< Stop button
    Gtk::ScrolledWindow *scrolledWindow; //!< Scrolled window
    Gtk::VBox *mainLayout; //!< Main layout
};
