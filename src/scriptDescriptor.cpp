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
	\file scriptDescriptor.cpp
	\author Thomas Maurice
	\version 0.2
	\brief Implementation of the ScriptDescriptor class
*/

#include <scriptDescriptor.h>

/**
	Constructor, this does nothnig but creating an instance of the class
*/
ScriptDescriptor::ScriptDescriptor() {

}

/**
	Sets all the fields to "" and clears the parameter list
*/
void ScriptDescriptor::reset() {
	name = "";
	description = "";
	author = "";
	directory = "";
	parameters.clear();
}

/**
	Setter for the name
	\param[in] n the new name
*/
void ScriptDescriptor::setName(std::string n) {
	name = n;
}

/**
	Getter for the name
	\return the name
*/
std::string ScriptDescriptor::getName() {
	return name;
}
		
/**
	Setter for the directory
	\param[in] d the directory
*/
void ScriptDescriptor::setDirectory(std::string d) {
	directory = d;
}

/**
	Getter for the directory
	\return the directory
*/
std::string ScriptDescriptor::getDirectory() {
	return directory;
}

/**
	Setter for the description
	\param[in] d the new description
*/
void ScriptDescriptor::setDescription(std::string d) {
	description = d;
}

/**
	Getter for the desctiption
	\return the description
*/
std::string ScriptDescriptor::getDescription() {
	return description;
}

/**
	Setter for the author
	\param[in] a the new author
*/
void ScriptDescriptor::setAuthor(std::string a) {
	author = a;
}

/**
	Getter for the author
	\return the author
*/
std::string ScriptDescriptor::getAuthor() {
	return author;
}

/**
	Adds a new Parameter to the parameter list
	
	\param[in] n the name
	\param[in] desc the description
	\param[in] dft the default value
	\param[in] min the minimum value
	\param[in] max the maximum value
	\param[in] inc the increment value
*/
void ScriptDescriptor::addParameter(std::string n, std::string desc, double dft, double min, double max, double inc) {
	Parameter p;
	p.name = n;
	p.description = desc;
	p.value = dft;
	p.inc = inc;
	p.min = min;
	p.max = max;
	
	parameters.push_back(p);
}

/**
	Getter for the paremeters
	\return the parameters
*/
std::vector<Parameter> ScriptDescriptor::getParameter() {
	return parameters;
}

/**
	Setter for the geometry directory
	\param[in] d the new geometry directory
*/
void ScriptDescriptor::setGeometryDirectory(std::string d) {
	geometryDirectory = d;
}

/**
	Getter for the geometry directory
	\return the geometry directory
*/
std::string ScriptDescriptor::getGeometryDirectory() {
	return geometryDirectory;
}

/**
	Setter for the geometry 
	\param[in] g the new geometry 
*/
void ScriptDescriptor::setGeometry(std::string g) {
	geometryFile = g;
}

/**
	Getter for the geometry
	\return the geometry
*/
std::string ScriptDescriptor::getGeometry() {
	return geometryFile;
}
