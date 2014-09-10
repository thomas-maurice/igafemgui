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
	\file getResultWindow.h
	\version 0.1
	\author Thomas Maurice
	\brief Implementation of the class GetResultWindow
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include <gtkmm.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <libxml++/libxml++.h>

#include <logWindow.h>
#include <misc.h>

/**
	\class GetResultWindow
	\brief Window used to download simulation results from the server
*/

class GetResultWindow : public Gtk::Window {
	public:
		GetResultWindow(ssh_session session); //!< Constructor
		void loadResults(); //!< Load the list of results you have
		void viewResult(); //!< Displays the info about a result
		void downloadResult(); //!< Downloads the result
		std::string readRemoteFileToString(std::string f); //!< Reads a remove file to a string
		void copyRemoteFile(std::string f, std::string outf); //!< Copies a remote file to the local computer
		void warn(std::string title, std::string summary, std::string body); //!< Pops a warning
		std::string executeRemoteCommand(std::string command); //!< Executes a command on the server
		bool onTimer(); //!< Timed events
		void removeDirectory(std::string directory, int level); //!< Recursively removes a directory
		void showOutputStream(int stream); //!< Shows an output stream
		void download(); //!< Downloads the results
		void copyDirectory(std::string remote, std::string local); //!< Recursively copies a directory
		void info(std::string title, std::string summary, std::string body); //!< Pops up an info message
		
	private:
    Gtk::ComboBoxText *resultSelect; //!< Result selection combobox
    Gtk::VBox *mainLayout; //!< Main layout
    Gtk::Label *infoLabel; //!< Label information
    Gtk::Button *refreshButton; //!< Button refresh
    Gtk::Button *removeButton; //!< Button remove
    Gtk::Button *showStdout; //!< Shows stdout ^^
    Gtk::Button *showStderr; //!< Shows stderr ^^
    Gtk::Button *downloadButton; //!< Download button ?
    ssh_session SSHSession; //!< SSH session
    std::string currentScript; //!< Current script name
    std::string currentSub; //!< Sumission time
    std::string currentDesc; //!< Description
};
