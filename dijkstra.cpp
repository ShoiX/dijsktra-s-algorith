#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#define LIMIT 500
#define INFINITY 99999999
using namespace std;

class vertex
{
public:
	int label = 0;
	int index;	//index in the list of vertices
	vector <int> outward;	// lists of adjacent vertices
	vector <int> weights; // lists of weights of corresponding adjacent vertex

	vertex()
	{
		this->outward.reserve(LIMIT / 4);
		this->weights.reserve(LIMIT / 4);
	}

};

// nodes that will be used in the min-heap
typedef struct node
{
	int score = INFINITY;		// dijkstra greedy score
	vertex* ref = NULL;	// vertex the node is referencing	
}node;

void gen_dump(vector<vertex> &vertices)
{
	for (int i = 0, j = vertices.size(); i < j; i++)
	{
		cout<<"Vertex "<<vertices[i].label<<endl;
		for (int k = 0, l = vertices[i].outward.size(); k < l; k++)
		{

			cout<<vertices[vertices[i].outward[k]].label<<"-"<<vertices[i].weights[k]<<" ";
		}
		cout<<endl;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout<<"Usage: ./dijkstra file source [1/2/3/4]\n";
		return 1;
	}
	
	// open input file
	fstream f;
	f.open(argv[1]);
	if (!f.is_open())
	{
		cout<<"Unable to open file\n";
		return 2;
	}
	int source = atoi(argv[2]);
	std::vector<vertex> vertices;

	/* LOAD SUBROUTINE */
	string line;
	int n = 0;	//  number of vertices created
	while (getline(f, line))
	{
		// convert the obtained line to a stream
		stringstream s(line);
		
		string tmp;

		// get the current vertex
		getline(s, tmp, '\t');
		int current = stoi(tmp);

		// always resize vertex when needed
		if (current > n)
		{
			vertices.resize(current);
			n = current;
		}
		vertices[current - 1].label = current;
		vertices[current - 1].index = current - 1;

		// obtain adjacent vertices and their weight
		while(getline(s, tmp, '\t') && (int)tmp[0] != 13)
		{
			int adj;
			int wt;

			// split the comma separated values
			int delim = tmp.find(',', 0);
			adj = stoi(tmp.substr(0, delim));
			wt = stoi(tmp.substr(delim + 1));
			if (adj > n)	// resize
			{
				vertices.resize(adj);
				n = adj;
			}
			vertices[current - 1].outward.push_back(adj- 1);
			vertices[current - 1].weights.push_back(wt);

			// set the label
			vertices[adj - 1].label = adj;
			vertices[adj - 1].index  = adj - 1;
		}

	}

	/* MAIN DIJKSTRA SUBROUTINE */
	int scores[n] = {0};	// dijkstra score of every vertex
	vertex* last = 	&vertices[source - 1];	// last added vertex to the conquered territory

	// mapping of every vertex and their location in the min-heap (0 if non-existant in)
	int map[n] = {0};
	vector<node> heap;
	heap.reserve(n);
	// min-heap data structure
	for (int i = 1; i < n; i++)
	{
		// iterate over the adjacent vertices of last added and add them to the heap
		for (int j = 0, k = last->outward.size(); j < k; j++)
		{
			vertex* current = &vertices[last->outward[j]];
			// current vertex is non-existant on the heap
			if (!map[current->index])
			{
				heap_insert();
			}
		}
	}
	
	if (argc == 3)
	{
		switch(atoi(argv[3]))
		{
			case 1: gen_dump(vertices);
		}
	}
}