#include "BinaryHeap.h"
#include <iostream>

using namespace std;

BinaryHeap::BinaryHeap(const double* priorities, int numItem)
{
  length = numItem;
  weights = new double[length];
  pos = new int[length];
  items = new int[length];
  int toHeapify = length / 2 - 1;

  for (int i = 0; i < length; i++)
  {
    pos[i] = i;
    items[i] = i;
    weights[i] = priorities[i];
  }

  for (int m = toHeapify; m >= 0; m--)
  {
    minHeapify(m);
  }
}

BinaryHeap::~BinaryHeap()
{
  delete[] weights;
  delete[] pos;
  delete[] items;
}

void BinaryHeap::minHeapify(int index)
{
  int left = 2*index + 1;
  int right = 2*index + 2;
  int minPos;
  
  if ((left < length) && (weights[left] < weights[index]))
  {
    minPos = left;
  }
  
  else
  {
    minPos = index;
  }

  if ((right < length) && (weights[right] < weights[minPos]))
  {
    minPos = right;
  }

  if (minPos != index)
  {
    swap(weights[index],weights[minPos]);
    swap(items[index],items[minPos]);
    pos[items[index]] = index;
    pos[items[minPos]] = minPos;
    minHeapify(minPos);
  } 
}

int BinaryHeap::getMin() const
{
  return items[0];
}

void BinaryHeap::popMin()
{
  pos[items[0]] = -1;
  pos[items[length-1]] = 0;
  weights[0] = weights[length-1];
  items[0] = items[length-1];
  length -= 1;
  minHeapify(0);
}


bool BinaryHeap::contains(int item) const
{
  if (pos[item] >= 0)  
  {
    return true;
  }

  return false;
}


double BinaryHeap::getPriority(int item) const
{
  return weights[pos[item]];
}


void BinaryHeap::decreasePriority(int item, double newPriority)
{
  if ((contains(item) == true) && (newPriority < getPriority(item)))
  {
    weights[pos[item]] = newPriority;
    percolateUp(pos[item]);
  }
}

void BinaryHeap::percolateUp(int index)
{
  int curPos = index;
  int parent = (index - 1) / 2;
  while ((curPos > 0) && (weights[parent] > weights[curPos]))
  {
    swap(weights[parent],weights[curPos]);
    swap(items[parent],items[curPos]);
    pos[items[parent]] = parent;
    pos[items[curPos]] = curPos;
    curPos = parent;
    parent = (curPos - 1) / 2;
  }
}

void BinaryHeap::print()
{
  for (int i = 0; i < length; i++)
  {
    cout << weights[i] << " ";
  }
  cout << endl;

}
