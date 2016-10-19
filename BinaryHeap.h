#ifndef BINARYHEAP
#define BINARYHEAP

class BinaryHeap
{
  private:
   double* weights;
   int* pos;
   int* items;
   int length;
   void minHeapify(int index);
   void percolateUp(int index);

  public:
   BinaryHeap(const double* priorities, int numItem);
   ~BinaryHeap();
   int getMin() const;
   void popMin();
   bool contains(int item) const;
   double getPriority(int item) const;
   void decreasePriority(int item, double newPriority);
   void print();
};


#endif
