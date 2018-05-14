class binheap { // max heap
				// because towers will delete the maximum priority enemy from its heap

private:
      enemy *heapArr;
      int heapSize;
      int arraySize;
 
      int getLeftChildIndex(int nodeIndex)
	  {
            return 2 * nodeIndex + 1;
      }
 
      int getRightChildIndex(int nodeIndex)
	  {
            return 2 * nodeIndex + 2;
      }
 
      int getParentIndex(int nodeIndex)
	  {
            return (nodeIndex - 1) / 2;
      }
 
public:
      binheap(int size) 
	  {
            heapArr = new enemy[size];
            heapSize = 0;
            arraySize = size;
      }
 
      bool isEmpty() 
	  {
            return (heapSize == 0);
      }
	
	  void insert(enemy value);
	  void reheapUp(int nodeIndex);
	  void reheapDown(int nodeIndex);
	  int removeMax();

      /*~binheap() {
            delete[] heapArr;
      }*/
};

void binheap::reheapUp(int nodeIndex) 
{
      int parentIndex, tmp1, tmp2;
      if (nodeIndex != 0) 
	  {
            parentIndex = getParentIndex(nodeIndex);
			if (heapArr[parentIndex].Priority < heapArr[nodeIndex].Priority) 
			{
				  tmp1 = heapArr[parentIndex].Priority;
				  tmp2 = heapArr[parentIndex].ID;
				  heapArr[parentIndex].Priority = heapArr[nodeIndex].Priority;
				  heapArr[parentIndex].ID = heapArr[nodeIndex].ID;
				  heapArr[nodeIndex].Priority = tmp1;
				  heapArr[nodeIndex].ID = tmp2;
                  reheapUp(parentIndex);
            }
      }
}
 
void binheap::insert(enemy value) 
{
      if (heapSize == arraySize)
		    cerr << "Heap's underlying storage is overflow" << endl; // c error for cout-ing errors
      else 
	  {
            heapSize++;
			heapArr[heapSize - 1].Priority = value.Priority;
			heapArr[heapSize - 1].ID = value.ID;
            reheapUp(heapSize - 1);
	  }
}

void binheap::reheapDown(int nodeIndex) {
      int leftChildIndex, rightChildIndex, maxIndex, tmp1, tmp2;
      leftChildIndex  = getLeftChildIndex(nodeIndex);
      rightChildIndex = getRightChildIndex(nodeIndex);
      if (rightChildIndex >= heapSize) {
            if (leftChildIndex >= heapSize)
                  return;
            else
                  maxIndex = leftChildIndex;
      } else {
		  if (heapArr[leftChildIndex].Priority >= heapArr[rightChildIndex].Priority)
                  maxIndex = leftChildIndex;
            else
                  maxIndex = rightChildIndex;
      }
	  if (heapArr[nodeIndex].Priority < heapArr[maxIndex].Priority) {
		    tmp1 = heapArr[maxIndex].Priority;
			tmp2 = heapArr[maxIndex].ID;
			heapArr[maxIndex].Priority = heapArr[nodeIndex].Priority;
			heapArr[maxIndex].ID = heapArr[nodeIndex].ID;
			heapArr[nodeIndex].Priority = tmp1;
			heapArr[nodeIndex].ID = tmp2;
            reheapDown(maxIndex);
      }
}

int binheap::removeMax() {
      if (isEmpty())
	  {
            //cout << "Heap is empty" << endl;
			return -1;
	  }
      else 
	  {
		  heapArr[0].Priority = heapArr[heapSize - 1].Priority;
		  heapArr[0].ID = heapArr[heapSize - 1].ID;
          heapSize--;
            if (heapSize > 0)
                  reheapDown(0);
      }
	  return heapArr[0].ID;
}