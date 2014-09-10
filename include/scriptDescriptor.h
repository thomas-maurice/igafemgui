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
	\file scriptDescriptor.h
	\author Thomas Maurice
	\version 0.2
	\brief Defines the class ScriptDescriptor and the struct Parameter
*/
#ifndef SCRIPT_DESC
#define SCRIPT_DESC

#include <iostream>
#include <vector>

/**
	\struct Parameter
	\brief Represents a parameter
*/
struct Parameter {
	std::string name; //!< The parameter name
	std::string description; //!< The parameter description
	double min; //!< The minimal value
	double max; //!< The maximal value
	double inc; //!< The parameter increment
	double value; //!< The default value
};

/**
	\class ScriptDescriptor
	\brief A class to describe an Octave script
*/
class ScriptDescriptor {
	public:
		ScriptDescriptor(); //!< Simple constructor
		
		void setName(std::string n); //!< Setter for the name
		std::string getName(); //!< Getter for the name
		
		void setDirectory(std::string d); //!< Setter for the directory
		std::string getDirectory(); //!< Getter for the directory
		
		void setGeometryDirectory(std::string d); //!< Setter for the geometry file directory
		std::string getGeometryDirectory(); //!< Getter for the geometry file directory
		
		void setGeometry(std::string g); //!< Setter for the geometry file
		std::string getGeometry(); //!< Getter for the geometry file
		
		void setDescription(std::string d); //!< Setter for the description
		std::string getDescription(); //!< Getter for the description
		
		void setAuthor(std::string a); //!< Setter for the author
		std::string getAuthor(); //!< getter for the author
		
		void addParameter(std::string n, std::string desc, double dft, double min, double max, double inc); //!< Add a parameter
		std::vector<Parameter> getParameter(); //!< Get the parameter list
		
		void reset(); //!< Reset the script
		
	private:
		std::string name; //!< Script name
		std::string description; //!< Script description
		std::string directory; //!< Script directory
		std::string author; //!< Script author
		std::string geometryFile; //!< Script geometry file
		std::string geometryDirectory; //!< Script geometry directory
		
		std::vector<Parameter> parameters; //!< Script parameter list
};

#endif
