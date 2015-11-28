/*
 * Psolve.h
 *
 *  Created on: Nov 27, 2015
 *      Author: kelicheng
 */

#ifndef PSOLVE_H_
#define PSOLVE_H_
#include "Octor.h"
#include "Param.h"
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;

//! Parameter object
/*! global parameter shared between psolve and Octor.*/

/*! \fn void mesh_generate()
    \brief Generate mesh as tree
*/
/*! \fn float query_model(Oct *leaf)
    \brief Assign property to leaf.
    \param leaf Pointer to an Oct
    \return Ground property
*/
/*! \fn int toexpand(Oct *leaf)
    \brief Check whether or not to refine given leaf
    \param leaf Pointer to an Oct
    \return 1 to expand; 0 not to expand
*/
/*! \fn void print_vector(vector<Element> elements, string path)
    \brief Generate text file printing elements' position and property.
    \param elements Vector of Elements extracted from mesh
    \param path Path to text file
*/
/*! \fn void print_nodes(unordered_map<int, Node> nodes_table, string path)
    \brief Generate text file printing Node's position
    \param nodes_table The hash table containing all the Nodes extracted from mesh.
    \param path Path to text file
*/
Param *param;
static void mesh_generate();
static float query_model(Oct *leaf);
static int toexpand(Oct *leaf);
static void print_vector(vector<Element> elements, string path);
static void print_nodes(unordered_map<int, Node> nodes_table, string path);

#endif /* PSOLVE_H_ */
