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
	\file mainWindow.h
	\brief Definition of the MainWindow class
	
	\author Thomas Maurice
	\version 0.2
*/

/**
	\class MainWindow
	\brief Represents the main window of the software
	
	This window has the following layout : ![Main window](mainwindow.png)
	
	And looks like that when we are connected to a remote server :
	![Main window connected to a remote server](netconnected.png)
*/

#include <gtkmm.h>

#include <libxml++/libxml++.h>

#include <libssh/libssh.h>
#include <libssh/sftp.h>

#include <dirent.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

#include <misc.h>
#include <scriptDescriptor.h>
#include <parameterSet.h>
#include <editConfWindow.h>
#include <editMetadataWindow.h>
#include <scriptExecWindow.h>
#include <scriptRemoteExecWindow.h>
#include <aboutWindow.h>
#include <dialogConnect.h>
#include <getResultWindow.h>

class MainWindow : public Gtk::Window {
	public:
		MainWindow(); //!< Simple constructor for the main window
		~MainWindow(); //!< Destructor for the main window
		void makeMenus(); //!< Creates the menus
		void loadAvailableScripts(); //!< List the scripts in the script directory and load them
		void loadConfiguration(); //!< Load the config file
		void loadRemoteConfiguration(); //!< Load config from remote host
		void runScript(); //!< Runs a script
		void dispScript(); //!< Displays on the window the scripts informations
		void errorAndExit(std::string title, std::string summary, std::string body = ""); //!< In case of fatal error
		void warn(std::string title, std::string summary, std::string body = ""); //!< Warning
		void info(std::string title, std::string summary, std::string body = ""); //!< Info
		int  question(std::string title, std::string summary, std::string body = ""); //!< Yes/No question
		void editConfiguration(); //!< Changes the configuration
		void editMetadata(); //!< Edit a script info
		void getResults(); //!< Download results from the server
		void newMetadata(); //!< Creates a new script info
		void removeCurrent(); //!< Removes the current script
		void about(); //!< About this program
		bool onTimer(); //!< Timed action
		void loadRemoteAvailableScripts(); //!< Load the remote script list
		bool checkRemoteHost(); //!< Check the identity of the remote host
		void connectToRemote(); //!< Connect to a remote server vis SSH to run scripts on it
		void disconnectFromRemote(); //!< Disconnects you from the remote server
		std::string executeRemoteCommand(std::string command); //!< Execute a command on the remote host
		std::string readRemoteFileToString(std::string f); //!< Read a remote file and put it into  a string
		
	private:
		// Layouts
		Gtk::VBox *mainLayout; //!< Main layout of the window
		// Inputs
		Gtk::ComboBoxText *scriptSelect; //!< Combo box used to select the script to run
		Gtk::Button *runButton; //!< Button used to run a script
		// Displays
		Gtk::Label *scriptDesc; //!< Label which displays info on the script
		Gtk::Label *connInfo; //!< Label to display if we are connected
		ParameterSet *params; //!< Layout-inherited class which contains the list of ParameterInput the script has
		
		// Entry
		Gtk::Entry *geometryEntry; //!< Entry for the grometry file
		Gtk::Entry *geometryDirEntry; //!< Entry for the geometry file directory
		
		// Menus
		Gtk::MenuBar *menuBar; //!< Obvious.
		Gtk::ImageMenuItem *menuItemConf; //!< Configuration tab
		Gtk::ImageMenuItem *menuItemScripts; //!< Scripts tab
		
		// Data
		ScriptDescriptor *currentScript; //!< A pointer on the currently loaded script descriptor
		std::string homedir; //!< The user's home
		std::string basescript; //!< The directory in which is stored the Octave code
		std::string basefiles; //!< The directory in which are stored the scriptinfos
		std::string defhost; //!< Default connection host
		std::string defuser; //!< Default connection user
		std::string currentwd; //!< Current working directory **DEPRECATED**
		
		// Network data
		ssh_session SSHSession; //!< The ssh_session used to connect to a remote server
		bool onRemote; //!< Are we connected to a remote server ?
};
