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
	\file getResultWindow.cpp
	\version 0.1
	\author Thomas Maurice
	\brief Implementation of the class GetResultWindow
*/

#include <getResultWindow.h>


using namespace std;

/**
	Basic constructor for the window
*/
GetResultWindow::GetResultWindow(ssh_session session) {
	set_title("Download results");
	set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
  set_default_size(300,200);
  set_size_request(300,200);
  
	SSHSession = session;
	
	mainLayout = Gtk::manage(new Gtk::VBox);
	
	Gtk::Label* l = Gtk::manage(new Gtk::Label("Select a result :"));
	
	Gtk::HBox *hb = Gtk::manage(new Gtk::HBox());
	
	infoLabel = Gtk::manage(new Gtk::Label());
	infoLabel->set_markup("");
	infoLabel->set_justify(Gtk::JUSTIFY_FILL);
	infoLabel->set_line_wrap();
	infoLabel->set_margin_left(10);
	infoLabel->set_margin_right(10);
	
	resultSelect = Gtk::manage(new Gtk::ComboBoxText());
	refreshButton = Gtk::manage(new Gtk::Button("Refresh the list"));
	removeButton = Gtk::manage(new Gtk::Button("Remove the result"));
	showStdout = Gtk::manage(new Gtk::Button("View stdout"));
	showStderr = Gtk::manage(new Gtk::Button("View stderr"));
	downloadButton = Gtk::manage(new Gtk::Button("Download"));
	
	hb->add(*showStdout);
	hb->add(*showStderr);
	
	mainLayout->pack_start(*l, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*resultSelect, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*refreshButton, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*hb, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*infoLabel);
	mainLayout->pack_end(*removeButton, Gtk::PACK_SHRINK);
	mainLayout->pack_end(*downloadButton, Gtk::PACK_SHRINK);
	add(*mainLayout);
	
	show_all();
	
	resultSelect->signal_changed().connect(sigc::mem_fun(*this, &GetResultWindow::viewResult));
	refreshButton->signal_clicked().connect(sigc::mem_fun(*this, &GetResultWindow::loadResults));
	removeButton->signal_clicked().connect(sigc::bind<std::string, int>(sigc::mem_fun(*this, &GetResultWindow::removeDirectory), resultSelect->get_active_text(), 0));
	showStdout->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &GetResultWindow::showOutputStream), 0));
	showStderr->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &GetResultWindow::showOutputStream), 1));
	downloadButton->signal_clicked().connect(sigc::mem_fun(*this, &GetResultWindow::download));
	
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &GetResultWindow::onTimer), 2000);
	loadResults();
}

/**
	Show an output stream, 0 is for STDOUT and 1 is for STDERR
	\param[in] stream stream to watch
*/
void GetResultWindow::showOutputStream(int stream) {
	if(stream != 0 && stream != 1) return;
	if(stream == 0) {
		string s = readRemoteFileToString("igafemtmp/" + resultSelect->get_active_text() + "/output.stdout");
		LogWindow *l = new LogWindow(resultSelect->get_active_text() + " - stdout", s);
		l->show();
	} else {
		string s = readRemoteFileToString("igafemtmp/" + resultSelect->get_active_text() + "/output.stderr");
		LogWindow *l = new LogWindow(resultSelect->get_active_text() + " - stderr", s);
		l->show();
	}
}

/**
	Downloads the remote result
*/
void GetResultWindow::download() {
	Gtk::FileChooserDialog dialogue(*this, "Select the directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialogue.set_create_folders();
	dialogue.set_current_folder(Glib::get_home_dir());
	dialogue.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialogue.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	int r = dialogue.run();
	if(r == Gtk::RESPONSE_OK) {
		make_directory(dialogue.get_filename() + "/" + resultSelect->get_active_text());
		copyDirectory("igafemtmp/" +resultSelect->get_active_text(), dialogue.get_filename() + "/" + resultSelect->get_active_text());
		info("Done", "Copy done", "The copying operation is now complete");
	}
}

/**
	Pops up an info message
	
	\param[in] title The info window title
	\param[in] summary The error summary
	\param[in] body A developped version of the warning message. Accepts Pango Markup language.
*/
void GetResultWindow::info(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, true, Gtk::MESSAGE_INFO);
	//Comme cette classe hérite de Gtk::Window, il est possible d'appeler des méthodes de cette dernière classe.
	dialogue.set_title(title);
	//Ajouter un texte secondaire.
	dialogue.set_secondary_text(body, true);
	dialogue.run(); //Lancer la boîte de dialogue.
}

