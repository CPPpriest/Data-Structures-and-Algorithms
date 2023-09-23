//
// Middle East Technical University
// Dept. of Computer Engineering
//
// CENG 213 - Data Structures
// Dr. Hakan YILDIZ
// hakan@ceng.metu.edu.tr
//
// Not to be reshared without permission.
//

// *******************************************
// *** AN EXAMPLE MAX-HEAP WITH FIXED CAPACITY
// *******************************************

#include <algorithm> // swap.
#include <iostream> // cin, cout, endl.
#include <stdexcept> // exception.
#include <string> // string.

using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::swap;

// Thrown when a pop/peek is attempted on an empty heap.
class HeapEmptyException : public exception {};

// Thrown when a push is attempted on a heap at full capacity, or when a heap
// is constructed on array larger than the given capacity.
class HeapOutOfCapacityException : public exception {};

// Represents a max-heap with fixed capacity.
template<typename T>
class MaxHeap
{
    private:
        T *mHeap;
        unsigned mSize;
        const unsigned mCapacity;

    private:
        // Repeatedly pushes the item at the i-th node up, locally restoring
        // the violated heap orders. The push stops when there is no local
        // violation of the heap order.
        void percolateUp(unsigned i)
        {
            while (i > 1 && mHeap[i / 2] < mHeap[i])
            {
                swap(mHeap[i / 2], mHeap[i]);
                i /= 2;
            }
        }

        // Repeatedly pushes the item at the i-th node down, locally restoring
        // the violated heap orders. The push stops when there is no local
        // violation of the heap order.
        void percolateDown(unsigned i)
        {
            while (2 * i <= mSize)
            {
                unsigned largerChild;

                if (2 * i + 1 > mSize || mHeap[2 * i + 1] < mHeap[2 * i])
                {
                    largerChild = 2 * i;
                }
                else
                {
                    largerChild = 2 * i + 1;
                }

                if (mHeap[i] < mHeap[largerChild])
                {
                    swap(mHeap[i], mHeap[largerChild]);
                    i = largerChild;
                }
                else
                {
                    break;
                }
            }
        }

    public:
        // Constructs an empty max-heap.
        MaxHeap(unsigned capacity) : mHeap(new T[capacity + 1]),
                                     mSize(0),
                                     mCapacity(capacity) {}


        // Constructs a max-heap from the given items, in linear time.
        MaxHeap(unsigned capacity, T items[],  unsigned size)
            : mCapacity(capacity),
              mSize(size),
              mHeap(nullptr)
        {
            if (capacity < size)
            {
                throw HeapOutOfCapacityException();
            }

            // We can allocate the heap array only after making sure that we do
            // not throw the above exception.
            mHeap = new T[capacity + 1];

            for (unsigned i = mSize; i >= 1; i--)
            {
                mHeap[i] = items[i - 1];
                percolateDown(i);
            }
        }

        // Peeks the largest element from the heap.
        T peek() const
        {
            if (mSize == 0)
            {
                throw HeapEmptyException();
            }

            return mHeap[1];
        }

        // Tells whether the heap is empty.
        bool isEmpty() const
        {
            return mSize == 0;
        }

        // Pushes a new element into the heap.
        void push(T t)
        {
            if (mSize == mCapacity)
            {
                throw HeapOutOfCapacityException();
            }

            mSize++;
            mHeap[mSize] = t;

            percolateUp(mSize);
        }

        // Pops the largest element from the heap.
        T pop()
        {
            if (mSize == 0)
            {
                throw HeapEmptyException();
            }

            swap(mHeap[1], mHeap[mSize]);
            mSize--;

            percolateDown(1);

            return mHeap[mSize + 1];
        }
};


int main()
{
    int a[] = {1, 6, 8, 7, 9, 5, 3, 4, 2};
    unsigned n = sizeof(a) / sizeof(a[0]);

    MaxHeap<int> h(n);

    for (auto v : a)
    {
        h.push(v);
    }

    // or MaxHeap<int> h(n, a, n) as a faster alternative.

    for (unsigned i = 0; i < n; i++)
    {
        cout << h.pop() << " ";
    }

    cout << endl;

    return 0;
}
