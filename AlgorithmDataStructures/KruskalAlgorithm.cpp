#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <iterator>
#include <fstream>

using namespace std;

struct Station 
{
    string name;
    int x, y, z, weight, profit;
};

double calculateDistance(const Station& a, const Station& b) 
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

// Function to generate random number from seed digits
int generateRandomNumber(mt19937& rng, const vector<int>& digits, int numDigits)
{
    uniform_int_distribution<> dist(0, digits.size() - 1);
    int result = 0;
    
	for (int i = 0; i < numDigits; ++i) 
	{
        int digit = digits[dist(rng)];
        
		// Ensure the first digit is non-zero when numDigits is more than 1
        if (i == 0 && numDigits > 1) 
		{
            while (digit == 0) 
			{
                digit = digits[dist(rng)];
            }
        }
        
		result = result * 10 + digit;
    }
    return result;
}


// Function to generate stations with random properties
vector<Station> generateStations(unsigned int seed, int numStations, const vector<int>& seedDigits) 
{
    vector<Station> stations;
    mt19937 rng(seed);
    
	for (int i = 0; i < numStations; ++i) 
	{
        Station station;
        
		// Map station index to a letter (A-Z)
        char stationLetter = 'A' + i;
        
		station.name = string(1, stationLetter);
        station.x = generateRandomNumber(rng, seedDigits, 3);
        station.y = generateRandomNumber(rng, seedDigits, 3);
        station.z = generateRandomNumber(rng, seedDigits, 3);
        station.weight = generateRandomNumber(rng, seedDigits, 2);
        station.profit = generateRandomNumber(rng, seedDigits, 2);
        stations.push_back(station);
    }
    
    return stations;
}


// Function to generate a map of connections between stations
vector<pair<int, int>> generateRoutes(const vector<Station>& stations, int numRoutes) 
{
    vector<pair<int, int>> routes;
    set<pair<int, int>> uniqueRoutes;
    mt19937 rng(random_device{}());
    map<int, set<int>> connections;

    // Ensure all stations have at least 3 connections
    for (int i = 0; i < stations.size(); ++i) 
	{
        while (connections[i].size() < 3) 
		{
            int j = rng() % stations.size();
            
			if (i != j && connections[i].find(j) == connections[i].end()) 
			{
                connections[i].insert(j);
                connections[j].insert(i);
                uniqueRoutes.insert(minmax(i, j));
            }
        }
    }

    // Add additional unique routes until we reach 54
    while (uniqueRoutes.size() < numRoutes) 
	{
        int a = rng() % stations.size();
        int b = rng() % stations.size();
        
		if (a != b && uniqueRoutes.find(minmax(a, b)) == uniqueRoutes.end()) 
		{
            uniqueRoutes.insert(minmax(a, b));
            connections[a].insert(b);
            connections[b].insert(a);
        }
    }

    // Transfer unique routes to the vector
    for (const auto& route : uniqueRoutes) 
	{
        routes.emplace_back(route);
    }

    return routes;
}

//Structure for edge between two stations
struct Edge			
{
	int u, v;
	double distance;
	
	Edge(int x, int y, double dist) 
	{
		u = x ;
		v = y ;
		distance = dist ;
	}
};

bool comp(const Edge &a, const Edge &b)
{
	return a.distance < b.distance ;
}

//find function - to determine root of set 
int find(int u, std::vector<int> &parent)
{
	if(u == parent[u])		//check if u is its own parent
		return u;
	
	return parent[u] = find(parent[u], parent) ;	 
}

//union function - perform union of two sets
void unionn(int u, int v, std::vector<int> &parent, vector<int> &rank)
{
	//find root to which u and v belong to
	u = find (u, parent);
	v = find (v, parent);
	
	if(rank[u] < rank[v])
	{
		parent[u] = v ;
	}
	else if(rank[v] < rank[u])
	{
		parent[v] = u ;
	}
	else
	{
		parent[v] = u ;
		rank[u] ++ ;
	}	
}

vector<Edge> KruskalsAlgorithm (const vector<Edge> &edges, int numOfVertices)
{
	vector<Edge> minimumSpanningTree;
	
	//initializes parent array
	vector<int> parent(numOfVertices);
	
	for(int i = 0 ; i < numOfVertices ; i++)
		parent[i] = i;
		
	vector<int> rank(numOfVertices, 0);
	
	//sort edges in non-decreasing order of distance
	vector<Edge> sortedEdges = edges ;
	sort(sortedEdges.begin(), sortedEdges.end(), comp);
	
	for(const Edge &edge : sortedEdges)
	{		
		if(find(edge.u, parent) != find(edge.v, parent))
		{
			minimumSpanningTree.push_back(edge);
			unionn(edge.u, edge.v, parent, rank);
		}
	}
	
	return minimumSpanningTree ;
} 

//Function to generate Graphviz for Minimum Spaning Tree
void graphMST(const vector<Station>& stations, const vector<Edge>& minimumSpanningTree)
{
	ofstream dotFile("MSTGraph.dot");
	
	dotFile << "graph MST { " << endl ;
	
	//Vertices
    for (int i = 0; i < stations.size(); ++i) 
	{
        dotFile << "  " << i << " [label=\"" << 
			stations[i].name << "\"];" << endl;
    }

    //Edges 
    for (const auto& edge : minimumSpanningTree) 
	{
        dotFile << "  " << edge.u << " -- " << 
			edge.v << " [label=\"" << edge.distance << "\"];" << endl;
    }
    
    dotFile << "}" << endl ;
	dotFile.close();
}

int main() 
{
    long long seedSum = 1201100116LL + 1201201773LL + 1201201862LL;
    vector<int> seedDigits;
    long long tempSeedSum = seedSum;
    
	while (tempSeedSum > 0) 
	{
        seedDigits.push_back(tempSeedSum % 10);
        tempSeedSum /= 10;
    }
    
	reverse(seedDigits.begin(), seedDigits.end());

    vector<Station> stations = generateStations(static_cast<unsigned int>(seedSum), 20, seedDigits);
    vector<pair<int, int>> routes = generateRoutes(stations, 54);

    for (const Station& station : stations) 
	{
        cout << station.name << " "
                  << station.x << " "
                  << station.y << " "
                  << station.z << " "
                  << station.weight << " "
                  << station.profit << endl;
    }

	cout << endl ;
	
	vector<Edge> edges;  // Convert routes to edges
    
	for (const auto& route : routes)
    {
        double distance = calculateDistance(stations[route.first], stations[route.second]);
        edges.emplace_back(route.first, route.second, distance);
    }
	
    vector<Edge> minimumSpanningTree = KruskalsAlgorithm(edges, stations.size());
    
    cout << "\nMinimum Spanning Tree Edges : " << endl ;
    
    for(const auto&edge : minimumSpanningTree)
    {
    	cout << "Edge between " << stations[edge.u].name
             << " (" << edge.u << ")"
             << " and " << stations[edge.v].name
             << " (" << edge.v << ")"
             << " Distance: " << edge.distance
             << endl;
	}
	
	cout << endl ;
	
	// Generate Graphviz DOT file
    graphMST(stations, minimumSpanningTree);

    // Inform the user
    cout << "Graphviz DOT file generated: MSTGraph.dot" << endl;
    
    return 0;
}

/*
HOW TO RUN THE GRAPH : USING COMMAND PROMPT
C:\Users\Elyana>E:

E:\>cd "MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3"

E:\MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3>dot -Tpng MSTGraph.dot -o MSTGraph.png

E:\MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3>
*/
