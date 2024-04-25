#include <iostream>
#include <vector>
#include <chrono>
#include <random> 
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// Function to generate a random dataset using the group leader's ID digits
std::vector<int> generateDataset(long long seed, int datasetSize, const std::vector<int>& idDigits) {
    std::mt19937 generator(seed);
    std::vector<int> dataset(datasetSize);

    for (int i = 0; i < datasetSize; ++i) {
        int number = 0;
        for (int j = 0; j < 3; ++j) {
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
        digits.insert(digits.begin(), id % 10);
        id /= 10;
    }
    return digits;
}

// Helper function to merge two subarrays of arr[]
void merge(std::vector<int>& arr, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<int> leftArray(n1), rightArray(n2);

    // Copy data to temporary arrays leftArray[] and rightArray[]
    for (int i = 0; i < n1; i++)
        leftArray[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArray[j] = arr[middle + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            arr[k] = leftArray[i];
            i++;
        } else {
            arr[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArray[], if there are any
    while (i < n1) {
        arr[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArray[], if there are any
    while (j < n2) {
        arr[k] = rightArray[j];
        j++;
        k++;
    }
}

// Main Merge Sort function
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Same as (left + right) / 2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        // Merge the sorted halves
        merge(arr, left, middle, right);
    }
}

// Function to perform Merge Sort and measure time
void runMergeSort(std::vector<int>& arr, std::vector<long long>& timings) {
    auto start_time = std::chrono::high_resolution_clock::now();

    int n = arr.size();

    // Perform Merge Sort
    mergeSort(arr, 0, n - 1);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    timings.push_back(duration.count());
}

// Function to check if the array is sorted
bool isSorted(const std::vector<int>& arr) {
    return std::is_sorted(arr.begin(), arr.end());
}

// Function to print the first few elements of the array
void printArray(const std::vector<int>& arr, int count = 20) {
    std::cout << "Sorted Array: ";
    for (int i = 0; i < std::min(count, static_cast<int>(arr.size())); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\nArray Size: " << arr.size() << "\n";
}

// Function to plot the graph
void plotGraph(const std::vector<int>& sizes, const std::vector<long long>& timings, const std::string& title) {
    plt::plot(sizes, timings);
    plt::title(title);
    plt::xlabel("Dataset Size");
    plt::ylabel("Time (microseconds)");
    plt::show();
}

int main() {
    // Specify dataset sizes
    std::vector<int> setSizes = {100, 1000, 10000, 100000, 500000};

    // Create vectors to store timings
    std::vector<long long> mergeSortTimings;

    // Generate datasets and perform Merge Sort
    for (int size : setSizes) {
        long long groupLeaderId = 1221303085;
        auto idDigits = getDigitsFromId(groupLeaderId);
        long long seed = groupLeaderId * 5;  // Use seed for Set 5
        std::vector<int> dataset = generateDataset(seed, size, idDigits);

        std::vector<int> mergeSortData = dataset;
        runMergeSort(mergeSortData, mergeSortTimings);

        // Check if the array is sorted
        if (!isSorted(mergeSortData)) {
            std::cerr << "Error: The array is not sorted after Merge Sort.\n";
            return 1;
        }

        // Print the first few elements of the sorted array
        printArray(mergeSortData);
    }

    // Plot the graph
    plotGraph(setSizes, mergeSortTimings, "Merge Sort Timings vs Dataset Size");

    return 0;
}