/**
	Copies recursively a directory
	\param[in] remote remote directory
	\param[in] local local directory
*/
void GetResultWindow::copyDirectory(std::string remote, std::string local) {
	sftp_session sftp;
	sftp_dir dir;
	sftp_attributes attributes;
	int rc;
	
	// Open a new SFTP session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(SSHSession));
		return;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		fprintf(stderr, "Error initializing SFTP session: %d.\n", sftp_get_error(sftp));
		sftp_free(sftp);
		return;
	}
	
	// List the files
	dir = sftp_opendir(sftp, remote.c_str());
	if (!dir) {
		fprintf(stderr, "Directory not opened: %s\n", ssh_get_error(SSHSession));
		return;
	}

	while ((attributes = sftp_readdir(sftp, dir)) != NULL) {
		string file = attributes->name;
		
		if(file != "." && file != "..") {
			if(attributes->type == SSH_FILEXFER_TYPE_DIRECTORY) {
				make_directory(local + "/" + file);
				copyDirectory(remote + "/" + file, local + "/" + file);
			}
			copyRemoteFile(remote + "/" + file, local + "/" + file);
		}
		sftp_attributes_free(attributes);
	}
	
	if (!sftp_dir_eof(dir)) {
		fprintf(stderr, "Can't list directory: %s\n", ssh_get_error(SSHSession));
		sftp_closedir(dir);
		return;
	}
	// Close the directory
	rc = sftp_closedir(dir);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close directory: %s\n", ssh_get_error(SSHSession));
		return;
	}
	
	// Close the sftp context
	sftp_free(sftp);
}

/**
	Refreshes the info about the currently tracked task, to check it's state
	change (i.e. RUN -> DONE)
*/
bool GetResultWindow::onTimer() {
	viewResult();
	return true;
}

/**
	Pops up a warning message
	
	\param[in] title The warning window title
	\param[in] summary The error summary
	\param[in] body A developped version of the warning message. Accepts Pango Markup language.
*/
void GetResultWindow::warn(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, true, Gtk::MESSAGE_WARNING);
	//Comme cette classe hérite de Gtk::Window, il est possible d'appeler des méthodes de cette dernière classe.
	dialogue.set_title(title);
	//Ajouter un texte secondaire.
	dialogue.set_secondary_text(body, true);
	dialogue.run(); //Lancer la boîte de dialogue.
}

/**
	(Re)loads all the results available
*/
void GetResultWindow::loadResults() {
	resultSelect->remove_all();
	
	sftp_session sftp;
	sftp_dir dir;
	sftp_attributes attributes;
	int rc;
	
	// Open a new SFTP session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(SSHSession));
		return;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		fprintf(stderr, "Error initializing SFTP session: %d.\n", sftp_get_error(sftp));
		sftp_free(sftp);
		return;
	}
	
	// List the files
	dir = sftp_opendir(sftp, "igafemtmp");
	if (!dir) {
		fprintf(stderr, "Directory not opened: %s\n", ssh_get_error(SSHSession));
		return;
	}

	while ((attributes = sftp_readdir(sftp, dir)) != NULL) {
		string file = attributes->name;
		
		if(file != "." && file != "..") {
			resultSelect->append(file);
			resultSelect->set_active_text(file);
		}
		sftp_attributes_free(attributes);
	}
	
	if (!sftp_dir_eof(dir)) {
		fprintf(stderr, "Can't list directory: %s\n", ssh_get_error(SSHSession));
		sftp_closedir(dir);
		return;
	}
	// Close the directory
	rc = sftp_closedir(dir);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close directory: %s\n", ssh_get_error(SSHSession));
		return;
	}
	
	// Close the sftp context
	sftp_free(sftp);
}

void GetResultWindow::downloadResult() {

}

std::string GetResultWindow::readRemoteFileToString(std::string f) {
	sftp_session sftp;
	int access_type;
	sftp_file file;
	char buffer[1024];
	int nbytes, rc;
	std::string result;
	
	// Open a new SFTP session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(SSHSession));
		return "";
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		fprintf(stderr, "Error initializing SFTP session: %d.\n", sftp_get_error(sftp));
		sftp_free(sftp);
		return "";
	}
	
	// Read the config file
	access_type = O_RDONLY;
	file = sftp_open(sftp, f.c_str(), access_type, 0);
	
	if (file == NULL) {
		fprintf(stderr, "Can't open file for reading: %s\n", ssh_get_error(SSHSession));
		return "";
	}
	nbytes = sftp_read(file, buffer, sizeof(buffer));
	
	while (nbytes > 0) {
		result += string(buffer).substr(0, nbytes);
		nbytes = sftp_read(file, buffer, sizeof(buffer));
	}
	
	if (nbytes < 0) {
		fprintf(stderr, "Error while reading file: %s\n", ssh_get_error(SSHSession));
		sftp_close(file);
		return "";
	}
	rc = sftp_close(file);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close the read file: %s\n", ssh_get_error(SSHSession));
		return "";
	}
	
	sftp_free(sftp);
	return result;
}

