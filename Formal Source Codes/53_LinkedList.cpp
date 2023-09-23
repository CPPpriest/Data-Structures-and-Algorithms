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

// ******************************
// *** DOUBLY LINKED LIST EXAMPLE
// ******************************

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

class InvalidParameterException : public std::exception {};

template<typename T>
class DLList
{
    public:
        class Node
        {
            friend class DLList;

            private:
                T mData;
                Node *mNext;
                Node *mPrev;

                Node(T t, Node *n, Node *p) : mData(t),
                                              mNext(n),
                                              mPrev(p)
                {}

            public:
                T getData() const { return mData; }
                Node * getNext() const { return mNext; }
                Node * getPrev() const { return mPrev; }
        };

    private:
        Node *mHead;
        Node *mTail;
        unsigned mSize;

    public:
        DLList() : mHead(nullptr), mTail(nullptr), mSize(0)
        {}

        Node *head() const { return mHead; }
        Node *tail() const { return mTail; }
        unsigned size() const { return mSize; }

        Node * insert(T t, unsigned pos)
        {
            if (pos > mSize)
            {
                throw InvalidParameterException();
            }

            mSize++;

            Node *pred;
            Node *succ;

            if (pos == 0)
            {
                pred = nullptr;
                succ = mHead;
            }
            else
            {
                pred = mHead;

                for (unsigned i = 0; i < pos - 1; i++)
                {
                    pred = pred -> mNext;
                }

                succ = pred -> mNext;
            }

            Node *newNode = new Node(t, succ, pred);

            if (pred == nullptr)
            {
                mHead = newNode;
            }
            else
            {
                pred -> mNext = newNode;
            }

            if (succ == nullptr)
            {
                mTail = newNode;
            }
            else
            {
                succ -> mPrev = newNode;
            }

            return newNode;
        }

        Node * insertAfter(T t, Node *predecessor)
        {
             // LEFT AS AN EXERCISE.
        }

        Node * insertBefore(T t, Node *successor)
        {
             // LEFT AS AN EXERCISE.
        }

        void remove(Node *node)
        {
            // LEFT AS AN EXERCISE.
        }

        ~DLList()
        {
            Node *p = mHead;

            while (p != nullptr)
            {
                Node *n = p -> mNext;
                delete p;
                p = n;
            }
        }
};



int main()
{
    DLList<float> l;

    l.insert(1.1f, 0);
    l.insert(2.2f, 1);
    l.insert(3.3f, 0);
    l.insert(4.4f, 1);
    l.insert(5.5f, 2);

    for (DLList<float>::Node *p = l.head(); p != nullptr; p = p->getNext())
    {
        cout << p->getData() << endl;
    }

    return 0;
}
