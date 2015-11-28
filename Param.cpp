/*
 * Param.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: kelicheng
 */

#include "Param.h"

Param::Param(string path){
	FILE* fp;
	int samples;
	char file_path[50];
	strcpy(file_path, path.c_str());

	fp = fopen (file_path, "r");

	//	 if (parsetext(fp, "region_length_east_m", 'd', "Illegal number of region_length_east_m", "lt", 0, 111, &domain_in_y) != 0) ||
	//	    parsetext(fp, "region_length_north_m", 'd', "Illegal number of region_length_north_m", "gt", 0, NULL, &domain_in_x) != 0) ||
	//		parsetext(fp, "region_depth_deep_m", 'd', "Illegal number of region_depth_deep_m", "gt", 0, NULL, &domain_in_z) ||
	//		parsetext(fp, "simulation_node_per_wavelength", 'i', "Illegal number of samples", "gt", 0, NULL, &samples) != 0) ||
	//		parsetext(fp, "simulation_wave_max_freq_hz", 'd', "Illegal frequency value", "gt", 0, NULL, &freq) != 0) )
	//	    {
	//	        fprintf(stderr, "Error reading parameters. \n");
	//	        return -1;
	//	    }
	parsetext(fp, "region_length_east_m", 'd', "Illegal number of region_length_east_m", "gt", 0, NULL, &domain_in_y);
	parsetext(fp, "region_length_north_m", 'd', "Illegal number of region_length_north_m", "gt", 0, NULL, &domain_in_x);
	parsetext(fp, "region_depth_deep_m", 'd', "Illegal number of region_depth_deep_m", "gt", 0, NULL, &domain_in_z);

	parsetext(fp, "simulation_node_per_wavelength", 'i', "Illegal number of samples", "gt", 0, NULL, &samples);
	parsetext(fp, "simulation_wave_max_freq_hz", 'd', "Illegal frequency value", "gt", 0, NULL, &freq);


	//	cout << domain_in_y << "\n";
	//	cout << domain_in_x << "\n";
	//	cout << domain_in_z << "\n";
	//	cout << samples << "\n";
	//	cout << freq << "\n";

	factor = freq * samples;
}

/**
 * Parse a text file and return the value of a match string.
 *
 * \return 0 if OK, -1 on error.
 */
int parsetext (FILE* fp, const char* querystring, const char type, string err_msg,
		string check, double ref_value, double default_value, void* result) {

	const static char delimiters[] = " =\n\t";

	int32_t res = 0, found = 0;

	/* Start from the beginning */
	rewind(fp);


	/* Look for the string until found */
	while (!found) {
		char line[LINESIZE];
		char *name, *value;

		/* Read in one line */
		if (fgets(line, LINESIZE, fp) == NULL)
			break;


		name = strtok(line, delimiters);
		if ((name != NULL) && (strcmp(name, querystring) == 0)) {

			found = 1;
			value = strtok(NULL, delimiters);

			switch (type) {
			case 'i':
				res = sscanf(value, "%d", (int *)result);
				break;
			case 'f':
				res = sscanf(value, "%f", (float *)result);
				break;
			case 'd':
				res = sscanf(value, "%lf", (double *)result);
				break;
			case 's':
				res = 1;
				strcpy((char *)result, value);
				break;
			case 'u':
				res = sscanf(value, "%u", (uint32_t *)result);
				break;
			default:
				fprintf(stderr, "parsetext: unknown type %c\n", type);
				return -1;
			}
		}
	}
	/*Sanity check*/
	int valid = 0;
	if (check != ""){
		if (check == "gt"){
			valid = ((*(double *)result) > ref_value);
		} else if (check == "ge"){
			valid = ((*(double *)result) >= ref_value);
		} else if (check == "lt"){
			valid = ((*(double *)result) < ref_value);
		} else if (check == "le"){
			valid = ((*(double *)result) <= ref_value);
		} else {
			cout << "[ERROR]: unknown condition for sanity check. \n";
			return -1;
		}

		if (!valid){
			if (default_value != NULL){
				(*(double *)result) = default_value;
				return 0;
			} else {
				cout << err_msg << "\n";
				return -1;
			}
		}
		return 0;
	}
	return (res == 1) ? 0 : -1;
}


