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
	\file mainWindow.cpp
	\version 0.2
	\author Thomas Maurice
	\brief Implementation of the class MainWindow
*/

#include <mainWindow.h>

using namespace std;

/**
	Constructor of the window, creates a new Window and loads the config file
	located in $HOME/.igafemgui/config.xml.
	Then it loads all the available metadata files located in $HOME/.igafemgui/files
	Then it pops up.
*/
MainWindow::MainWindow() {
	set_title("IsoGeometric Analysis FEM - Graphical UI");
	set_position(Gtk::WIN_POS_CENTER);
	set_resizable(true);
  set_default_size(400,300);
  set_size_request(400,300);
  
  SSHSession = NULL;
  /*
  	Data
  */
  onRemote = false;
  homedir = getenv("HOME");
  // Checking if the config dir exists
  if(!directory_exists(homedir + "/.igafemgui")) {
  	cout << "Creating " << homedir + "/.igafemgui" << endl;
  	make_directory(homedir + "/.igafemgui");
  	make_directory(homedir + "/.igafemgui/files");
  	cout << "Creating sample config..." << endl;
  	ofstream f(string(homedir + "/.igafemgui/config.xml").c_str(), ios::out | ios::trunc);
  	if(f) {
  		f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<config>\n\
\t<basefem>" << homedir << "/IsogeometricalAnalysis</basefem>\n\
\t<basedat>" << homedir << "/.igafemgui/files</basedat>\n\
</config>";
			f.close();
  	} else {
  		cout << "Cannot open config file" << endl;
  	}
  }
  
  loadConfiguration();
  char buf[512];
  getcwd(buf, 512);
  currentwd = buf;

	/*
		Layout initialisations
	*/
	
	mainLayout = Gtk::manage(new Gtk::VBox());
	
	/*
		Input initialisation
	*/
	scriptSelect = Gtk::manage(new Gtk::ComboBoxText());
	
	loadAvailableScripts();
	
	runButton = Gtk::manage(new Gtk::Button("Run the selected script"));
	
	/*
		Displays
	*/
	Gtk::HBox *hb1 = Gtk::manage(new Gtk::HBox());
	Gtk::HBox *hb2 = Gtk::manage(new Gtk::HBox());
	Gtk::Label* geom = Gtk::manage(new Gtk::Label("Geometry file (without .m) : "));
	Gtk::Label* geomd = Gtk::manage(new Gtk::Label("Geometry file directory : "));
	geometryEntry = Gtk::manage(new Gtk::Entry());
	geometryDirEntry = Gtk::manage(new Gtk::Entry());
	
	connInfo = Gtk::manage(new Gtk::Label(""));
	connInfo->set_markup("<span foreground=\"orange\">Offine</span>. You are currently offline.");
	connInfo->set_justify(Gtk::JUSTIFY_CENTER);
	connInfo->set_line_wrap();
	connInfo->set_margin_left(10);
	connInfo->set_margin_right(10);
	
	scriptDesc = Gtk::manage(new Gtk::Label());
	scriptDesc->set_markup("<b>ScriptName</b>: Some description");
	scriptDesc->set_justify(Gtk::JUSTIFY_FILL);
	scriptDesc->set_line_wrap();
	scriptDesc->set_margin_left(10);
	scriptDesc->set_margin_right(10);
	
	params = Gtk::manage(new ParameterSet());
	
	/*
		Menubar
	*/
	
	makeMenus();
	
	/*
		Configuring the layouts
	*/
	
	hb1->set_spacing(5);
	hb2->set_spacing(5);
	mainLayout->set_spacing(5);
	
	hb1->add(*geomd);
	hb1->add(*geometryDirEntry);
	hb2->add(*geom);
	hb2->add(*geometryEntry);
	
	// And the main one
	mainLayout->pack_start(*menuBar, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*connInfo, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*scriptSelect, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*scriptDesc, Gtk::PACK_SHRINK, 5);
	mainLayout->pack_start(*hb1, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*hb2, Gtk::PACK_SHRINK);
	mainLayout->pack_start(*params, Gtk::PACK_SHRINK);
	mainLayout->pack_end(*runButton, Gtk::PACK_SHRINK);
	
	// Adding the main layout
	add(*mainLayout);
	
	/*
		Signal connections
	*/
	
	runButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::runScript));
	scriptSelect->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::dispScript));
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::onTimer), 1000);
	
	/*
		Data
	*/
	
	currentScript = new ScriptDescriptor();
	
	// Here we go !
	
	dispScript();
	
	show_all();
}

