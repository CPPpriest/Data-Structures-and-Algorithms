#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

class InvalidParameterException : std::exception{};

template<typename T>
class DLList
{
    template<typename TT>
    friend std::ostream& operator<<(std::ostream& out, DLList<TT>& list);

    public:
        class Node
        {
            friend class DLList;

            private:
                T mData;
                Node* mNext;
                Node* mPrev;

                //Private constructor
                Node(T t, Node* n, Node* p):    mData(t),
                                                mNext(n),
                                                mPrev(p)
                {
                }

            public:
                T getData() const{return mData;}
                Node* getNext() const{return mNext;}
                Node* getPrev() const{return mPrev;}
        };

    private:
        Node* mHead;
        Node* mTail;
        int mSize;

    public:

        DLList():   mHead(nullptr),
                    mTail(nullptr),
                    mSize(0)
        {
        }

        Node* getHead() const {return mHead;}
        Node* getTail() const {return mTail;}
        int getSize() const {return mSize;}

        Node* insert(T t, int pos)
        {
            if(pos > mSize )
                throw InvalidParameterException();

            mSize++;

            Node* pred;
            Node* succ;

            if(pos == 0)
            {
                pred = nullptr;
                succ = mHead;
            }
            else
            {
                pred = mHead;

                for(int i=0; i < pos-1 ; ++i)
                {
                    pred = pred -> mNext;
                }

                succ = pred -> mNext;
            }

            Node* newNode = new Node(t, succ, pred);

            if(pred==nullptr)
                mHead = newNode;
            else
            {
                pred -> mNext = newNode;
            }

            if(succ==nullptr)
                mTail=newNode;
            else
            {
                succ -> mPrev = newNode;
            }

            return newNode;
        }

        Node* insertAfter(T t, Node* predecessor)
        {
            //Edge cases:
            //  if predecessor == nullptr   --> insert to beginning == mHead is modified
            //  if predecessor -> next ==  nullptr  --> mTail is modified

            Node* pred = predecessor;
            Node* succ;

            if(pred == nullptr)
            {
                succ = mHead;
            }
            else
            {
                succ = pred->mNext;
            }

            Node* newNode = new Node(t,succ,pred);

            //Update pred and succ pointers + mHead and mTail

            if(pred == nullptr)
            {
                mHead = newNode;
            }
            else
            {
                pred-> mNext = newNode;
            }
            if(succ == nullptr)
            {
                mTail = newNode;
            }
            else
            {
                succ->mPrev = newNode;
            }

            return newNode;
        }

        Node* insertBefore(T t, Node* successor)
        {
            Node* pred;
            Node* succ = successor;

            if(successor == nullptr)
            {
                pred = mTail;
            }
            else
            {
                pred = succ->mPrev;
            }

            //create newNode
            Node* newNode = new Node(t,succ,pred);


            //Update pred and succ pointers + mHead and mTail

            if(pred == nullptr)
            {
                mHead = newNode;
            }
            else
            {
                pred-> mNext = newNode;
            }
            if(succ == nullptr)
            {
                mTail = newNode;
            }
            else
            {
                succ->mPrev = newNode;
            }

            return newNode;
        }

        void remove(int pos)
        {
            if(pos >= mSize || pos < 0)
                throw InvalidParameterException();

            Node* node = mHead;

            for(int i=0; i < pos; ++i)
            {
                node = node -> mNext;
            }

            removeNode(node);
        }

        void removeNode(Node* node)
        {
            //Assumption: node already exists in the list
            Node* pred = node->mPrev;
            Node* succ = node->mNext;

            if(pred == nullptr)
                mHead = succ;
            else
            {
                pred -> mNext = succ;
            }
            if(succ == nullptr)
                mTail = succ;
            else
            {
                succ -> mPrev = pred;
            }
            mSize--;

            node->mNext = nullptr;
            node->mPrev = nullptr;
            delete node;
        }

        ~DLList()
        {
            Node* p = mHead;
            Node* q;

            while(p != nullptr)
            {
                q = p -> mNext;
                removeNode(p);
                p = q;
            }

            mHead = nullptr;
            mTail = nullptr;

            cout << "DLList is deleted" << endl;
        }


};

template<typename T>
std::ostream& operator<<(std::ostream& out, DLList<T>& list)
{
    typename DLList<T>::Node* curr = list.mHead;
    cout << "Print Linked List" << endl;
    for(int i=0; i < list.getSize() ; ++i)
    {
        out << i << "): " << curr->getData() << endl;
        curr = curr -> getNext();
    }

    return out;
}



int main()
{
    DLList<int> linkedList;

    linkedList.insert(12,0);
    linkedList.insert(13,0);
    linkedList.insert(14,0);
    linkedList.insert(15,0);
    cout << linkedList << endl;

    linkedList.insert(22,1);
    linkedList.insert(33,2);
    linkedList.insert(44,3);
    cout << linkedList << endl;

    linkedList.remove(3);
    cout << linkedList << endl;

    linkedList.remove(5);
    cout << linkedList << endl;

    linkedList.remove(0);
    cout << linkedList << endl;


    return 0;
}










