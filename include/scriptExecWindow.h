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
	\file scriptExecWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the ScriptExecWindow class
*/

#include <gtkmm.h>
#include <sys/types.h>
#include <unistd.h>
#include <scriptDescriptor.h>
#include <iostream>
#include <sys/wait.h>
#include <fstream>
#include <iomanip>
#include <misc.h>

/**
	\class ScriptExecWindow
	\brief Window in which the octave session is launched
	
	This window will hold the PID of the Octave session it has launched.
	The window looks like ![ScripeExecWindow window](launchscript.png)
*/

class ScriptExecWindow : public Gtk::Window {
	public:
		ScriptExecWindow(std::string baseDir, ScriptDescriptor script, std::vector<Parameter> p); //!< Constructor of the window
		~ScriptExecWindow(); //!< Destroys the window
		bool onTimer(); //!< Timed action
		void stopScript(); //!< Stop the script
		
	private:
		Gtk::TextView *logTextView; //!< Log output
    Glib::RefPtr<Gtk::TextBuffer> logBuffer; //!< Buffer of the log
    Gtk::Button *stopButton; //!< Stop button
    sigc::connection timer; //!< The timer
    Gtk::ScrolledWindow *scrolledWindow; //!< Scrolled window
    Gtk::VBox *mainLayout; //!< Main layout
    Gtk::Spinner *spinner; //!< Spinner; not used
    int inf, outf, errf; //!< File pointers
		pid_t scriptPid; //!< Octave session PID
};
