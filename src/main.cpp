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
  \file main.cpp
  \brief Main file of the program
  
  \author Thomas Maurice
  \version 0.3
*/

/**
  \mainpage igafemgui developpement documentation
  
  ![](octave.png)
  
  [TOC]
  
  \section ab About this documentation
  	This documentation is a **developpement** documentation. This should help
  	anyone who want to improve the software, update it, modify it or so. If you
  	are just a standard user (*i.e. not a programmer*) then you just want to use
  	the program. A user manual is available
  	[here](http://svn.svartbergtroll.fr/igafemgui/doc/igafemgui_manual.pdf)
  	for you. Otherwise, this documentation should give you the basic keys to understand
  	the software, how it works, and what it should do.
  
  \section Introduction
  
  igafemgui is a C++ program aimed to make the use of Matlab
  and Octave simulation code more easy. The main purpose is
  the ability to launch the same script with different parameters
  to see how they influence the simulation.
  
  But doing that can break the script if you are not careful enough,
  that's why this program will allow you to change the parameters of
  a script **without actually editing it**.
  
  It also (since v0.3) has some network capabilities, such as running a script
  on a remote server. The drawback is that for security reasons, the user is not
  able to change the config files nor the scripts informations on the server from
  his computer. You will have to upload files manually to do that.
  
  To manage that, it will proceed in 2 operations. It will generate
  a temporary M file containing all the information needed to start
  the main script, then it will be launched by octave.This script contains
  for exemple :
  - The parameters needed
  - If necessary a "geometry file" to define the shape of the solid
  - a 'cd' command to go in the right directory
  - a call to the program
  - a final printf to warn that everything is done
  - a 'pause' call so that octave will close when we kill its process.
  
  To understand how this program works, you shound at least understand the
  following classes :
  - MainWindow
  - ScriptExecWindow
  - EditableParameterSet & EditableParameterInput (the non editable one are basically the same)
  - You can also look at the [file formats (XML)](@ref file_format) used by the program to store config and scripts.
  
  \section checkout Downloading the software
  	In order to use or modify the software, it is obvious that you need to
  	download it. To maintain the code, I use a Subversion repository (because
  	it's more easy to configure than a Git one, that's why !) which can be
  	accessed here : [http://svn.svartbergtroll.fr/igafemgui](http://svn.svartbergtroll.fr/igafemgui)
  	
  	To proceed to a checkout of the code, just type :
  	
  	    svn co http://svn.svartbergtroll.fr/igafemgui
  	    
  	And it should do it. To perform this action you must download the SVN client
  	which is available in every distribution, so depending of which Linux distro
  	you're using, adapt one of the following :
  	
  	    sudo apt-get install svn # For Debian/Ubuntu/Mint/...
  	    su -c 'yum install svn' # For Fedora/RedHat/CentOS/...
  	    ...
  	
  	Then get into the newly created directory and go on to the next section to
  	compile the software.
  
  \section sec Compiling
  	\subsection sub1 A word on the portability
  		This software is not portable. You will see it in the popen2.c and
  		is_process_alive.c files. To start several Octave process, I use some
  		UNIX system calls that are not portable to Windows plateforms. So if you
  		want the software to be ported to Windows, you will have to reimplement
  		those functions on your own. Sorry.
  		
  	\subsection sub2 Let's compile !
  		To compile the code it is pretty easy. However you need some additional
  		stuff :
  		- CMake to generate the Makefile
  		- gtkmm3.0 developpement packages (3.0 at least)
  		- libxml++ developpement packages (2.6 at least, higher is better)
  		- libssh developpement packages (0.5.4 is ok)
  		
  		To compile, the good way to do it is to make a special directory in your
  		toplevel directory.
  		
  		```mkdir build && cd build```
  		
  		Then run cmake, make and make install to see it installed :)
  		
  		```cmake .. && make && make install```
  		
  		And it's done, easy isn't it ?
  	\subsection doc Compiling the documentation.
  		The documentation you are currently reading is compiled using [Doxygen](http://www.doxygen.org).
  		To regenerate it just type ```doxygen``` in a terminal from the root directory and everything will
  		be generated promptly in the doxygen/html directory.
  \section lic License
  
        Copyright (C) 2013 Thomas MAURICE <tmaurice59@gmail.com>
        
        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
        
        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
        GNU General Public License for more details.
        
        You should have received a copy of the GNU General Public License along
        with this program; if not, write to the Free Software Foundation, Inc.,
        51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
   
   \section screenshot Screenshots
   
   ![The main window](mainwindow.png)
   ![Configuration window](confwindow.png)
   ![Metadata window](editmetadata.png)
   ![Edition of a parameter](editparameter.png)
   ![A script being launched](launchscript.png)
   
   \section auth Authors
   This program has been written by Thomas Maurice (<tmaurice59@gmail.com>) for
   Cardiff University's Institute of Mechanics and Advanced Materials.
*/

/**
	\page file_format File format used by the program
	
	[TOC]
	
	All the external files are written, in XML, so that there is no problem
	of portability or other issue that can be induced by the use of binary
	or non standardized files.
	
	\section conf Configuration file
	This is a template of configuration file :
	~~~~~~~~~~~~~~~~~{.xml}
	<?xml version="1.0" encoding="UTF-8"?>
	<config>
		<basefem>/home/thomas/IsogeometricAnalysis</basefem>
		<basedat>/home/thomas/.igafemgui/files</basedat>
	</config>
	~~~~~~~~~~~~~~~~~
	
	There are two tags, "basefem" which represents the base directory in which the
	Octave code is stored, and "basedat" which represent the base directory in which
	the metadata scriptinfo files are stored.
	
	Easy isn't it ? ;)
	
	\section si Script information file
	Same as the config file, here is an example :
	~~~~~~~~~~~~~~~~~{.xml}
	<?xml version="1.0" encoding="UTF-8"?>
	<octscript>
		<name>sampleGraph</name>
		<dir>sample</dir>
		<geomdir>foodir</geomdir>
		<geom>foofile</geom>
		<author>Thomas Maurice</author>
		<description>Sample script that plots a 2D graph of a polynomial of degree n. Here n is between 0 and 7.</description>
		<param name="n" default="1" desc="degree of the polynome" min="0" max="7" inc="1" />
	</octscript>
	~~~~~~~~~~~~~~~~~
	
	The attributes are pretty much self explanatory...
	- name : The script name, this is the one you type in octave to invoke the script
	- dir : subdirectory in which the script is located
	- author : Author name
	- description : Script description what does it do ?
	- geomdir : Directory in which the geometry file can be found
	- geom : Geometry file itself
	
	For the "param" tag, the attributes have the following meanings :
	- name : the parameter name
	- default : the default value
	- desc : the desctription of the parameter
	- min : minimal value
	- max : maximal value
	- inc : increment of the value.
*/
#include <mainWindow.h>

#include <cstdlib>
#include <cstdio>
#include <libxml++/libxml++.h>

#include <iostream>

using namespace std;

/**
	Main function, this will launch the graphical UI
*/
int main(int argc, char* argv[]) {
	cout << "    igafemgui  Copyright (C) 2013 Thomas Maurice <tmaurice59@gmail.com>\n\
    This program comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software, and you are welcome to redistribute it\n\
    under certain conditions; see the GNU/GPL license for details.\n\
" << endl;

  Gtk::Main app(argc, argv);
  MainWindow w;
  Gtk::Main::run(w);

  return EXIT_SUCCESS;
}
