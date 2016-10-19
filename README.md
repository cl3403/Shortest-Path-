# Shortest-Path-
This program finds the shortest path using Dijkstra’s algorithm for adjacency matrix, Dijkstra’s algorithm for adjacency list, Bellman-Ford algorithm and parallel Dijkstra’s algorithm.

The command 'make all' will compile the entire project. You may also compile and run the project according to the directions below.

---sptest---

To compile: make sptest
To run: ./sptest <a graph file name> <source vertex name> <destination vertex name> <number of threads> <number of threads> 
Description:
The sptest program reads the graph from <a graph file name>, prints out the graph represented by both adjacency matrix and adjacency list, and finds the shortest path from <source vertex name> to <destination vertex name> and prints out the path, distance and runtime using Dijkstra’s algorithm for adjacency matrix, Dijkstra’s algorithm for adjacency list, Bellman-Ford algorithm and parallel Dijkstra’s algorithm. The parallel Dijkstra’s will be called with 1 thread and again with the specified <number of threads> , and prints CPU time and Wall clock time for each call instead of runtime. If a negative cycle is detected, the Bellman-Ford algorithm will instead print out one cycle with the sum of weights of the edges in the cycle.
