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
	int dist = INFINITY;	// min-distance to a specific vertex 
	bool conquered = false;
	int label = 0;
	int index;	//index in the list of vertices
	vector <int> outward;	// lists of index of adjacent vertices
	vector <int> weights; // lists of weights of corresponding adjacent vertex
	vertex()
	{
		this->outward.reserve(LIMIT / 4);
		this->weights.reserve(LIMIT / 4);
	}

};
typedef struct node
{
	int score;
	vertex* ref;
}node;

// the heap data structure
class heap
{
public:
	vector<node> nodes;
	heap(int n)
	{
		nodes.reserve(n - 1);
	}

	int insert(vertex* v, int greedy_score, int map[])
	{
		// insert vertex and the weight
		node tmp;
		tmp.ref = v;
		tmp.score = greedy_score;
		this->nodes.push_back(tmp);
		map[v->index] = this->nodes.size();

		int pos = this->nodes.size() - 1;
		// bubble up
		while(pos != 0)
		{
			int parent = (pos - 1) / 2;
			if (this->nodes[pos].score < this->nodes[parent].score)
			{
				// swap
				swap(pos, parent, map);
				pos = parent;
			}
			else
				break;
		}
		return pos + 1;
	}
	int replace(vertex* v, int greedy_score, int pos, int map[])
	{
		if (pos > this->nodes.size() - 1)
			return 0;
		this->nodes[pos].ref = v;
		this->nodes[pos].score = greedy_score;
		
		while(pos != 0)
		{
			int parent = (pos - 1) / 2;
			if (this->nodes[pos].score < this->nodes[parent].score)
			{
				swap(pos, parent, map);
				pos = parent;
			}
			else
				break;
		}
		return pos + 1;
	}
	// extracts the root node and returns the address of the last
	vertex* extract(int map[])
	{
		if (this->nodes.size() < 1)
			return NULL;
		// swap the root and the tail
		node head = this->nodes[0];
		swap(0, this->nodes.size() - 1, map);
		this->nodes.pop_back();

		int pos = 0;
		int nsize = this->nodes.size();

		// bubble down
		while (pos < nsize - 1)
		{
			int  left 	= (pos * 2) + 1;
			int  right = (pos * 2) + 2;
			if (left > nsize - 1)
				break;
			else if (this->nodes[left].score < this->nodes[pos].score)
			{
				swap(left, pos, map);
				pos = left;
				continue;
			}
			if (right > nsize - 1)
				break;
			else if (this->nodes[right].score < this->nodes[pos].score)
			{
				swap(right, pos, map);
				pos = right;
				continue;
			}
			break;
		}

		// set the dijkstra score of min
		head.ref->dist = head.score;
		return head.ref;
	}
private:
	void swap(int a, int b, int map[])
	{
		// swap entries on the mapping
		map[this->nodes[a].ref->index] = b + 1;
		map[this->nodes[b].ref->index] = a + 1;
		node tmp = this->nodes[a];
		this->nodes[a] = this->nodes[b];
		this->nodes[b] = tmp;


		return;
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

void dist_dump(vector<vertex> &vertices)
{
	for (int i = 0, j = vertices.size(); i < j; i++)
	{
		cout<<"Vertex "<<vertices[i].label<<": "<<vertices[i].dist<<endl;
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
	if (source < 1 || source > n)
	{
		cout<<"Invalid source vertex\n";
		return 3;
	}
	/* MAIN DIJKSTRA SUBROUTINE */
	vertex* last = 	&vertices[source - 1];	// last added vertex to the conquered territory
	last->conquered = true;
	last->dist = 0;

	// mapping of every vertex and their location in the min-heap (0 if non-existant in heap, not zero indexed)
	int map[n] = {0};
	heap minheap(n);
	// min-heap data structure
	while (1)
	{
		// iterate over the adjacent vertices of last added and add them to the heap
		for (int j = 0, k = last->outward.size(); j < k; j++)
		{
			if (vertices[last->outward[j]].conquered)
				continue;
			// not existing in heap
			if (!map[last->outward[j]])
			{
				minheap.insert(&vertices[last->outward[j]], last->dist + last->weights[j], map);
			}
			// else compare score in heap vs current dijkstra score
			else
			{
				// existing score
				int existing = minheap.nodes[map[last->outward[j]] - 1].score;
				if (last->dist + last->weights[j] < existing)
				{
					minheap.replace(&vertices[last->outward[j]], last->dist + last->weights[j], map[last->outward[j]] - 1, map);
				}
			}
		}
		// extraction of the min-heap head and set the dijkstra score
		last = minheap.extract(map);
		if (last == NULL)
			break;
		last->conquered = true;
	}
	
	if (argc > 3)
	{
		switch(atoi(argv[3]))
		{
			case 1: gen_dump(vertices);
			case 2: dist_dump(vertices);
		}
	}
}