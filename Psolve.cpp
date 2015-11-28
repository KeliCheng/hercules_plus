/*
 * psolve.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: kelicheng
 */
/**
 * Generates simple mesh in tree structure.
 */
#include "Psolve.h"

static float query_model(Oct *leaf){
	double center_x = leaf->center()[0];
	double center_y = leaf->center()[1];
	double center_z = leaf->center()[2];

	// test of cylinder model
	double origin_x = 100;
	double origin_y = 100;
	double radius = 50;
	double distance_square = pow(fabs(center_x-origin_x), 2) + pow(fabs(center_y-origin_y), 2);

	// cout << leaf->x << ", " << leaf->y << ", " << leaf->z << "\n";
	// cout << leaf->center() << "\n";
	if (distance_square <= pow(radius, 2)){
		return 10;
	} else{
		return 40;
	}
//	if (center_y <= 150){
//			return 50;
//		} else{
//			return 20;
//		}
}

static int toexpand(Oct *leaf){
	// apply vs-rule; return 0 if need to expand; return 1 not to expand
	if (leaf->size() <= leaf->vs/param->factor){
//		 cout << "* Not expand: "<< leaf->x << ", " << leaf->y << ", " << leaf->z << ", " << leaf->size() << ", " << leaf->vs << "\n";
//		mymesh <<leaf->x << ", " << leaf->y << ", " << leaf->z << ", " << leaf->vs << "\n";
		return 0;
	}else{
//		 cout << "* To expand: "<< leaf->x << ", " << leaf->y << ", " << leaf->z << ", " << leaf->size() << ", " << leaf->vs << "\n";
//		mymesh << leaf->x << ", " << leaf->y << ", " << leaf->z << ", " << leaf->vs << "\n";
		return 1;
	}
}

static void print_vector(vector<Element> elements, string path){
	ofstream mymesh;
	mymesh.open (path);
	for(int i = 0; i != elements.size(); i++) {
		mymesh <<elements[i].x << ", " << elements[i].y << ", " << elements[i].z << ", " << elements[i].vs << "\n";
	}
	mymesh.close();
}
static void print_nodes(unordered_map<int, Node> nodes_table, string path){
	vector<Node> nodes;
	ofstream mynodes;
	mynodes.open(path);
	for (auto kv : nodes_table){
		// nodes.push_back(kv.second);
		mynodes << kv.second.x << ", " << kv.second.y << ", " << kv.second.z << ", " << 1 << "\n";
	}
	mynodes.close();
}

static void mesh_generate(){
	// double ppwl = param->factor / param->freq;
	// double myfactor = param->factor;

	Octree tree = octor_newtree();
	tree.root = refine_tree(&tree.root, &toexpand, &query_model);

	vector<Element> elements;
	unordered_map<int , Node> nodes;
	elements = traverse_tree(&tree.root, elements);
	print_vector(elements, "/Users/kelicheng/Desktop/mymesh.txt");
	nodes = extract_mesh(elements);
	print_nodes(nodes, "/Users/kelicheng/Desktop/mynodes.txt");
}





int main(int argc, char* argv[]){
	string infile = "";
	if (argc > 1){
		infile = argv[0];
	}

	infile = "/Users/kelicheng/Desktop/hercules_inputs ";
	param = new Param(infile);


	mesh_generate();


	return 0;
}



