#include <limits>
#include "BinaryHeap.h"
#include <vector>
#include <thread>

using namespace std;

void dijkstra(const double* const * adjMatrix, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  bool* added = new bool[numVertices];
  for (int m = 0; m < numVertices; m++)
  {
    dist[m] = numeric_limits<double>::infinity();  
    prev[m] = -1;
    added[m] = false;  
  }
  
  added[source] = true;
  dist[source] = 0;
  int currentV = source;
  
  for (int i = 1; i < numVertices; i++)
  {
    for (int j = 0; j < numVertices; j++)
    {
      if ((added[j] == false) && (dist[currentV] + adjMatrix[currentV][j] < dist[j]))
      {
        dist[j] = dist[currentV] + adjMatrix[currentV][j];
        prev[j] = currentV;
      }
    }

    double minimum = numeric_limits<double>::infinity();  
    for (int j = 0; j < numVertices; j++)
    {
      if ((added[j] == false) && (dist[j] < minimum))
      {
        currentV = j;
        minimum = dist[j];
      }
    }  
    added[currentV] = true;
  }

  delete[] added;
}

void dijkstra(const int* const * adj, const double* const * weights, const int* lengths, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  for (int m = 0; m < numVertices; m++)
  {
    dist[m] = numeric_limits<double>::infinity();  
    prev[m] = -1;
  }

  BinaryHeap heap(dist,numVertices);
  heap.decreasePriority(source,0);
  
  int currentV = heap.getMin();
  heap.popMin();
  dist[currentV] = 0;
  
  for (int i = 1; i < numVertices; i++)
  {
    for (int j = 0; j < lengths[currentV]; j++)
    {
      if ((heap.contains(adj[currentV][j])) && (dist[currentV] + weights[currentV][j] < heap.getPriority(adj[currentV][j])))
      {
        heap.decreasePriority(adj[currentV][j],dist[currentV] + weights[currentV][j]);
        prev[adj[currentV][j]] = currentV;
      }
    }

    currentV = heap.getMin();
    dist[currentV] = heap.getPriority(currentV);
    heap.popMin();   
  }

}

int bellmanFord(const double* const * graph, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  for (int h = 0; h < numVertices; h++)
  {
    dist[h] = numeric_limits<double>::infinity();  
    prev[h] = -1;
  }

  vector<pair<int,int> > edges;

  for (int i = 0; i < numVertices; i++)
  {
    for (int j = 0; j < numVertices; j++)
    {
      if ((graph[i][j] != 0) && (graph[i][j] != numeric_limits<double>::infinity()))
      {
        edges.push_back(make_pair(i,j));
      }
    }
  }

  dist[source] = 0;
  int start;
  int end;

  for (int m = 1; m < numVertices; m++)
  {
    for (unsigned int n = 0; n < edges.size(); n++)
    {
      start = edges[n].first;
      end = edges[n].second;
      if (dist[end] > dist[start] + graph[start][end])
      {
        dist[end] = dist[start] + graph[start][end];
        prev[end] = start;
      }
    }
  }

  for (unsigned int a = 0; a < edges.size(); a++)
  {
    start = edges[a].first;
    end = edges[a].second;
    if (dist[end] > dist[start] + graph[start][end])
    {
      return end+1;
    }
  }

  return 0;

}

void initialize(int start, int end, double* dist, int* prev, bool* added, int numThreads)
{ 
  if (end == start + 1)
  {
    dist[start] = numeric_limits<double>::infinity();  
    prev[start] = -1;
    added[start] = false;  
  }

  else
  {
    if (numThreads == 1)
    {
      int midway = (start + end)/2;
      initialize(start,midway,dist,prev,added,1);
      initialize(midway,end,dist,prev,added,1);
    }
    else
    {
      int halfNumThreads = numThreads/2;
      int mid = start + halfNumThreads*((end - start)/numThreads);
	 
      thread leftThread(initialize,start,mid,dist,prev,added,halfNumThreads);	
      initialize(mid,end,dist,prev,added,numThreads - halfNumThreads);	 	
      leftThread.join();
    }	
  } 
}

void getMinVertex(int start, int end, double* dist, bool* added, int numV,int numThreads, int* minVer, double* minimum)
{

  if (end == start + 1)
  {
    if ((added[start] == false) && (dist[start] < *minimum))
    {
      *minVer = start;
      *minimum = dist[start];
    }
  }  
  else
  {
    if (numThreads == 1)
    {
      int midway = (start + end)/2;
      getMinVertex(start,midway,dist,added,numV,1,minVer,minimum);
      getMinVertex(midway,end,dist,added,numV,1,minVer,minimum);      
    }
    else
    {
      int halfNumThreads = numThreads/2;
      int mid = start + halfNumThreads*((end - start)/numThreads);
	 
      thread leftThread(getMinVertex,start,mid,dist,added,numV,halfNumThreads,minVer,minimum);
      getMinVertex(mid,end,dist,added,numV,numThreads - halfNumThreads,minVer,minimum);	      	 	
      leftThread.join();
    }
    
    
  }	 
    
}

void updateDistance(int start, int end, int currentV, const double* const * graph, double* dist, int* prev, bool* added, int numThreads)
{
  if (end == start + 1)
  {
    if ((added[start] == false) && (dist[currentV] + graph[currentV][start] < dist[start]))
    {
      dist[start] = dist[currentV] + graph[currentV][start];
      prev[start] = currentV;
    }
  }

  else
  {
    if (numThreads == 1)
    {
      int midway = (start + end)/2;
      updateDistance(start,midway,currentV,graph,dist,prev,added,1);
      updateDistance(midway,end,currentV,graph,dist,prev,added,1);
    }
    else
    {
      int halfNumThreads = numThreads/2;
      int mid = start + halfNumThreads*((end - start)/numThreads);
	 
      thread leftThread(updateDistance,start,mid,currentV,graph,dist,prev,added,halfNumThreads);
      updateDistance(mid,end,currentV,graph,dist,prev,added,numThreads - halfNumThreads);	      	 	
      leftThread.join();
    }
  }	 
}

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev, int numThreads)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  bool* added = new bool[numVertices];
 
  initialize(0,numVertices,dist,prev,added,numThreads);

  dist[source] = 0;
  int currentV;
  double minimum;
  
  for (int i = 1; i < numVertices; i++)
  {
    minimum = numeric_limits<double>::infinity();
    getMinVertex(0,numVertices,dist,added,numVertices,numThreads,&currentV,&minimum);
    added[currentV] = true;
    updateDistance(0,numVertices,currentV,graph,dist,prev,added,numThreads);
  }

  delete[] added;

}
