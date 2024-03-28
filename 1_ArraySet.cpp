// Set implementation with array (dynamic array)
// Elements are comparable and stored in Increasing order in the set

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;


class OutOfCapacityException : public std::exception {};

//---- Interface for SET ---//
template<typename T>
class ISet
{
    public:
        virtual int size() const = 0;
        virtual void insert(T t) = 0;
        virtual void remove(T t) = 0;
        virtual bool has(T t) const = 0;
};

//--- Array Set Implementation --//
template<typename T>
class ArraySet : public ISet<T>
{
    template<typename TT>
    friend std::ostream & operator<<(std::ostream & out, const ArraySet<TT>& s);


    private:
        const int mCapacity;   // maximum capacity of array (memory), m is for member
        T* const mArray;            // pointer to array
        int mSize;             // current size of the array (Number of elements)

    // const members can only be initialized in initializer list of constructor
    // cannot be modified after init.
    // can be deleted.

    public:
        ArraySet(int capacity) :   mCapacity( capacity ),
                                        mArray( new T[capacity] ),  //fixed array, not dynamically chnaging
                                        mSize(0)
        {
        }

        int size() const override
        {
            return mSize;
        }

        // Binary Search
        /*
         * Constant time random access to elements by index -->> array
         * Sorted order -->> requires comparable elements
         *
         * Based on the ASSUMPTION: T type supports comparison !
         *
         */


        // Edge Cases
        /*
         *  l = r       -->     mid = l = r
         *                          if t==mid   -> true
         *                          if t!=mid   -> l > r (inconsistent)
         *
         *  l + 1 = r   -->     mid = l
         *                          if t==mid(l)-> true
         *                          if t>mid    -> l = r (l = mid +1 = r)   inconsistent (since t != l in the first place)
         *
         *                          if t<mid    -> l = r (r = mid -1 = r)   (l = r case where r is not compared with t yet)
         *
         *  l + 1 < r   -->     consistent
         *
         *
         *  Basically,  if l exceeds r,     return false
         *              otherwise,          continue
         */

        bool has(T t) const override
        {
            /*
            int left = 0;
            int right = mSize - 1; // if unsigned is used; mSize=0 -> results in right = 255 or larger value
            */
            int left = 0;
            int right = mSize - 1;

            while(left <= right)
            {
                int mid = (left + right)/2;

                if(mArray[mid] == t)
                    return true;
                else
                {
                    if(mArray[mid] > t)
                    {
                        right = mid - 1;
                    }
                    else    // if(mArray[mid] < t)
                    {
                        left = mid + 1;
                    }
                }
            }

            return false;
        }

        //Edge cases
        /*
         * -Element already exists in set
         *
         * -Array is full
         *
         */

        void insert(T t)
        {
            if(has(t))
                return; //Early return
            if(mSize == mCapacity)
                throw OutOfCapacityException(); //Early exception

            //mArray[mSize] = t;    //useless since during shifted, its value is overwritten
            mSize++;

            int currIndexOfT = mSize - 1;

            while(currIndexOfT != 0 && mArray[currIndexOfT - 1] > t)
            {
                mArray[currIndexOfT] = mArray[currIndexOfT - 1];
                currIndexOfT--;
            }

            mArray[currIndexOfT] = t;
        }

        //Edge cases
        /*
         * -Element does NOT exist in set
         *
         * -Array size is 0
         *
         */

        void remove(T t) override
        {
            if(!has(t))
                return; //Early return


            // Since set already has t ---> mSize != 0
            /*
                if(mSize == 0)
                    throw EmptySetException(); //Early exception
            */

            int currIndex = mSize - 1;

            // to left shift, overwritten value must be save until reaching t
            T savedValue = mArray[currIndex];

            while(currIndex > 0 && savedValue != t)         // if currIndex == 0, mArray[currIndex - 1] is invalid ??
                                                            // if currIndex is 0, then it must be t.
            {
                std::swap(savedValue, mArray[currIndex - 1]);
                currIndex--;
            }

            mSize--;

            //Remark:   This method leaves the last value on its index.
            //          So if t != last element in the beginning, last element repeats
            //          Since mSize is decreased by one, it is just a garbage value which is ignored by the implementation
            //          its value will be overwritten by insert without being read.

        }
};



template<typename T>
std::ostream & operator<<(std::ostream & out, const ArraySet<T>& s)
{
    out <<  "[mCapacity="   << s.mCapacity
        <<  ", mSize="  << s.mSize
        <<  ", mArray={";

    for(int i=0; i<s.mSize; ++i)
    {
        out << ( (i > 0)? ", " : ""  )<< s.mArray[i];
    }

    out << "} ]";

    return out;
}







int main()
{
    cout << "Hello World!" << endl;

    ArraySet<int> mySet(24);

    mySet.insert(11);
    mySet.insert(5);
    mySet.insert(7);
    mySet.insert(89);
    mySet.insert(17);
    mySet.insert(0);
    mySet.insert(34);
    mySet.insert(60);

    cout << mySet << endl;

    mySet.remove(11);
    mySet.remove(0);
    mySet.remove(89);
    mySet.remove(34);

    cout << mySet << endl;

    cout << mySet.has(11) << endl;
    cout << mySet.has(17) << endl;

    return 0;
}




