/*
 * Param.h
 *
 *  Created on: Nov 4, 2015
 *      Author: kelicheng
 */

#ifndef PARAM_H_
#define PARAM_H_
#define LINESIZE 50
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

class Param {
public:
	double domain_in_x; /*! Domain of mesh in x axis. */
	double domain_in_y; /*! Domain of mesh in y axis. */
	double domain_in_z; /*! Domain of mesh in z axis. */
	double freq; /*! Maximum frequency of simulation wave in Hz */
	double factor; /*! Factor = maximum frequence * samples */
	Param(string path);
};



/*! \fn int parsetext (FILE* fp, const char* querystring, const char type, string err_msg,
		string check, double ref_value, double default_value, void* result);
    \brief Parse the parameters out from given input file.
    \param fp Given input file
    \param querystring Label of parameter in the file
    \param type Type of the parameter
    \param err_msg The error message to print when the parameter is invalid
    \param check "gt" = greater than; "ge" = greater than or equal to;  ect.
    \param ref_value The reference value used in sanity check
    \param default_value The default value of parameter when it's invalid
    \param result Pointer to field of Param
    \return 0 if OK, -1 on error.
 */

int parsetext (FILE* fp, const char* querystring, const char type, string err_msg,
		string check, double ref_value, double default_value, void* result);

#endif /* PARAM_H_ */