/**
	USeless now, can be useful if someone want to implement times actions.
*/
bool MainWindow::onTimer() {
	menuItemConf->set_sensitive(!onRemote);
	menuItemScripts->set_sensitive(!onRemote);
	return true;
}

/**
	Creates all the menus for the window and add them. This function was created
	not to make the contructor too long.
*/
void MainWindow::makeMenus() {
	// The menu bar
	menuBar = Gtk::manage(new Gtk::MenuBar());
	
	// The 'file' menu
	Gtk::ImageMenuItem *menuItemFile = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::FILE));
	menuBar->append(*menuItemFile);
	// Menu inside 'file'
	Gtk::Menu *menuFile = Gtk::manage(new Gtk::Menu());
	menuItemFile->set_submenu(*menuFile);
	// Items
	
	Gtk::ImageMenuItem *menuQuit = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::QUIT));
	menuFile->append(*menuQuit);
	Gtk::ImageMenuItem *menuAbout = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::ABOUT));
	menuFile->append(*menuAbout);
	
	// The 'scripts' menu
	menuItemScripts = Gtk::manage(new Gtk::ImageMenuItem("Scripts"));
	menuBar->append(*menuItemScripts);
	// Menu inside 'file'
	Gtk::Menu *menuScripts = Gtk::manage(new Gtk::Menu());
	menuItemScripts->set_submenu(*menuScripts);
	// Items
	Gtk::ImageMenuItem *menuRefresh = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::REFRESH));
	menuScripts->append(*menuRefresh);
	Gtk::ImageMenuItem *menuNew = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::NEW));
	menuScripts->append(*menuNew);
	Gtk::MenuItem *menuEdit = Gtk::manage(new Gtk::MenuItem("_Edit the current script info", true));
	menuScripts->append(*menuEdit);
	Gtk::MenuItem *menuDelete = Gtk::manage(new Gtk::MenuItem("_Delete the current script info", true));
	menuScripts->append(*menuDelete);
	
	// The 'configuration' menu
	menuItemConf = Gtk::manage(new Gtk::ImageMenuItem("_Configuration", true));
	menuBar->append(*menuItemConf);
	// Menu inside 'file'
	Gtk::Menu *menuConf = Gtk::manage(new Gtk::Menu());
	menuItemConf->set_submenu(*menuConf);
	// Items
	Gtk::ImageMenuItem *menuEditConf = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::EDIT));
	menuConf->append(*menuEditConf);
	
	
	// The 'Network' menu
	Gtk::ImageMenuItem *menuItemNetwork = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::NETWORK));
	menuBar->append(*menuItemNetwork);
	// Menu inside 'Network'
	Gtk::Menu *menuNetwork = Gtk::manage(new Gtk::Menu());
	menuItemNetwork->set_submenu(*menuNetwork);
	Gtk::ImageMenuItem *menuConnect = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::CONNECT));
	menuNetwork->append(*menuConnect);
	Gtk::ImageMenuItem *menuGetResults = Gtk::manage(new Gtk::ImageMenuItem("Do_wnload results", true));
	menuNetwork->append(*menuGetResults);
	Gtk::ImageMenuItem *menuDisconnect = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::DISCONNECT));
	menuNetwork->append(*menuDisconnect);
	
	// Signals
	menuQuit->signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
	menuRefresh->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::loadAvailableScripts));
	menuEditConf->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::editConfiguration));
	menuEdit->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::editMetadata));
	menuNew->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::newMetadata));
	menuDelete->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::removeCurrent));
	menuAbout->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::about));
	menuConnect->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::connectToRemote));
	menuDisconnect->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::disconnectFromRemote));
	menuGetResults->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::getResults));
}

/**
	Deletes every dynamically allocated stuff to avoid memory leaks
*/
MainWindow::~MainWindow() {
	delete currentScript;
}

/**
	Displays an AboutWindow
*/
void MainWindow::about() {
	AboutWindow a;
	a.run();
}

