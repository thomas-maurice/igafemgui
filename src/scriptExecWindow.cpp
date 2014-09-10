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
	\file scriptExecWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the ScriptExecWindow class
*/

#include <scriptExecWindow.h>

using namespace std;

/**
	Constructor, it will create a new octave session in a forking thread and will
	run the selected script with the given parameters. It will create a temporary M file containing
	- The parameters needed
	- a cd command to go in the right directory
	- a call to the program
	- a final printf to warn that everything is done
	- a 'pause' call so that octave will close when we kill its process.
  
  and then run it until the user closes the window.
  
  \param basescript basename of the script (without .m)
  \param currentScript Descriptor of the script you want to launch
  \param p Vector of parameter
*/
ScriptExecWindow::ScriptExecWindow(std::string basescript, ScriptDescriptor currentScript, std::vector<Parameter> p) {
	set_title(currentScript.getName());
	//set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
  set_default_size(400,500);
  set_size_request(400,500);
  
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
  
	// Buttons
	stopButton = Gtk::manage(new Gtk::Button(Gtk::Stock::STOP));

	//Création d'un conteneur ayant des barres de défilement.
	scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
	scrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolledWindow->add(*logTextView);
	
	mainLayout->add(*scrolledWindow);
	mainLayout->pack_end(*stopButton, Gtk::PACK_SHRINK);
	
	stopButton->signal_clicked().connect(sigc::mem_fun(*this, &ScriptExecWindow::stopScript));
	this->signal_hide().connect(sigc::mem_fun(*this, &ScriptExecWindow::stopScript));
	timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &ScriptExecWindow::onTimer), 1000);
	
	add(*mainLayout);
	
	show_all();
	
	string fname = tmpnam(NULL);
	ofstream of(fname.c_str(), ios::out);
	ostringstream out("");
	if(of) {
		out << "---------------------------------------------" << endl;
		out << "> Created temporary M file : " << fname << endl;
		out << "> Exporting the variables to the script" << endl;
		out << "---------------------------------------------" << endl;
		out << "     Name             |          Value       " << endl;
		out << "---------------------------------------------" << endl;
		for(unsigned int i = 0; i < p.size(); i++) {
			out << setw(21) << right << p.at(i).name << right << " |" << setw(21) << p.at(i).value << endl;
		}
		out << "---------------------------------------------" << endl;
		of << "clear all;" << endl;
		of << "restoredefaultpath();" << endl;
		//if(currentScript.getGeometryDirectory() != "")
		of << "cd " + basescript + "/" + currentScript.getGeometryDirectory() << endl;
		if(currentScript.getGeometry() != "")
			of << currentScript.getGeometry() << endl;
		of << "cd " + basescript + "/" + currentScript.getDirectory() << endl;
		for(unsigned int i = 0; i < p.size(); i++) { // Export variables !
			of << p.at(i).name << " = " << p.at(i).value << ";" << endl;
		}
		of << currentScript.getName() << endl;
		of << "printf('---------------------------------------------\\n')" << endl;
		of << "printf('              SCRIPT HAS ENDED               \\n')" << endl;
		of << "pause" << endl;
		of.close();
		out << "> Running the script" << endl;
		scriptPid = popen2(string("octave -q " + fname).c_str(), &inf, &outf, &errf);
		out << "> Octave PID : "<<scriptPid<<endl;
		out << "---------------------------------------------" << endl;
	} else {
		out << "Failed to create tmp file" << endl;
	}
	
	logBuffer->set_text(out.str());
}

/**
	Timed actions. It checks if the process has new data output and append it to
	the log display. It also check if the process is still alive.
*/
bool ScriptExecWindow::onTimer() {
		// Using select
		// CHECKING OUT
		fd_set set;
		struct timeval delai;

		FD_ZERO(&set);
		FD_SET(outf, &set);
		delai.tv_sec = 0;
		delai.tv_usec = 100;

		int ret = select(FD_SETSIZE, &set, NULL, NULL, &delai);

		if(ret == 1)
		{
      char buff[512];
			int nbyte = 0;
			if((nbyte = read(outf, buff, sizeof(buff))) > 0) {
				//logBuffer->set_text(logBuffer->get_text()+string(buff).substr(0, nbyte));
				logBuffer->insert(logBuffer->end(), string(buff).substr(0, nbyte));
				//logBuffer->place_cursor(logBuffer->end());
				Glib::RefPtr<Gtk::TextBuffer::Mark> m = logBuffer->create_mark(logBuffer->end());
				logTextView->scroll_to(m, 0.0);
			}
		}
		
		// CHECKING ERROR
		FD_ZERO(&set);
		FD_SET(errf, &set);
		delai.tv_sec = 0;
		delai.tv_usec = 100;

		ret = select(FD_SETSIZE, &set, NULL, NULL, &delai);

		if(ret == 1)
		{
      char buff[512];
			int nbyte = 0;
			if((nbyte = read(errf, buff, sizeof(buff))) > 0) {
				logBuffer->insert(logBuffer->end(), string(buff).substr(0, nbyte));
				// These two lines make you scroll down to the bottom of the buffer
				Glib::RefPtr<Gtk::TextBuffer::Mark> m = logBuffer->create_mark(logBuffer->end());
				logTextView->scroll_to(m, 0.0);
			}
		}
	
	// Let's check if the process is still alive.
	if(!is_process_alive(scriptPid)) {
		logBuffer->set_text(logBuffer->get_text() + "\nOctave process has been killed.\nYou should re-run the script");
		timer.disconnect();
		Glib::RefPtr<Gtk::TextBuffer::Mark> m = logBuffer->create_mark(logBuffer->end());
		logTextView->scroll_to(m, 0.0);
	}
	
	return true;
}

/**
	Stops the script by killing its PID and close the window
*/
ScriptExecWindow::~ScriptExecWindow() {
	// Let's kill the program
	kill(scriptPid, SIGINT);
	// And reap the process
	waitpid(scriptPid, NULL, 0);
	cout << "> Octave session with PID " << scriptPid << " terminated." << endl;
}

/**
	Stops the script and destroys the window
*/
void ScriptExecWindow::stopScript() {
	this->~ScriptExecWindow();
}
