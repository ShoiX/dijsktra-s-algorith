#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#define LIMIT 500
using namespace std;

class vertex
{
public:
	int label = 0;
	vector <int> outward;	// lists of adjacent vertices
	vector <int> weights; // lists of weights of corresponding adjacent vertex

	vertex()
	{
		this->outward.reserve(LIMIT / 2);
		this->weights.reserve(LIMIT / 2);
	}

};

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
		cout<<"Usage: ./dijkstra file [1/2/3/4]\n";
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

	std::vector<vertex> vertices;

	/* LOAD SUBROUTINE */
	string line;
	int n = 0;	//  number of vertices created
	while (getline(f, line))
	{
		// convert the obtained line to a string
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
		}

	}

	if (argc == 3)
	{
		switch(atoi(argv[2]))
		{
			case 1: gen_dump(vertices);
		}
	}	
}