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
	\file scriptRemoteExecWindow.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implements the ScriptRemoteExecWindow class
*/

#include <scriptRemoteExecWindow.h>

using namespace std;

/**
	Constructor, it will create a new script in order to run
	run the selected script with the given parameters. It will create a temporary M file containing
	- The parameters needed
	- a cd command to go in the right directory
	- a call to the program
	- a final printf to warn that everything is done
	- a 'pause' call so that octave will close when we kill its process.
  
  and then runs the job scheduler on the server.
  
  \param basescript basename of the script (without .m)
  \param currentScript Descriptor of the script you want to launch
  \param p Vector of parameter
  \param session SSH session
*/
ScriptRemoteExecWindow::ScriptRemoteExecWindow(std::string basescript, ScriptDescriptor currentScript, std::vector<Parameter> p, ssh_session session) {
	set_title(currentScript.getName() + " [remote]");
	//set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
  set_default_size(500,500);
  set_size_request(500,500);
  
  SSHSession = session;
	
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
	stopButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));

	//Création d'un conteneur ayant des barres de défilement.
	scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
	scrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolledWindow->add(*logTextView);
	
	mainLayout->add(*scrolledWindow);
	mainLayout->pack_end(*stopButton, Gtk::PACK_SHRINK);
	
	stopButton->signal_clicked().connect(sigc::mem_fun(*this, &ScriptRemoteExecWindow::stopScript));
	this->signal_hide().connect(sigc::mem_fun(*this, &ScriptRemoteExecWindow::stopScript));
	timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &ScriptRemoteExecWindow::onTimer), 1000);
	
	add(*mainLayout);
	
	show_all();
	
	// All will be put in an igafemtmp directory
	string base = random_string(10);
	string fname = "igafemtmp/" + base + "/script.m";
	string jname = "igafemtmp/" + base + "/job.sh";
	string xmlname = "igafemtmp/" + base + "/job.xml";
	
	string output = "igafemtmp/" + base + "/output";
	
	createDirectory("igafemtmp");
	createDirectory("igafemtmp/" + base);
	
	ostringstream out("");
	ostringstream sc("");//The script
	ostringstream job(""); //The job
	ostringstream xml(""); //The xml file
	
	xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	xml << "<job>" << endl;
	xml << "\t<script>" << currentScript.getName() << "</script>" << endl;
	xml << "\t<desc>" << currentScript.getDescription() << "</desc>" << endl;
	xml << "\t<submitted>" << getTime() << "</submitted>" << endl;
	xml << "</job>" << endl;
	
	
	job << ""
	"#!/bin/bash --login" << endl << \
	"#BSUB -x                 # give this job exclusive access" << endl << \
	"#BSUB -n 1                         # give this job 1 core" << endl << \
	"#BSUB -o " << output << ".stdout          # put the output stream here" << endl << \
	"#BSUB -e " << output << ".stderr           # put the error stream here" << endl << \
	"#BSUB -J " << base << "                     # give the job a name" << endl << \
	"#BSUB -W 03:00     # run the job for no more than 3 hours" << endl << \
	"#BSUB -R \"span[ptile=12]\"       # fully populate the node" << endl << \
	"#BSUB -q q_cf_htc_work    # run on the cardiff htc system" << endl << \
	"" << endl << \
	"# Load the Environment" << endl << \
	"module purge                   # purge any loaded modules" << endl << \
	"module load lapack/3.4.2-gnu-4.6.2 octave # Loading octave" << endl << \
	"# Run the Program" << endl << \
	"octave ~/" << fname << endl;

	out << "---------------------------------------------" << endl;
	out << "> Exporting the variables to the script" << endl;
	out << "---------------------------------------------" << endl;
	out << "     Name             |          Value       " << endl;
	out << "---------------------------------------------" << endl;
	for(unsigned int i = 0; i < p.size(); i++) {
		out << setw(21) << right << p.at(i).name << right << " |" << setw(21) << p.at(i).value << endl;
	}
	out << "---------------------------------------------" << endl;
	sc << "clear all;" << endl;
	sc << "restoredefaultpath();" << endl;
	sc << "igaoutputroot = '" << executeRemoteCommand("echo -n $HOME") << "/igafemtmp/" << base << "'" << endl;
	//if(currentScript.getGeometryDirectory() != "")
		sc << "cd " + basescript + "/" + currentScript.getGeometryDirectory() << endl;
	if(currentScript.getGeometry() != "")
		sc << currentScript.getGeometry() << endl;
	sc << "cd " + basescript + "/" + currentScript.getDirectory() << endl;
	for(unsigned int i = 0; i < p.size(); i++) { // Export variables !
		sc << p.at(i).name << " = " << p.at(i).value << ";" << endl;
	}
	sc << currentScript.getName() << endl;
	sc << "printf('---------------------------------------------\\n')" << endl;
	sc << "printf('              SCRIPT HAS ENDED               \\n')" << endl;
	sc << "printf('---------------------------------------------\\n')" << endl;
		
	out << "> Writing the script file" << endl;
	createFile(fname, sc.str(), out);
	out << "> Writing the job file" << endl;
	createFile(jname, job.str(), out);
	out << "> Writing the XML job info file" << endl;
	createFile(xmlname, xml.str(), out);
	out << "---------------------------------------------" << endl;
	
	ostringstream cmd("");
	
	cmd << "source /etc/profile.d/lsf.sh && bsub < ";
	cmd << "igafemtmp/" + base + "/job.sh && mv ";
	cmd << "igafemtmp/" + base + "/job.sh ";
	cmd << "igafemtmp/" + base + "/jobSubmitted.sh;" << endl;
	
	out << executeRemoteCommand(cmd.str()) << endl;
	
	logBuffer->set_text(out.str());
	
	
}