/**
	Ask a question
	
	\param[in] title The question window title
	\param[in] summary question summary
	\param[in] body A developped version of the question message. Accepts Pango Markup language.
*/
int MainWindow::question(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	dialogue.set_title(title);
	dialogue.set_secondary_text(body);
     
	return dialogue.run();
}

/**
	Checks if the remote host is known and ask the user if we should trust him.
	This function must be called in the process of connection to ensure that the host
	is the correct one.
	Shamelessly inspired of the
	[official documentation](http://api.libssh.org/stable/libssh_tutor_guided_tour.html)
	of libssh
*/
bool MainWindow::checkRemoteHost() {
	int state, hlen;
	unsigned char *hash = NULL;
	char *hexa;
	int ans;
	state = ssh_is_server_known(SSHSession);
	hlen = ssh_get_pubkey_hash(SSHSession, &hash);
	if (hlen < 0)
		return false;
	switch (state) {
		case SSH_SERVER_KNOWN_OK:
			break; /* ok */
		case SSH_SERVER_KNOWN_CHANGED:
			hexa = ssh_get_hexa(hash, hlen);
			warn("Key changed !", "Host key for server changed: it is now:\n" + string(hexa), "For security reasons, connection will be stopped.");
			free(hexa);
			free(hash);
			return false;
		case SSH_SERVER_FOUND_OTHER:
			warn("Key changed !", "The host key for this server was not found but an other"
			"type of key exists.\n", "An attacker might change the default server key to"
			"confuse your client into thinking the key does not exist\n");
			free(hash);
			return false;
		case SSH_SERVER_FILE_NOT_FOUND:
			warn("Could not find known host file.", "If you accept the host key here, the file will be"
			"automatically created.\n");
		/* fallback to SSH_SERVER_NOT_KNOWN behavior */
		case SSH_SERVER_NOT_KNOWN:
				hexa = ssh_get_hexa(hash, hlen);
				ans = question("Server unknown", "The server is unknown. Do you trust the host key?", "Public key hash: " + string(hexa));
				free(hexa);
				if(ans != Gtk::RESPONSE_YES) {
					free(hash);
					return false;
				}
				if (ssh_write_knownhost(SSHSession) < 0) {
					warn("Key writting error", strerror(errno));
					free(hash);
					return false;
				}
				break;
		case SSH_SERVER_ERROR:
			warn("Server error", ssh_get_error(SSHSession));
			free(hash);
			return false;
	}
	free(hash);
	return true;
}

/**
	Will connect you to a remote server to run scripts on vit via SSH.
	Only the scripts uploaded on the server will be run, not yours.
	Shamelessly inspired of the
	[official documentation](http://api.libssh.org/stable/libssh_tutor_guided_tour.html)
	of libssh
*/
void MainWindow::connectToRemote() {
	std::string u=defuser;
	std::string h=defhost;
	std::string pw="";
	int p=22;
	// Getting the connect info.
	DialogConnect d(&u, &pw, &h, &p);
	int r = d.run();
	if(r != Gtk::RESPONSE_OK)
		return;
		
	// Delete everything in the combobox.
	scriptSelect->remove_all();

	// Connect the SSH socket
	if(SSHSession != NULL)
		disconnectFromRemote();
		
	SSHSession = ssh_new();
	if(!SSHSession) {
		warn("SSH error", "We were unable to create the SSH context", ssh_get_error(SSHSession));
		SSHSession = NULL;
		loadAvailableScripts();
		return;
	}

	int verb = 0;//SSH_LOG_PROTOCOL;
	int rc;
	
	ssh_options_set(SSHSession, SSH_OPTIONS_HOST, h.c_str());
	ssh_options_set(SSHSession, SSH_OPTIONS_PORT, &p);
	ssh_options_set(SSHSession, SSH_OPTIONS_LOG_VERBOSITY, &verb);
	ssh_options_set(SSHSession, SSH_OPTIONS_USER, u.c_str());

	rc = ssh_connect(SSHSession);

	if(rc != SSH_OK) {
		warn("Connection error", "We were unable to connect to " + h, ssh_get_error(SSHSession));
		SSHSession = NULL;
		loadAvailableScripts();
		return;
	}

	if(checkRemoteHost() != true) {
		warn("Untrusted host", "I was not able to identify the remote host. :(", "For security reasons, connection will be stopped.");
		SSHSession = NULL;
		loadAvailableScripts();
		return;
	}

	rc = ssh_userauth_password(SSHSession, NULL, pw.c_str());

	if(rc != SSH_AUTH_SUCCESS) {
		warn("Authentification error", "We were unable to identify as " + u + " on " + h, ssh_get_error(SSHSession));
		disconnectFromRemote();
		loadAvailableScripts();
		return;
	}
	
	info("Connection successful", "We are connected to " + h);
	onRemote = true;
	
	connInfo->set_markup("<span foreground=\"orange\">Connected</span> as <b>" + u + "</b> on <b>" + h + "</b>");
	
	loadConfiguration();
}

