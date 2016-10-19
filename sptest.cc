#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include "shortestPath.h"
#include <vector>
#include <cstdlib>
#include <thread>

using namespace std;

int readGraph(ifstream& fin, double**& matrix, string*& labels)
{
  int numV = 0;
  string label;
  double weight;
  fin >> numV;
  matrix = new double*[numV];
  for (int j = 0; j < numV; j++)
  {
    matrix[j] = new double[numV];
  }
  labels = new string[numV];
  
  for (int i = 0; i < numV; i++)
  {
    fin >> label;
    labels[i] = label;
  }
 
  for (int m = 0; m < numV; m++)
  {
    for (int n = 0; n < numV; n++)
    {
      fin >> weight;

      if (weight == 0)
      {
        weight = numeric_limits<double>::infinity();  
      }

      if (m == n)
      {
        weight = 0;
      }
 
      matrix[m][n] = weight;
    }
  }  

  return numV;
}

int readGraph(ifstream& fin, int**& adj, double**& weights, int*& lengths, string*& labels)
{
  int numV = 0;
  string label;
  double weight;
  fin >> numV;
  labels = new string[numV];
  for (int i = 0; i < numV; i++)
  {
    fin >> label;
    labels[i] = label;
  }

  adj = new int*[numV];
  weights = new double*[numV];
  lengths = new int[numV];
  int numEdges;
  double* edgeList = new double[numV];

  for (int m = 0; m < numV; m++)
  {
    numEdges = 0;
    for (int n = 0; n < numV; n++)
    {
      fin >> weight;
      edgeList[n] = weight;
      if (weight != 0)
      {
        numEdges += 1;
      }
    }

    lengths[m] = numEdges;
    adj[m] = new int[numEdges];
    weights[m] = new double[numEdges];
    
    numEdges = 0;
    for (int a = 0; a < numV; a++)
    {
      if (edgeList[a] != 0)
      {
        adj[m][numEdges] = a;
        weights[m][numEdges] = edgeList[a];
        numEdges += 1;
      }
    }
  
  }  

  delete[] edgeList;

  return numV; 
}

