/*
 * Octor.h
 *
 *  Created on: Oct 28, 2015
 *      Author: kelicheng
 */

#ifndef OCTOR_H_
#define OCTOR_H_
#define NULL 0
#define LEAF       0
#define INTERIOR   1
#define T_UNDEFINED  2

#include "Param.h"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
using namespace std;

extern Param *param;

class Oct;
typedef float query_model_t(Oct *leaf);
typedef int toexpand_t(Oct *leaf);

class Octor {
};
class Octant {
public:
	/* structural information */
	unsigned char type;     /* LEAF or INTERIOR */
	unsigned char where;    /* Where is this octants */
	int which;           /* which child of the parent */
	int level;           /* Root is at level 0 */
};

//!  Oct class representing nodes in Octree.
class Oct{
public:
	unsigned char type; /*! LEAF or INTERIOR */
	int which; /*! Which child of parent */
	int level; /*! Root is at level 0 */
	float vs; /*! Property */
	string code; /*! Unused attribute; idea: convertion between binary code and real corredinates */
	float x; /*! position in x axis. */
	float y; /*! position in y axis. */
	float z; /*! position in z axis. */
	Oct *parent; /*! Pointer to parent */
	Oct *child[8]; /*! Array of pointer to children */

	//! Function to calculate the size of Oct.
	/*!
	  \return The length in x direction
	 */
	float size();
	//! Function to assign the position(origin) of Oct.
	void origin();
	//! Function to calculate position of center of Oct.
	/*!
	  \return A vector including real corredinates in x, y, and z direction.
	 */
	vector<double> center();
};


//!  Octree class representing the mesh.
class Octree {
public:
	Oct root; /*! Root of the tree */
	int depth; /*! Depth = total number of levels */
};

//! Eight nodes in each Element
class Node{
public:
	float x; /*! position in x axis. */
	float y; /*! position in y axis. */
	float z; /*! position in z axis. */
};

//! Element is simplified Oct
class Element{
public:
	float x; /*! position in x axis. */
	float y; /*! position in y axis. */
	float z; /*! position in z axis. */
	float size; /*! length of Element in x axis/ */
	int level; /*! which level the element at */
	float vs; /*! ground property */
};


/*! \fn Octree octor_newtree()
    \brief Instantiate the tree with root only.
    \return The Octree
 */
/*! \fn Oct refine_tree(Oct *leaf, toexpand_t *toexpand, query_model_t *query_model)
    \brief Resursively refine the tree until every oct satisfy the vs-rule.
    \param leaf Pointer to current oct (starts with root)
    \param toexpand Pointer to toexpand function in psolve; check whether or not current leaf needs to expand
    \param query_model Pointer to query_model function in psolve; assign property to leaf's children
    \return Root of tree
 */
/*! \fn vector<Element> traverse_tree(Oct *leaf, vector<Element> elements)
    \brief Recursively traverse the tree to extract LEAF only.
    \param leaf Pointer to current oct (starts with root)
    \param elements Vector of Elements
    \return Vector of Elements
 */
/*! \fn unordered_map<int , Node> extract_mesh(vector<Element> elements)
    \brief Extract the mesh to get Nodes without duplication.
    \param elements Vector of Element (LEAF)
    \return Hash table of Nodes
 */
/*! \fn vector<Node> elem_to_nodes(Element elem)
    \brief Generate eight Nodes for each Element
    \param elem A leaf
    \return Vector of 8 Nodes
 */
/*! \fn int pairing(float x, float y, float z)
    \brief Generate key for each Node in the hash table based on its position
    \param x Node's position in x axis
    \param y Node's position in y axis
    \param z Node's position in z axis
    \return Key to the Node
 */
extern Octree octor_newtree();
extern Oct refine_tree(Oct *leaf, toexpand_t *toexpand, query_model_t *query_model);
extern vector<Element> traverse_tree(Oct *leaf, vector<Element> elements);
extern unordered_map<int , Node> extract_mesh(vector<Element> elements);
static vector<Node> elem_to_nodes(Element elem);
static int pairing(float x, float y, float z);
static uint32_t math_hashuint32(const void *start, int32_t count);


#endif /* OCTOR_H_ */