/**
	Timed actions. It checks if the process has new data output and append it to
	the log display. It also check if the process is still alive.
*/
bool ScriptRemoteExecWindow::onTimer() {
		
	
	return true;
}

/**
	Creates a directory
	\param[in] dname directory name
*/
void ScriptRemoteExecWindow::createDirectory(std::string dname) {
	sftp_session sftp;
	int rc;
	
	// New sftp session
	sftp = sftp_new(SSHSession);
	
	rc = sftp_mkdir(sftp, dname.c_str(), S_IRWXU);
	if (rc != SSH_OK) {
		if (sftp_get_error(sftp) != SSH_FX_FILE_ALREADY_EXISTS)	{
			fprintf(stderr, "Can't create directory: %s\n", ssh_get_error(SSHSession));
			return;
		}
	}
	
	sftp_free(sftp);
}

/**
	Creates a file with some string as it's content
	\param[in] fname file name
	\param[in] s content
	\param[in] out log
*/
void ScriptRemoteExecWindow::createFile(std::string fname, std::string s, ostringstream &out) {
	int access_type = O_WRONLY | O_CREAT | O_TRUNC;
	sftp_file file;
	sftp_session sftp;
	int rc, nwritten;
	
	// New sftp session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		out << "Error allocating SFTP session: " << ssh_get_error(SSHSession) << endl;
		return;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		out << "Error initializing SFTP session: " << sftp_get_error(sftp) << endl;
		sftp_free(sftp);
		return;
	}
	
	// Write a file
	
	file = sftp_open(sftp,fname.c_str(), access_type, S_IRWXU);
	
	if (file == NULL)	{
		out << "Can't open file for writing: " << ssh_get_error(SSHSession) << endl;
		return;
	}
	
	nwritten = sftp_write(file, s.c_str(), s.length());
	
	if (nwritten != (int)s.length())	{
		out << "Can't write data to file: " << ssh_get_error(SSHSession) << endl;
		sftp_close(file);
		return;
	}
	
	rc = sftp_close(file);
	
	if (rc != SSH_OK)	{
		out << "Can't close the written file: " << ssh_get_error(SSHSession) << endl;
		return;
	}
	
	out << "> File " << fname << " written !" << endl;
	sftp_free(sftp);
	return;
}

/**

*/
ScriptRemoteExecWindow::~ScriptRemoteExecWindow() {
	
}

/**
	Executes a command on the remote server.
	Shamelessly inspired of the
	[official documentation](http://api.libssh.org/stable/libssh_tutor_guided_tour.html)
	of libssh
	\param[in] command the command to execute
	\return the textual output of the command
*/
std::string ScriptRemoteExecWindow::executeRemoteCommand(std::string command) {
	ssh_channel channel;
	int rc;
	char buffer[256];
	unsigned int nbytes;
	channel = ssh_channel_new(SSHSession);
	if (channel == NULL)
		return "Cannot open channel";
		
	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK) {
		ssh_channel_free(channel);
		return "Error when calling the command";
	}
	rc = ssh_channel_request_exec(channel, command.c_str());
	if (rc != SSH_OK) {
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return "Error when calling the command";
	}
	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	string response;
	while (nbytes > 0) {
		// no write
		if (write(1, buffer, nbytes) != nbytes) { 
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			return "Error when calling the command";
		}
	  response += string(buffer).substr(0, nbytes);
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}
	
	if (nbytes < 0) {
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return "Error when calling the command";
	}
	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
	return response;
}

/**
	destroys the window
*/
void ScriptRemoteExecWindow::stopScript() {
	this->~ScriptRemoteExecWindow();
}

/**
	Returns the current time, taken
	from [Wikipedia](https://en.wikipedia.org/wiki/C_date_and_time_functions#Example)
*/
std::string ScriptRemoteExecWindow::getTime() {
	time_t current_time;
	char* c_time_string;
 
	/* Obtain current time as seconds elapsed since the Epoch. */
	current_time = time(NULL);
 
	if (current_time == ((time_t)-1))
	{
		(void) fprintf(stderr, "Failure to compute the current time.");
		return "unknown";
	}
 
	/* Convert to local time format. */
	c_time_string = ctime(&current_time);
	
	return string(c_time_string).substr(0, string(c_time_string).length()-1); // to remove \n
}