/**
	Disconnects the software from the remote server.
*/
void MainWindow::disconnectFromRemote() {
	if(SSHSession) {
		ssh_disconnect(SSHSession);
		ssh_free(SSHSession);
		info("Disconnected from the server", "You are now disconnected from remote host");
		onRemote = false;
		
  	SSHSession = NULL;
	}
	connInfo->set_markup("<span foreground=\"orange\">Offine</span>. You are currently offline.");
	onRemote = false;
	loadConfiguration();
	loadAvailableScripts();
}

/**
	Executes a command on the remote server.
	Shamelessly inspired of the
	[official documentation](http://api.libssh.org/stable/libssh_tutor_guided_tour.html)
	of libssh
	\param[in] command the command to execute
	\return the textual output of the command
*/
std::string MainWindow::executeRemoteCommand(std::string command) {
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
	Destroys the currently loaded scriptinfo. Will ask for confirmation
*/
void MainWindow::removeCurrent() {
	Gtk::MessageDialog dialogue(*this, "Do you want to delete the file", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	dialogue.set_title("Remove ?");
	dialogue.set_secondary_text("If you answer \"Yes\" then the current script info will be deleted. This operation shall not alter the corresponding M file.");
     
	int result = dialogue.run();
	if(result == Gtk::RESPONSE_YES) {
		if(std::remove(string(basefiles + "/" + currentScript->getName() + ".xml").c_str()) == 0) {
			cout << "File removed" << endl;
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "The file was removed !", true, Gtk::MESSAGE_INFO);
			dialogue.set_title("File removed");
			dialogue.run();
		} else {
			cout << "File failed to be removed" << endl;
			Gtk::MessageDialog dialogue((Gtk::Window&)(*this->get_toplevel()), "The file could not be removed", true, Gtk::MESSAGE_ERROR);
			dialogue.set_title("Cannot remove");
			dialogue.run();
		}
	
		loadAvailableScripts();
	}
}

/**
	Opens an EditConfWindow to edit the current configuration file.
	Then the signal hide is connected to the loadConfiguration() function
	so that when the editing is over everything is reloaded so that you
	don't need to restart the application to see your changes.
*/
void MainWindow::editConfiguration() {
	EditConfWindow *confWindow = new EditConfWindow();
	confWindow->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::loadConfiguration));
	confWindow->show();
}

/**
	Opens an EditMetadataWindow to edit the current script info
*/
void MainWindow::editMetadata() {
	EditMetadataWindow *w = new EditMetadataWindow(basefiles, currentScript->getName());
	w->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::loadAvailableScripts));
	w->show();
}

/**
	Opens an EditMetadataWindow to create a new script info
*/
void MainWindow::newMetadata() {
	EditMetadataWindow *w = new EditMetadataWindow(basefiles, "untitled");
	w->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::loadAvailableScripts));
	w->show();
}

/**
	Download scripts results from the server
*/
void MainWindow::getResults() {
	if(!onRemote) {
		warn("Not connected", "You are not connected to any remote server !", "In order to download results, you shall first log into a server");
		return;
	}
	
	GetResultWindow *w = new GetResultWindow(SSHSession);
	w->show();
}

