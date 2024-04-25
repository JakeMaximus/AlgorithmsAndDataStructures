#include <iostream>
#include <vector>
#include<algorithm> //include the header for reverse

using namespace std;

struct Station {
    string name;
    int weight;
    int profit;
};

vector<Station> loadDataset() {
    // Define the dataset as an array of Station structs.
    vector<Station> dataset = {
        {"Station A", 250, 100},
        {"Station B", 125, 80},
        {"Station C", 315, 120},
        {"Station D", 670, 230},
        {"Station E", 420, 140},
        {"Station F", 220, 90},
        {"Station G", 315, 110},
        {"Station H", 670, 220},
        {"Station I", 420, 130},
        {"Station J", 220, 85}
        // Add more stations as needed
    };

    return dataset;
}

vector<Station> solveKnapsack(const vector<Station>& stations, int maxCapacity) {
    int numStations = stations.size();
    // Create a 2D table to store the maximum profit for each station and capacity combination.
    vector<vector<int>> dp(numStations + 1, vector<int>(maxCapacity + 1, 0));

    // Fill the dp table using dynamic programming.
    for (int i = 1; i <= numStations; ++i) {
        for (int w = 1; w <= maxCapacity; ++w) {
            if (stations[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - stations[i - 1].weight] + stations[i - 1].profit);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Trace back to find the selected stations.
    int i = numStations;
    int w = maxCapacity;
    vector<Station> selectedStations;

    while (i > 0 && w > 0) {
        if (dp[i][w] != dp[i - 1][w]) {
            selectedStations.push_back(stations[i - 1]);
            w -= stations[i - 1].weight;
        }
        --i;
    }

    reverse(selectedStations.begin(), selectedStations.end());
    return selectedStations;
}

int main() {
    vector<Station> stations = loadDataset();
    int maxCapacity = 800;

    vector<Station> selectedStations = solveKnapsack(stations, maxCapacity);

    // Output the selected stations and their properties.
    cout << "Selected Stations:" << endl;
    for (const Station& station : selectedStations) {
        cout << "Station Name: " << station.name << endl;
        cout << "Weight: " << station.weight << " Profit: " << station.profit << endl;
    }

    return 0;
}
