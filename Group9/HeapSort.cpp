#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "matplotlibcpp.h"

#ifndef MATPLOTLIBCPP_H
#define MATPLOTLIBCPP_H

namespace plt = matplotlibcpp;

// Include necessary headers for C++ random library
#include <random>

// Function to generate a random dataset using the group leader's ID digits
std::vector<int> generateDataset(long long seed, int datasetSize, const std::vector<int>& idDigits) {
    std::mt19937 generator(seed);  // Mersenne Twister 19937 generator
    std::vector<int> dataset(datasetSize);

    // Generate numbers by combining digits from the ID
    for (int i = 0; i < datasetSize; ++i) {
        int number = 0;
        for (int j = 0; j < 3; ++j) {  // Generate a 3-digit number
            std::uniform_int_distribution<> dist(0, idDigits.size() - 1);
            number = number * 10 + idDigits[dist(generator)];
        }
        dataset[i] = number;
    }

    return dataset;
}

// Helper function to get the digits from the group leader's ID
std::vector<int> getDigitsFromId(long long id) {
    std::vector<int> digits;
    while (id > 0) {
        digits.insert(digits.begin(), id % 10);  // Insert at the beginning to keep the order
        id /= 10;
    }
    return digits;
}

// Helper function to perform heapify
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Function to perform Heap Sort and measure time
void heapSort(std::vector<int>& arr, std::vector<long long>& timings) {
    auto start_time = std::chrono::high_resolution_clock::now();

    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements one by one from the heap
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    timings.push_back(duration.count());
}

// Function to check if the array is sorted
bool isSorted(const std::vector<int>& arr) {
    return std::is_sorted(arr.begin(), arr.end());
}

// Function to plot the graph
void plotGraph(const std::vector<int>& sizes, const std::vector<long long>& timings, const std::string& title) {
    plt::plot(sizes, timings);
    plt::title(title);
    plt::xlabel("Dataset Size");
    plt::ylabel("Time (microseconds)");
    plt::show();
}

#endif // MATPLOTLIBCPP_H

int main() {
    // Specify dataset sizes
    std::vector<int> setSizes = {100, 1000, 10000, 100000, 500000};

    // Create vectors to store timings
    std::vector<long long> heapSortTimings;

    // Generate datasets and perform Heap Sort
    for (int size : setSizes) {
        long long groupLeaderId = 1221303085;
        auto idDigits = getDigitsFromId(groupLeaderId);
        long long seed = groupLeaderId * 5;  // Use seed for Set 5
        std::vector<int> dataset = generateDataset(seed, size, idDigits);

        std::vector<int> heapSortData = dataset;
        heapSort(heapSortData, heapSortTimings);

        // Check if the array is sorted (for debugging)
        if (!isSorted(heapSortData)) {
            std::cerr << "Error: The array is not sorted.\n";
            return 1;
        }
    }

    // Plot the graph
    plotGraph(setSizes, heapSortTimings, "Heap Sort Timings vs Dataset Size");

    return 0;
}