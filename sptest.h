#ifndef SPTEST_H
#define SPTEST_H

int readGraph(ifstream& fin, double**& matrix, string*& labels);

int readGraph(ifstream& fin, int**& adj, double**& weights, int*& lengths, string*& labels);

#endif
