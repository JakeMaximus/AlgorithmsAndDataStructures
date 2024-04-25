#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
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

// Function to build the graph representation
vector<vector<pair<int, int>>> buildGraph(const vector<Station>& stations, const vector<pair<int, int>>& routes) 
{
    vector<vector<pair<int, int>>> graph(stations.size());

    for (const auto& route : routes) 
	{
        int a = route.first;
        int b = route.second;
        int distance = calculateDistance(stations[a], stations[b]);
        
		graph[a].push_back({b, distance});
        graph[b].push_back({a, distance});
    }

    return graph;
}

//Dijkstra's Algorithm
void DijkstraAlgorithm(const vector<vector<pair<int, int>>>& graph, int start, vector<int>& distance, vector<int>& previous)
{
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;		//declares priority queue 'pq' of pairs
	
	distance[start] = 0;	//initializes distance starting node as 0
	pq.push({0, start});	//initializes priority queue start at distance of 0
	
	while(!pq.empty())
	{
		int current = pq.top().second ;
		int currentDistance = pq.top().first ;
		pq.pop();
		
		if(currentDistance > distance[current])
		{
			continue ;
		}
		
		for(const auto& neighbour : graph[current])		//iterates through neighbors of current node in graph
		{
			int next = neighbour.first;
			int weight = neighbour.second;
			
			if(distance[current] + weight < distance[next])	
			{
				distance[next] = distance[current] + weight;	//if new distance is shorter, it will update the distance of the neighbour
				previous[next] = current;
				pq.push({distance[next], next});				//add neighbour to priority queue with updated distance
			}
		}
	}
}

//Print shortest path
void shortestPath(const vector<int>& distance, const vector<int>& previous, int start)
{
	for(int i=0; i<distance.size(); ++i)	//iterates through all stations in graph
	{
		cout << "Shortest path from Station " << start << " to Station " << i << " is " << distance[i] << endl ;
		cout << "Shortest path : " ;
		
		int current = i ;
		
		while(current != -1)	//loop continue as long as current node is not -1
		{
			cout << current << " " ;
			current = previous[current];	//update current note index to be its previous
		}
		
		cout << endl << endl ;
	}
}

//Function to generate Grapgviz for shortest path Dijkstra's Algorithm
void graphShortestPath(const vector<Station>& stations, const vector<pair<int, int>>& routes, const vector<int>& previous)
{
	ofstream dotFile("shortestPathGraph.dot");
	
	dotFile << "graph G { " << endl ;
	
	//Nodes
	for(const Station& station : stations)
	{
		dotFile << "    " << station.name << " [label=\"" << 
					station.name << "\", shape=circle];" << endl;
	}
	
	//Edges
	for(const auto& route : routes)
	{
		dotFile << "    " << stations[route.first].name << " -- " 
					<< stations[route.second].name;
		dotFile << " [label=\"" << calculateDistance(stations[route.first], stations[route.second]) << "\"];" << endl;
	}
	
	//Shortest path
	for(size_t i = 0 ; i < stations.size() ; ++i)
	{
	    int current = i ;
	
	    while(current != -1 && previous[current] != -1)
	    {
	        dotFile << "    " << stations[current].name << " -- " << stations[previous[current]].name << " [color=red];" << endl;
	        current = previous[current] ;
	    }
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

    for (const auto& route : routes) 
	{
        cout << "Route between " << stations[route.first].name
                  << " (" << route.first << ")"
                  << " and " << stations[route.second].name
                  << " (" << route.second << ")"
                  << " Distance: " << calculateDistance(stations[route.first], stations[route.second])
                  << endl;
    }
    
    vector<vector<pair<int, int>>> graph = buildGraph(stations, routes);
	
	int startStation = 0 ;
	
	vector<int> distance(stations.size(), numeric_limits<int> :: max());
	vector<int> previous(stations.size(), -1);
	
	DijkstraAlgorithm(graph, startStation, distance, previous);
	
	cout << endl ;
	
	//print Shortest Path
	shortestPath(distance, previous, startStation);
	
	// Generate Graphviz DOT file
    graphShortestPath(stations, routes, previous);

    // Inform the user
    cout << "Graphviz DOT file generated: shortestPathGraph.dot" << endl;

    return 0;
}


/*
HOW TO RUN THE GRAPH : USING COMMAND PROMPT
C:\Users\Elyana>E:

E:\>cd "MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3"

E:\MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3>dot -Tpng shortestPathGraph.dot -o shortestPathGraph.png

E:\MMU Degree\Sem4\Algorithm Design Analysis\Assignment\Q3>
*/