/**
	Copies a file from the remote server to the local computer
	\param[in] f The remote file
	\param[in] outf The local file
*/
void GetResultWindow::copyRemoteFile(std::string f, std::string outf) {
	sftp_session sftp;
	int access_type;
	sftp_file file;
	char buffer[1024];
	int nbytes, rc;
	std::string result;
	FILE* lfile = fopen(outf.c_str(), "wb");
	
	
	// Open a new SFTP session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(SSHSession));
		fclose(lfile);
		return ;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		fprintf(stderr, "Error initializing SFTP session: %d.\n", sftp_get_error(sftp));
		fclose(lfile);
		sftp_free(sftp);
		return;
	}
	
	access_type = O_RDONLY;
	file = sftp_open(sftp, f.c_str(), access_type, 0);
	
	if (file == NULL) {
		fprintf(stderr, "Can't open file for reading: %s\n", ssh_get_error(SSHSession));
		fclose(lfile);
		return;
	}
	nbytes = sftp_read(file, buffer, sizeof(buffer));
	
	while (nbytes > 0) {
		fwrite(buffer, sizeof(char), nbytes, lfile);
		nbytes = sftp_read(file, buffer, sizeof(buffer));
	}
	
	if (nbytes < 0) {
		fprintf(stderr, "Error while reading file: %s\n", ssh_get_error(SSHSession));
		fclose(lfile);
		sftp_close(file);
		return;
	}
	rc = sftp_close(file);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close the read file: %s\n", ssh_get_error(SSHSession));
		fclose(lfile);
		return;
	}
	
	fclose(lfile);
	sftp_free(sftp);
	return;
}

void GetResultWindow::viewResult() {
	if(resultSelect->get_active_text() == "") {
		infoLabel->set_text("");
		return;	
	}
	
	currentDesc = "";
	currentSub = "";
	
	string job = readRemoteFileToString("igafemtmp/" + resultSelect->get_active_text() + "/job.xml");
	
	xmlpp::DomParser parser;
	parser.set_throw_messages(true);
	parser.set_substitute_entities(true);
	try {
		parser.parse_memory(job);
	} catch (exception e) {
		warn("Unable to load job info", "Unable to load the job info",
		     string("The file <b>~/igafemtmp/") + string(resultSelect->get_active_text()) + \
		     string("/job.xml</b> was not found on the server :("));
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
				const xmlpp::ContentNode* nodeText = dynamic_cast<const xmlpp::ContentNode*>(elem->get_first_child());
				if(nodeText) {
					if(name == "script") {
						currentScript = nodeText->get_content();
					} else if(name == "submitted") {
						currentSub = nodeText->get_content();
					} else if(name == "desc") {
						currentDesc = nodeText->get_content();
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
		
		std::string state = executeRemoteCommand("source /etc/profile.d/lsf.sh &&\
			bjobs -a | grep " + resultSelect->get_active_text() + " | awk '{print $3}'");
		
		if(state == "") {
			state = "DONE";
		}		 
		
		infoLabel->set_markup("<b>Job ID : </b>" + resultSelect->get_active_text() + "\n" +\
		"<b>Launched script : </b>" + currentScript + "\n<b>Submitted : </b> " + currentSub + "\n" +\
		"<b>State : </b>" + state + "\n\n"+\
		"<b>Description : </b>" + currentDesc);
		
  }
}

/**
	Recursively removes a result's directory. If no parameter is provided it will
	remove the directory of the selected result.
	
	\param[in] directory The directory to remove
	\param[in] level Recursion level, is 0 when toplevel.
*/
void GetResultWindow::removeDirectory(std::string directory, int level) {
	if(directory == "")
		directory = "igafemtmp/" + resultSelect->get_active_text();

	sftp_session sftp;
	sftp_dir dir;
	sftp_attributes attributes;
	int rc;
	
	// Open a new SFTP session
	sftp = sftp_new(SSHSession);
	
	if (sftp == NULL) {
		fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(SSHSession));
		return;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK) {
		fprintf(stderr, "Error initializing SFTP session: %d.\n", sftp_get_error(sftp));
		sftp_free(sftp);
		return;
	}
	
	// List the files
	dir = sftp_opendir(sftp, directory.c_str());
	if (!dir) {
		fprintf(stderr, "Directory not opened: %s\n", ssh_get_error(SSHSession));
		return;
	}

	while ((attributes = sftp_readdir(sftp, dir)) != NULL) {
		string file = attributes->name;
		
		if(file != "." && file != "..") {
			if(attributes->type == SSH_FILEXFER_TYPE_DIRECTORY) {
				removeDirectory(directory + "/" + file, level+1);
				sftp_rmdir(sftp,string(directory + "/" + file).c_str());
			}
			sftp_unlink(sftp,string(directory + "/" + file).c_str());
		}
		sftp_attributes_free(attributes);
	}
	
	sftp_rmdir(sftp,string(directory).c_str());
	if(level == 0)
		loadResults();
	
	if (!sftp_dir_eof(dir)) {
		fprintf(stderr, "Can't list directory: %s\n", ssh_get_error(SSHSession));
		sftp_closedir(dir);
		return;
	}
	// Close the directory
	rc = sftp_closedir(dir);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close directory: %s\n", ssh_get_error(SSHSession));
		return;
	}
	
	// Close the sftp context
	sftp_free(sftp);
}

/**
	Executes a command on the remote server.
	Shamelessly inspired of the
	[official documentation](http://api.libssh.org/stable/libssh_tutor_guided_tour.html)
	of libssh
	\param[in] command the command to execute
	\return the textual output of the command
*/
std::string GetResultWindow::executeRemoteCommand(std::string command) {
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
		/*if (write(1, buffer, nbytes) != nbytes) { 
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			return "Error when calling the command";
		}*/
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
