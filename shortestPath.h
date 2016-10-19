#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

void dijkstra(const double* const * adjMatrix, int numVertices, int source, double*& dist, int*& prev);

void dijkstra(const int* const * adj, const double* const * weights, const int* lengths, int numVertices, int source, double*& dist, int*& prev);

int bellmanFord(const double* const * graph, int numVertices, int source, double*& dist, int*& prev);

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev, int numThreads);

#endif
