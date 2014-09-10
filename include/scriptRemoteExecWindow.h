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
	\file scriptRemoteExecWindow.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the ScriptRemoteExecWindow class
*/

#include <gtkmm.h>

#include <libssh/libssh.h>
#include <libssh/sftp.h>

#include <sys/types.h>
#include <unistd.h>
#include <scriptDescriptor.h>
#include <iostream>
#include <sys/wait.h>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <misc.h>

/**
	\class ScriptRemoteExecWindow
	\brief Creates temporary scripts and launch them on the server
*/

class ScriptRemoteExecWindow : public Gtk::Window {
	public:
		ScriptRemoteExecWindow(std::string baseDir, ScriptDescriptor script, std::vector<Parameter> p, ssh_session session); //!< Constructor of the window
		~ScriptRemoteExecWindow(); //!< Destroys the window
		bool onTimer(); //!< Timed action
		void createFile(std::string fname, std::string s, std::ostringstream &out); //!< Create a file.
		void createDirectory(std::string dname); //!< Creates a directory
		void stopScript(); //!< Stop the script
		std::string executeRemoteCommand(std::string command); //!< Executes a command and returns the output to string
		std::string getTime(); //!< Returns the current time
		
	private:
		Gtk::TextView *logTextView; //!< Log output
    Glib::RefPtr<Gtk::TextBuffer> logBuffer; //!< Buffer of the log
    Gtk::Button *stopButton; //!< Stop button
    sigc::connection timer; //!< The timer
    Gtk::ScrolledWindow *scrolledWindow; //!< Scrolled window
    Gtk::VBox *mainLayout; //!< Main layout
    ssh_session SSHSession;
};