std::string MainWindow::readRemoteFileToString(std::string f) {
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

void MainWindow::loadRemoteAvailableScripts() {
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
	dir = sftp_opendir(sftp, basefiles.c_str());
	if (!dir) {
		fprintf(stderr, "Directory not opened: %s\n", ssh_get_error(SSHSession));
		return;
	}

	while ((attributes = sftp_readdir(sftp, dir)) != NULL) {
		string file = attributes->name;
		if(file.length() > 5 && file.substr(file.length()-4) == ".xml") { // Ok, XML file
			scriptSelect->append(file);
			scriptSelect->set_active_text(file);
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
	Loads the program's configuration from remote host. If nothing is found, then
	we will output error messages
*/
void MainWindow::loadRemoteConfiguration() {
	string config = readRemoteFileToString(".igafemgui/config.xml");
	
	cout << "> Loading configuration" << endl;
	xmlpp::DomParser parser;
	parser.set_throw_messages(true);
	parser.set_substitute_entities(true);
	try {
		parser.parse_memory(config);
	} catch (exception e) {
		warn("Unable to load config", "Unable to load the remote configuration, check that it exists !");
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
					if(name == "basefem") {
						basescript = nodeText->get_content();
					} else if(name == "basedat") {
						basefiles = nodeText->get_content();
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
  	cout << "Script directory       : " << basescript << endl;
		cout << "Scripts info directory : " << basefiles << endl;
		loadRemoteAvailableScripts();
  }
}

/**
	Loads the program's configuration. If nothing is found, then a sample default
	configuration will be created and used instead. The default config places the
	config directory into ~/.igafemgui and the IGA Octave code into ~/IsogeometricAnalysis
*/
void MainWindow::loadConfiguration() {
	if(onRemote) {
		loadRemoteConfiguration();
		return;
	}
	
	cout << "> Loading configuration" << endl;
	xmlpp::DomParser parser;
	parser.set_throw_messages(true);
	parser.set_substitute_entities(true);
	
	try {
		parser.parse_file(homedir + "/.igafemgui/config.xml");
	} catch (exception e) {
		warn("Unable to load config", "Unable to load the configuration, creating it.");
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
					if(name == "basefem") {
						basescript = nodeText->get_content();
					} else if(name == "basedat") {
						basefiles = nodeText->get_content();
					} else if (name == "defhost" && !onRemote) { // No' gonna change host while on remote, yo.
						defhost = nodeText->get_content();
					} else if (name == "defuser" && !onRemote) { // No' gonna change user while on remote, yo.
						defuser = nodeText->get_content();
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
  } else {
  	cout << "Unable to open config file " << homedir + "/.igafemgui/config.xml" << endl;
  	cout << "Creating sample config..." << endl;
  	ofstream f(string(homedir + "/.igafemgui/config.xml").c_str(), ios::out | ios::trunc);
  	if(f) {
  		f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<config>\n\
\t<basefem>" << homedir << "/IsogeometricalAnalysis</basefem>\n\
\t<basedat>" << homedir << "/.igafemgui/files</basedat>\n\
\t<defhost>" << "example.fr" << "</defhost>\n\
\t<defuser>" << "user" << "</defuser>\n\
</config>";
			f.close();
			loadConfiguration();
  	}
	}
	cout << "Script directory       : " << basescript << endl;
	cout << "Scripts info directory : " << basefiles << endl;
}

/**
	Pops up an error message and exits the program
	
	\param[in] title The error window title
	\param[in] summary The error summary
	\param[in] body A developped version of the error message. Accepts Pango Markup language.
*/
void MainWindow::errorAndExit(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, true, Gtk::MESSAGE_ERROR);
	//Comme cette classe hérite de Gtk::Window, il est possible d'appeler des méthodes de cette dernière classe.
	dialogue.set_title(title);
	//Ajouter un texte secondaire.
	dialogue.set_secondary_text(body, true);
	dialogue.run(); //Lancer la boîte de dialogue.
	Gtk::Main::quit();
}

/**
	Pops up a warning message
	
	\param[in] title The warning window title
	\param[in] summary The error summary
	\param[in] body A developped version of the warning message. Accepts Pango Markup language.
*/
void MainWindow::warn(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, true, Gtk::MESSAGE_WARNING);
	//Comme cette classe hérite de Gtk::Window, il est possible d'appeler des méthodes de cette dernière classe.
	dialogue.set_title(title);
	//Ajouter un texte secondaire.
	dialogue.set_secondary_text(body, true);
	dialogue.run(); //Lancer la boîte de dialogue.
}

/**
	Pops up an info message
	
	\param[in] title The info window title
	\param[in] summary The error summary
	\param[in] body A developped version of the warning message. Accepts Pango Markup language.
*/
void MainWindow::info(std::string title, std::string summary, std::string body) {
	Gtk::MessageDialog dialogue(*this, summary, true, Gtk::MESSAGE_INFO);
	//Comme cette classe hérite de Gtk::Window, il est possible d'appeler des méthodes de cette dernière classe.
	dialogue.set_title(title);
	//Ajouter un texte secondaire.
	dialogue.set_secondary_text(body, true);
	dialogue.run(); //Lancer la boîte de dialogue.
}

/**
	Loads all the scripts present in the baefiles directory and places them in the
	scriptSelect combo box so that the user can chose them.
*/
void MainWindow::loadAvailableScripts() {
	DIR* dir = opendir(basefiles.c_str());
	scriptSelect->remove_all();
	if(dir == NULL)
	{
		errorAndExit("Unable to find scriptinfo directory",
		"The directory could not be opened",
		"<b>Error !</b> I'm unable to open <i>" + basefiles + "</i> directory :(\n\nPlease check it's existence and the permissions !");
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string file = fic->d_name;
				// Check if is a file
				DIR* t = opendir(fic->d_name);
				if(!t) { // Then it is a file
					//cout << "!" << file << endl;
					if(file.length() > 5 && file.substr(file.length()-4) == ".xml") { // Ok, XML file
						scriptSelect->append(file);
						scriptSelect->set_active_text(file);
					}
				}
				closedir(t);
			}
		}
	}
	
	closedir(dir);
}

/**
	Opens a ScriptExecWindow to run the selected script.
*/
void MainWindow::runScript() {
	std::vector<Parameter> p = params->get();//currentScript->getParameter();
	// If we had an error
	/*reset_error_handler();
	int status;
	eval_string("clear all", false, status);
	eval_string("restoredefaultpath()", true, status);
	chdir(string(basescript + "/" + currentScript->getDirectory()).c_str());
	eval_string("cd " + basescript + "/" + currentScript->getDirectory(), true, status);*/
	
	/*isRunning = true;
	runButton->set_sensitive(false);
	stopButton->set_sensitive(true);
	
	cout << "  Exporting the variables to the script" << endl;
	cout << "-----------------------------------------" << endl;
	cout << "     Name           |        Value       " << endl;
	cout << "-----------------------------------------" << endl;
	
	for(unsigned int i = 0; i < p.size(); i++) { // Export variables !
		//set_global_value(p.at(i).name, p.at(i).value);
		cout << setw(19) << right << p.at(i).name << right << " |" << setw(19) << p.at(i).value << endl;
	}
	cout << "-----------------------------------------" << endl << endl;*/
	/*
	cout << "-----------------------------------------" << endl;
	cout << "  Running " << currentScript->getName() << endl;
	cout << "#########################################" << endl;
	feval(currentScript->getName());
	eval_string("cd " + basescript, true, status);
	chdir(currentwd.c_str());
	cout << "#########################################" << endl;
	cout << "  Done." << endl;*/
	
	// Methode 2 : popen
	
	/*string fname = tmpnam(NULL);
	ofstream of(fname.c_str(), ios::out);
	if(of) {
		cout << "> Created temporary M file : " << fname << endl;
		of << "clear all;" << endl;
		of << "restoredefaultpath();" << endl;
		of << "cd " + basescript + "/" + currentScript->getDirectory() << endl;
		for(unsigned int i = 0; i < p.size(); i++) { // Export variables !
			of << p.at(i).name << " = " << p.at(i).value << ";" << endl;
		}
		of << currentScript->getName() << endl;
		of << "pause" << endl;
		of.close();
		cout << "> Running the script" << endl;
		scriptPid = popen2(string("octave -q " + fname).c_str(), &inf, &outf);
		cout << "> Octave PID : "<<scriptPid<<endl;
		spinner->set_tooltip_markup("Executing " + basescript + "/" + currentScript->getDirectory() + "/" + currentScript->getName());
	} else {
		cout << "Failed to create tmp file" << endl;
	}*/
  
	currentScript->setGeometry(geometryEntry->get_text());
	currentScript->setGeometryDirectory(geometryDirEntry->get_text());
	if(!onRemote) {
		ScriptExecWindow *w = new ScriptExecWindow(basescript, *currentScript, p);
		w->show();
		this->signal_hide().connect(sigc::mem_fun(*w, &ScriptExecWindow::stopScript));
	} else {
		ScriptRemoteExecWindow *w = new ScriptRemoteExecWindow(basescript, *currentScript, p, SSHSession);
		w->show();
		this->signal_hide().connect(sigc::mem_fun(*w, &ScriptRemoteExecWindow::stopScript));
	}
}

/**
	Displays information about the selected script. Its name, author and description.
	The funtction also loads what parameters it takes and add the into the ParameterSet
	of the window.
*/
void MainWindow::dispScript() {
	
	if(scriptSelect->get_active_text() == "")
		return;	
	
	string script = basefiles + "/" + scriptSelect->get_active_text();
	
	params->clear();
	
	currentScript->reset();
	
	cout << "> Loading scriptinfo " << script << endl;
	
	xmlpp::DomParser parser;
	parser.set_throw_messages(true);
	parser.set_substitute_entities(true);
	try {
		if(onRemote) {
			parser.parse_memory(readRemoteFileToString(script));
		} else
			parser.parse_file(script);
	} catch (exception &e) {
		return;
	}
	
	if(parser)
	{
		geometryEntry->set_text("");
		geometryDirEntry->set_text("");
		//Walk the tree:
		xmlpp::Node* rootNode = parser.get_document()->get_root_node();
		xmlpp::Node* elem = rootNode->get_first_child();
		
		while(elem) {
			string name = elem->get_name();
			if(name != "text") { // ignore 'text' parasite nodes
				if(name == "param") { // We load the parameters
					const xmlpp::Element* contentNode = dynamic_cast<const xmlpp::Element*>(elem);
					const xmlpp::Attribute* paramName = contentNode->get_attribute("name");
					const xmlpp::Attribute* paramDefault = contentNode->get_attribute("default");
					const xmlpp::Attribute* paramDesc = contentNode->get_attribute("desc");
					const xmlpp::Attribute* paramMin = contentNode->get_attribute("min");
					const xmlpp::Attribute* paramMax = contentNode->get_attribute("max");
					const xmlpp::Attribute* paramInc = contentNode->get_attribute("inc");
					
					if(paramName && paramDefault && paramDesc && paramMin && paramMax && paramInc) { // We load the parameters
						//cout << paramName->get_value() << " : " << paramDefault->get_value() << " : " << paramDesc->get_value() << endl;
						currentScript->addParameter(paramName->get_value(), paramDesc->get_value(), nstrtod(paramDefault->get_value().c_str(), NULL),
						nstrtod(paramMin->get_value().c_str(), NULL),
						nstrtod(paramMax->get_value().c_str(), NULL),
						nstrtod(paramInc->get_value().c_str(), NULL));
						
					}
				} else { // We load the main information about the script
					const xmlpp::ContentNode* nodeText = dynamic_cast<const xmlpp::ContentNode*>(elem->get_first_child());
					if(nodeText) {
						if(name == "name") { 
							currentScript->setName(nodeText->get_content());
						} else if(name == "author") {
							currentScript->setAuthor(nodeText->get_content());
						} else if(name == "description") {
							currentScript->setDescription(nodeText->get_content());
						} else if(name == "dir") {
							currentScript->setDirectory(nodeText->get_content());
						} else if(name == "geomdir") {
							currentScript->setGeometryDirectory(nodeText->get_content());
							geometryDirEntry->set_text(currentScript->getGeometryDirectory());
						} else if(name == "geom") {
							currentScript->setGeometry(nodeText->get_content());
							geometryEntry->set_text(currentScript->getGeometry());
						} else {
						
						}
					}
				}	
			}
			elem = elem->get_next_sibling();
		}
  } else {
  	cout << "Unable to open file " << script << endl;
  }
  
  // We refresh the info
  
  scriptDesc->set_markup("<b>" + currentScript->getName() + " </b> :\n"
                         + currentScript->getDescription()
                         + "\n\nAuthor : <i>"+currentScript->getAuthor() + "</i>");
  
  vector<Parameter> p = currentScript->getParameter();
  for(unsigned int i = 0; i < p.size(); i++) {
  	params->addInput(p.at(i));
  }
  show_all();
}
