#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <iterator>

using namespace std;

struct Station {
    string name;
    int x, y, z, weight, profit;
};

double calculateDistance(const Station& a, const Station& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

// Function to generate random number from seed digits
int generateRandomNumber(mt19937& rng, const vector<int>& digits, int numDigits) {
    uniform_int_distribution<> dist(0, digits.size() - 1);
    int result = 0;
    for (int i = 0; i < numDigits; ++i) {
        int digit = digits[dist(rng)];
        // Ensure the first digit is non-zero when numDigits is more than 1
        if (i == 0 && numDigits > 1) {
            while (digit == 0) {
                digit = digits[dist(rng)];
            }
        }
        result = result * 10 + digit;
    }
    return result;
}


// Function to generate stations with random properties
vector<Station> generateStations(unsigned int seed, int numStations, const vector<int>& seedDigits) {
    vector<Station> stations;
    mt19937 rng(seed);
    for (int i = 0; i < numStations; ++i) {
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
vector<pair<int, int>> generateRoutes(const vector<Station>& stations, int numRoutes) {
    vector<pair<int, int>> routes;
    set<pair<int, int>> uniqueRoutes;
    mt19937 rng(random_device{}());
    map<int, set<int>> connections;

    // Ensure all stations have at least 3 connections
    for (int i = 0; i < stations.size(); ++i) {
        while (connections[i].size() < 3) {
            int j = rng() % stations.size();
            if (i != j && connections[i].find(j) == connections[i].end()) {
                connections[i].insert(j);
                connections[j].insert(i);
                uniqueRoutes.insert(minmax(i, j));
            }
        }
    }

    // Add additional unique routes until we reach 54
    while (uniqueRoutes.size() < numRoutes) {
        int a = rng() % stations.size();
        int b = rng() % stations.size();
        if (a != b && uniqueRoutes.find(minmax(a, b)) == uniqueRoutes.end()) {
            uniqueRoutes.insert(minmax(a, b));
            connections[a].insert(b);
            connections[b].insert(a);
        }
    }

    // Transfer unique routes to the vector
    for (const auto& route : uniqueRoutes) {
        routes.emplace_back(route);
    }

    return routes;
}



int main() {
    long long seedSum = 1201100116LL + 1201201773LL + 1201201862LL;
    vector<int> seedDigits;
    long long tempSeedSum = seedSum;
    while (tempSeedSum > 0) {
        seedDigits.push_back(tempSeedSum % 10);
        tempSeedSum /= 10;
    }
    reverse(seedDigits.begin(), seedDigits.end());

    vector<Station> stations = generateStations(static_cast<unsigned int>(seedSum), 20, seedDigits);
    vector<pair<int, int>> routes = generateRoutes(stations, 54);

    for (const Station& station : stations) {
        cout << station.name << " "
                  << station.x << " "
                  << station.y << " "
                  << station.z << " "
                  << station.weight << " "
                  << station.profit << endl;
    }

    for (const auto& route : routes) {
        cout << "Route between " << stations[route.first].name
                  << " (" << route.first << ")"
                  << " and " << stations[route.second].name
                  << " (" << route.second << ")"
                  << " Distance: " << calculateDistance(stations[route.first], stations[route.second])
                  << endl;
    }

    return 0;
}
