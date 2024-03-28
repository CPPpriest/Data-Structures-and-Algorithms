// Set implementation with fixed size array
// Array pointer is const, cant be deleted ---> Memory Leakage
// Elements are comparable and stored in Increasing order in the set

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;


class OutOfCapacityException : public std::exception {};


template<typename T>
class ISet
{
    public:
        virtual int size() const = 0;
        virtual void insert(T t) = 0;
        virtual void remove(T t) = 0;
        virtual bool has(T t) const = 0;
};


template<typename T>
class ArraySet : public ISet<T>
{
    template<typename TT>
    friend std::ostream & operator<<(std::ostream & out, const ArraySet<TT>& s);


    private:
        const int mCapacity;
        T* const mArray;
        int mSize;

    public:
        ArraySet(int capacity) :    mCapacity( capacity ),
                                    mArray( new T[capacity] ),
                                    mSize(0)
        {
        }

        int size() const override
        {
            return mSize;
        }

        bool has(T t) const override
        {
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

        void insert(T t)
        {
            if(has(t))
                return;
            if(mSize == mCapacity)
                throw OutOfCapacityException();

            mSize++;

            int currIndexOfT = mSize - 1;

            while(currIndexOfT != 0 && mArray[currIndexOfT - 1] > t)
            {
                mArray[currIndexOfT] = mArray[currIndexOfT - 1];
                currIndexOfT--;
            }

            mArray[currIndexOfT] = t;
        }


        void remove(T t) override
        {
            if(!has(t))
                return; //Early return


            int currIndex = mSize - 1;
            T savedValue = mArray[currIndex];

            while(currIndex > 0 && savedValue != t)
            {
                std::swap(savedValue, mArray[currIndex - 1]);
                currIndex--;
            }
            mSize--;
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


