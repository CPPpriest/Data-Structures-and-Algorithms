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

// **********************************
// *** ARRAY BASED SET IMPLEMENTATION
// **********************************

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::ostream;

#include <string>
using std::string;

template<typename T>
class ISet
{
    public:
        virtual int size() const = 0;
        virtual void insert(T t) = 0;
        virtual void remove(T t) = 0;
        virtual bool has(T t) const = 0;
};

class OutOfCapacityException : public std::exception {};

template<typename T>
class ArraySet : public ISet<T>
{
    template<typename TT>
    friend ostream & operator<<(ostream & out, const ArraySet<TT> & s);

    private:
        const int mCapacity;
        T * const mArray;
        int mSize;

    public:
        // In this particular implementation, we allocate a ``fixed capacity''
        // array upon construction.
        ArraySet(int capacity) : mCapacity(capacity),
                                 mArray(new T[capacity]),
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

            while (left <= right)
            {
                int mid = (left + right) / 2;

                if (mArray[mid] == t)
                {
                    return true;
                }
                else
                {
                    if (mArray[mid] > t)
                    {
                        right = mid - 1;
                    }
                    else
                    {
                        left = mid + 1;
                    }
                }
            }

            return false;
        }

        void insert(T t) override
        {
            if (has(t))
            {
                return; // Early return.
            }

            if (mSize == mCapacity)
            {
                throw OutOfCapacityException();
            }

            mSize++;

            int curIndexOfT = mSize - 1;

            while (curIndexOfT != 0 && mArray[curIndexOfT - 1] > t)
            {
                mArray[curIndexOfT] = mArray[curIndexOfT - 1];
                curIndexOfT--;
            }

            mArray[curIndexOfT] = t;
        }

        void remove(T t) override
        {
            if (!has(t))
            {
                return; // Early return.
            }

            int curIndex = mSize - 1;
            T savedValue = mArray[curIndex];

            while (curIndex > 0 && savedValue != t)
            {
                std::swap(savedValue, mArray[curIndex - 1]);
                curIndex--;
            }

            mSize--;
        }
};

template<typename T>
ostream & operator<<(ostream & out, const ArraySet<T> & s)
{
    out << "[mCapacity=" << s.mCapacity
        << ", mSize=" << s.mSize
        << ", mArray={";

    for (int i = 0; i < s.mSize; i++)
    {
        cout << (i > 0 ? ", " : "") << s.mArray[i];
    }

    out << "}]";

    return out;
}

int main()
{
    ArraySet<int> s(10);

    s.insert(5);
    s.insert(3);
    s.insert(8);
    s.insert(1);
    s.insert(9);
    s.insert(2);

    cout << s << endl;

    s.remove(1);
    s.remove(9);
    s.remove(5);

    cout << s << endl;

    cout << s.has(0) << endl;
    cout << s.has(10) << endl;
    cout << s.has(6) << endl;
    cout << s.has(3) << endl;

    return 0;
}
