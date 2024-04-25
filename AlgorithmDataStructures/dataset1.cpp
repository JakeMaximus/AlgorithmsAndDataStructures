#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

// Function to generate a random dataset using the group leader's ID digits
vector<int> generateDataset(long long seed, int datasetSize, const vector<int>& idDigits) {
    mt19937 generator(seed);  // Mersenne Twister 19937 generator
    vector<int> dataset(datasetSize);

    // Generate numbers by combining digits from the ID
    for (int i = 0; i < datasetSize; ++i) {
        int number = 0;
        for (int j = 0; j < 3; ++j) {  // Generate a 3-digit number
            uniform_int_distribution<> dist(0, idDigits.size() - 1);
            number = number * 10 + idDigits[dist(generator)];
        }
        dataset[i] = number;
    }

    return dataset;
}

// Helper function to get the digits from the group leader's ID
vector<int> getDigitsFromId(long long id) {
    vector<int> digits;
    while (id > 0) {
        digits.insert(digits.begin(), id % 10);  // Insert at the beginning to keep the order
        id /= 10;
    }
    return digits;
}

int main() {
    long long groupLeaderId = 1221303085;
    auto idDigits = getDigitsFromId(groupLeaderId);

    // Specify dataset sizes
    vector<int> setSizes = {100, 1000, 10000, 100000, 500000};

    // Create a map to store datasets
    map<string, vector<int>> datasets;

    // Generate datasets
    for (size_t i = 0; i < setSizes.size(); ++i) {
        long long seed = groupLeaderId * (i + 1);  // Use different seed for each set
        vector<int> dataset = generateDataset(seed, setSizes[i], idDigits);
        datasets["Set " + to_string(i + 1) + " (" + to_string(setSizes[i]) + ")"] = dataset;
    }

    // Print or use the generated datasets as needed
    for (const auto& setPair : datasets) {
        cout << setPair.first << ": ";
        for (int i = 0; i < min(5, static_cast<int>(setPair.second.size())); ++i) {
            cout << setPair.second[i] << " ";
        }
        cout << "...\n";  // Print the first 5 elements as an example
    }

    // Check if the set actually has 500000 elements
    string datasetKey = "Set 5 (500000)";
    if (datasets.find(datasetKey) != datasets.end()) {
        size_t size = datasets[datasetKey].size();
        cout << "Size of " << datasetKey << ": " << size << " elements." << endl;
    } else {
        cerr << "Dataset " << datasetKey << " not found." << endl;
    }

    return 0;
}