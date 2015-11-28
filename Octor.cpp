/*
 * Octor.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: kelicheng
 */
#include "Octor.h"
#define mix(a,b,c) \
		{ \
	a -= b; a -= c; a ^= (c>>13); \
	b -= c; b -= a; b ^= (a<<8); \
	c -= a; c -= b; c ^= (b>>13); \
	a -= b; a -= c; a ^= (c>>12);  \
	b -= c; b -= a; b ^= (a<<16); \
	c -= a; c -= b; c ^= (b>>5); \
	a -= b; a -= c; a ^= (c>>3);  \
	b -= c; b -= a; b ^= (a<<10); \
	c -= a; c -= b; c ^= (b>>15); \
		}

extern Octree octor_newtree(){
	Octree tree;
	tree.depth = 4; // TODO: update with refine_tree
	tree.root.type = T_UNDEFINED;
	tree.root.parent = NULL;
	tree.root.which = 0;
	tree.root.level = 0;

	tree.root.x = 0;
	tree.root.y = 0;
	tree.root.z = 0;
	tree.root.vs = 100;
	return tree;
}

extern Oct refine_tree(Oct *leaf, toexpand_t *toexpand, query_model_t *query_model){
	if(toexpand(leaf) == 1){
		for (int which = 0; which < 8; which ++){
			leaf->child[which] = new Oct;
			leaf->child[which]->parent = leaf;
			leaf->child[which]->which = which;
			leaf->child[which]->type = T_UNDEFINED;
			leaf->child[which]->level = leaf->level +1;
			leaf->child[which]->origin(); // allocate the origin of child
			leaf->child[which]->vs = query_model(leaf->child[which]);
			refine_tree(leaf->child[which], toexpand, query_model);
		}
		leaf->type = INTERIOR;
	} else {
		leaf->type = LEAF;
	}
	return *leaf;

}

extern vector<Element> traverse_tree(Oct *leaf, vector<Element> elements){
	if (leaf->type == LEAF){
		Element e;
		e.x = leaf->x;
		e.y = leaf->y;
		e.z = leaf->z;
		e.level = leaf->level;
		e.vs = leaf->level;
		e.size = leaf->size();
		elements.push_back(e);
	} else {
		for (int i = 0; i < 8; i++){
			elements = traverse_tree(leaf->child[i], elements);
		}
	}
	return elements;
}

extern unordered_map<int , Node> extract_mesh(vector<Element> elements){
	vector<Node> nodes;
	unordered_map <int , Node> nodes_table;
	int hashentry;

	ofstream mynodes;
	mynodes.open ("/Users/kelicheng/Desktop/mykeys.txt");

	for (int i = 0; i < elements.size(); i++){
		nodes = elem_to_nodes(elements[i]);

		for (int j = 0; j < nodes.size(); j++){
//			TODO: optimize the pairing function for a more accurate hashentry 8^4
			hashentry = pairing(nodes[j].x, nodes[j].y, nodes[j].z);
			mynodes << nodes[j].x << ", " << nodes[j].y << ", " << nodes[j].z << ", " << hashentry << "\n";
			nodes_table[hashentry] = nodes[j];
		}
	}
	cout << elements.size() << "\n";
	 cout << elements.size()*8 << "\n";
	 cout << nodes_table.size() << "\n";
	mynodes.close();
	return nodes_table;

}

/*generate nodes of an element*/
static vector<Node> elem_to_nodes(Element elem){
	vector<Node> nodes;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 2; k++){
				Node n;
				n.x = elem.x + elem.size*i;
				n.y = elem.y + elem.size*j;
				n.z = elem.z + elem.size*k;
				nodes.push_back(n);
			}
		}
	}
	return nodes;
}

static int pairing(float x, float y, float z){
	int n = ((x + y)*(x + y + 1)/2) + y;
	int hashentry = ((n + z)*(n + z + 1)/2) + z;
	return hashentry;
}

/**
 * math_hashuint32: Hash 'count' number of uint32's to an offset value
 * copied from original hercules
 */
static uint32_t math_hashuint32(const void *start, int32_t length){
	const uint32_t *k = (uint32_t *)start;
	register uint32_t a, b, c, len;

	/* Set up the internal state */
	len = length;
	a = b = 0x9e3779b9;    /* the golden ratio; an arbitrary value */
	c = 0;                 /* A random number */
	/* Handle most of the key */
	while (len >= 3) {
		a += k[0];
		b += k[1];
		c += k[2];
		mix(a,b,c);
		k += 3; len -= 3;
	}
	/* Handle the last 2 uint32_t's */
	c += length;
	switch(len) {
	/* all the case statements fall through */
	/* c is reserved for the length */
	case 2 :
		b+=k[1];
		break;
	case 1 :
		a+=k[0];
		break;
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	return c;
}

/* classes of Oct */
float Oct::size(){
	return param->domain_in_x*pow(0.5, level);
}

void Oct::origin(){
	vector<float> position(3);
	float unit_x = param->domain_in_x*pow(0.5, level);
	float unit_y = param->domain_in_y*pow(0.5, level);
	float unit_z = param->domain_in_z*pow(0.5, level);

	// assigning x
	if (which%2 == 0){
		x = parent->x;
	} else {
		x = parent->x + unit_x;
	}

	// assigning y
	if (which < 2 || which == 4 || which == 5){
		y = parent->y;
	} else {
		y = parent->y + unit_y;
	}

	// assigning z
	if (which < 4){
		z = parent->z;
	} else {
		z = parent->z + unit_z;
	}
}

vector<double> Oct::center(){
	vector<double> center(3);
	double unit_x = param->domain_in_x*pow(0.5, level+1);
	double unit_y = param->domain_in_y*pow(0.5, level+1);
	double unit_z = param->domain_in_z*pow(0.5, level+1);

	center[0] = x + unit_x;
	center[1] = y + unit_y;
	center[2] = z + unit_z;

	return center;
}