int main(int argc, char** argv)
{
  ifstream fin(argv[1]);
  string source = argv[2];
  string destination = argv[3];
  int numThreads = atoi(argv[4]);

  double** matrix;
  string* labels;
  int numV;

  double* distMatrix;
  int* prevMatrix;
  double* distList;
  int* prevList;
  double* distBF;
  int* prevBF;
  double* distPl;
  int* prevPl;
  int sourceIndex = -1;
  int destinIndex = -1;
  
  int** adj;
  double** weights;
  int* lengths;
  
  numV = readGraph(fin,matrix,labels);

  cout << "Adjacency Matrix" << endl;
  for (int i = 0; i < numV; i++)
  {
    cout << labels[i] << ": ";
    for (int j = 0; j < numV; j++)
    {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }

  cout << endl;

  ifstream fin1(argv[1]);
  delete[] labels;
  numV = readGraph(fin1,adj,weights,lengths,labels);

  cout << "Adjacency List" << endl;
  for (int i = 0; i < numV; i++)
  {
    cout << labels[i] << ": ";
    for (int j = 0; j < lengths[i]; j++)
    {
      cout << labels[adj[i][j]] << "(" << weights[i][j] << ") "; 
    }
    cout << endl;
  }
  
  for (int a = 0; a < numV; a++)
  {
    if (source == labels[a])
    {
      sourceIndex = a;
    }

    if (destination == labels[a])
    {
      destinIndex = a;
    }
  }

  if ((sourceIndex == -1) || (destinIndex == -1))
  {
    cout << "Error: source vertex or destination vertex not in the graph" << endl;
    return 1;
    }

  //Dijkstra for adjacency matrix

  cout << endl;
  cout << "Dijkstra (Matrix)" << endl;
 
  auto startTime = chrono::system_clock::now();
  dijkstra(matrix,numV,sourceIndex,distMatrix,prevMatrix);
  auto endTime = chrono::system_clock::now();
  auto dur = endTime - startTime;
  auto durMS = chrono::duration_cast<chrono::microseconds>(dur);
  double elapsed = durMS.count();
  
  string path = labels[destinIndex];
  int currentIndex = destinIndex;
  while (currentIndex != sourceIndex)
  {
    currentIndex = prevMatrix[currentIndex];
    path = labels[currentIndex] + " -> " + path;
  }
  
  cout << "Path: " << path << " (dist: " << distMatrix[destinIndex] << ")" << endl;
  cout << "Runtime: " << elapsed << " microseconds" << endl;

  cout << endl;

  //Dijkstra for adjacency list

  cout << "Dijkstra (List)" << endl;
  startTime = chrono::system_clock::now();
  dijkstra(adj,weights,lengths,numV,sourceIndex,distList,prevList);
  endTime = chrono::system_clock::now();
  dur = endTime - startTime;
  durMS = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durMS.count();
  
  path = labels[destinIndex];
  currentIndex = destinIndex;
  while (currentIndex != sourceIndex)
  {
    currentIndex = prevList[currentIndex];
    path = labels[currentIndex] + " -> " + path;
  }
  
  cout << "Path: " << path << " (dist: " << distList[destinIndex] << ")" << endl;
  cout << "Runtime: " << elapsed << " microseconds" << endl;

  //Bellman-Ford

  cout << endl;
  cout << "Bellman-Ford" << endl;
 
  startTime = chrono::system_clock::now();
  int cycle = bellmanFord(matrix,numV,sourceIndex,distBF,prevBF);
  endTime = chrono::system_clock::now();
  dur = endTime - startTime;
  durMS = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durMS.count();
  
  if (cycle == 0)
  {
    path = labels[destinIndex];
    currentIndex = destinIndex;
    while (currentIndex != sourceIndex)
    {
      currentIndex = prevBF[currentIndex];
      path = labels[currentIndex] + " -> " + path;
    }
  
    cout << "Path: " << path << " (dist: " << distBF[destinIndex] << ")" << endl;
    cout << "Runtime: " << elapsed << " microseconds" << endl;
  }

  else
  {
    vector<bool> isSeen(numV);
    for (int b = 0; b < numV; b++)
    {
      isSeen[b] = false;
    }
    
    int cycleIndex = cycle - 1;
    int weight = 0;
    int prevIndex;

    while (isSeen[cycleIndex] != true)
    { 
      isSeen[cycleIndex] = true;
      cycleIndex = prevBF[cycleIndex];
    }

    path = labels[cycleIndex];
    currentIndex = cycleIndex;
    prevIndex = prevBF[cycleIndex];

    while (prevIndex != cycleIndex)
    {
      path = labels[prevIndex] + " -> " + path;
      weight += matrix[prevIndex][currentIndex];
      currentIndex = prevIndex;
      prevIndex = prevBF[prevIndex];
    }

    path = labels[prevIndex] + " -> " + path;
    weight += matrix[prevIndex][currentIndex];
  
    cout << "Cycle: " << path << " (weight: " << weight << ")" << endl;
    cout << "Runtime: " << elapsed << " microseconds" << endl;
  }

  //Parallel Dijkstra with 1 thread

  cout << endl;
  cout << "Parallel Dijkstra (1 thread)" << endl;
  startTime = chrono::system_clock::now();
  auto startCPU = clock();
  dijkstra(matrix,numV,sourceIndex,distPl,prevPl,1);
  endTime = chrono::system_clock::now();
  auto endCPU = clock();
  dur = endTime - startTime;
  durMS = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durMS.count();
  
  path = labels[destinIndex];
  currentIndex = destinIndex;
  while (currentIndex != sourceIndex)
  {
    currentIndex = prevPl[currentIndex];
    path = labels[currentIndex] + " -> " + path;
  }
  
  cout << "Path: " << path << " (dist: " << distPl[destinIndex] << ")" << endl;
  cout << "CPU Time: " << (endCPU-startCPU)*(1000000/CLOCKS_PER_SEC) << " microseconds" << endl;
  cout << "Wall Clock Time: " << elapsed << " microseconds" << endl;

  //Parallel Dijkstra with specified number of thread

  cout << endl;
  cout << "Parallel Dijkstra (" << numThreads << " threads)" << endl;
  startTime = chrono::system_clock::now();
  startCPU = clock();
  dijkstra(matrix,numV,sourceIndex,distPl,prevPl,numThreads);
  endTime = chrono::system_clock::now();
  endCPU = clock();
  dur = endTime - startTime;
  durMS = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durMS.count();
  
  path = labels[destinIndex];
  currentIndex = destinIndex;
  while (currentIndex != sourceIndex)
  {
    currentIndex = prevPl[currentIndex];
    path = labels[currentIndex] + " -> " + path;
  }
  
  cout << "Path: " << path << " (dist: " << distPl[destinIndex] << ")" << endl;
  cout << "CPU Time: " << (endCPU-startCPU)*(1000000/CLOCKS_PER_SEC) << " microseconds" << endl;
  cout << "Wall Clock Time: " << elapsed << " microseconds" << endl;

  
  for (int m = 0; m < numV; m++)
  {
    delete[] matrix[m];
    delete[] adj[m];
    delete[] weights[m];
  }

  delete[] matrix;
  delete[] labels;
  delete[] distMatrix;
  delete[] prevMatrix;
  delete[] distList;
  delete[] prevList;
  delete[] distBF;
  delete[] prevBF;
  delete[] distPl;
  delete[] prevPl;
  delete[] lengths;
  delete[] adj;
  delete[] weights;
}
