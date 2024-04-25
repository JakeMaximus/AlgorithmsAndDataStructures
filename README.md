# AlgorithmsAndDataStructures
Programming Using Algorithms and Data Structures

The system consists of:
• Algorithm 1 (dataset 1 and 2 generation) implementation and reporting and other questions
• Algorithm 2 (Heap and Merge Sort) implementation and reporting and other questions
• Algorithm 3 (Dijkstra’s and Kruskal’s) implementation and reporting and other questions
• Algorithm 4 (Dynamic Programming) implementation and reporting and other questions

First Part
Dataset 1
Write an algorithm to generate 4 sets of data with random numeric seeding. Use group leader’s ID as the random seed reference to generate dataset of size(s) as follows:
Set 1: 100
Set 2: 1,000
Set 3: 10,000
Set 4: 100,000
Set 5: 500,000
Dataset 2
You are assigned to assist in designing locations of stations with treasures that each station should hold for a treasure hunt project. In order to ensure the feasibility for participants to win the treasure hunt with designed stations’ locations, a theoretical analysis need to be conducted.
Write an algorithm to generate dataset for 20 stations (vertices) with 54 routes (edges) that connect between stations. Each station needs to at least connect to 3 stations. Use the sum of other group members’ ID number to generate data consisting of values for station name, x-coordinate, y-coordinate, z-coordinate, weight and profit. The weight and profit refer to the treasure to be collected from the station.
For example: Station A 265 324 427 18 40
Use the following equation to calculate the distance between stations: 𝑑𝑖𝑠𝑡𝑎𝑛𝑐𝑒𝑖,𝑗= √(𝑥𝑗− 𝑥𝑖)2+(𝑦𝑗− 𝑦𝑖)2+ (𝑧𝑗− 𝑧𝑖)2
Draw the designed stations with routes with details of station name as example shown in Figure 1:

![image](https://github.com/JakeMaximus/AlgorithmsAndDataStructures/assets/84440007/2a582f82-c693-46dc-bab0-4801f2dafb1f)


Second Part
Use Dataset 1 for implementation of Heap Sort and Merge Sort Algorithm.
Heap Sort
Store the data in a priority queue using a heap. Record the time to insert all data into the priority queue. Dequeue the data to test the timing of the priority queue. Plot the graph of timing vs dataset size and discuss about the time and space complexity of the algorithm.
Merge Sort
Sort the dataset(s) using merge sort. Record the time to complete sorting of data. Plot the graph of timing vs dataset size and discuss about the time and space complexity of the algorithm.


Third Part
Use Dataset 2 for the implementation of Dijkstra’s and Kruskal’s Algorithm.
Shortest Paths
Write a program to identify the shortest paths from Station A to the other stations using Dijkstra’s Algorithm. Then display the shortest distance to each station. Draw the graph representing the shortest paths. Discuss about the time and space complexity of the algorithm.
Minimum Spanning Tree
Write a program to identify the Minimum Spanning Tree using Kruskal’s Algorithm. Display the edges of the tree and draw the graph representing the Minimum Spanning Tree. Discuss about the time and space complexity of the algorithm.


Forth Part
Use Dataset 2 for the implementation of 0/1 Knapsack Algorithm.
Dynamic Programming
Assume that each participant travels in a truck which have maximum capacity of 800kg. Write a program to identify the set of station to visit without returning to home station using the 0/1 Knapsack Algorithm (assume that if you reach a station, you have to take all the treasure at that station). Draft out the resulting matrix and the list of stations to visit, with the weights and profits from each station to be winner of the treasure hunt project. Discuss about the time and space complexity of the algorithm.
