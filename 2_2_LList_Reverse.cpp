#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

class InvalidParameterException : std::exception{};

template<typename T>
class LList
{
    template<typename TT>
    friend std::ostream& operator<<(std::ostream& out, LList<TT>& list);

    public:
        class Node
        {
            friend class LList;

            private:
                T mData;
                Node* mNext;

                Node(T t, Node* n):    mData(t), mNext(n)
                {
                }

            public:
                T getData() const{return mData;}
                Node* getNext() const{return mNext;}
        };

    private:
        Node* mHead;
        int mSize;

    public:

        LList():   mHead(nullptr),mSize(0)
        {
        }

        Node* getHead() const {return mHead;}
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

            Node* newNode = new Node(t, succ);

            if(pred==nullptr)
                mHead = newNode;
            else
            {
                pred -> mNext = newNode;
            }

            return newNode;
        }


        void remove(int pos)
        {

            //1) Check is pos valid
            if(mSize == 0 )
                throw InvalidParameterException();

            if(pos >= mSize || pos < 0)
                throw InvalidParameterException();

            //2)Assign pred and node and succ

            Node* pred;
            Node* node;
            Node* succ;

            if(pos==0){
                pred=nullptr;
                node=mHead;
            }
            else
            {
                pred = mHead;
                for(int i=0; i < pos-1 ; ++i)
                {
                    pred = pred -> mNext;
                }
                node = pred->mNext;
            }

            succ = node->mNext; //node known to be exists. it has a next.

            //3) delete node

            node->mNext = nullptr;
            delete node;

            //4)    Update prev and succ
            //      Update mHead

            if(pred == nullptr)
                mHead = succ;
            else
            {
                pred -> mNext = succ;
            }

            //5) Update Size !!!

            mSize--;

        }

        void reverseLL()
        {
            // 1)Empty or 1 element list
            if( (mHead == nullptr) || (mHead->mNext == nullptr) ) //  if( (!curr) || (!curr->next) )
            {
                return;
            }

            // 2)pred, curr, succ pointers
            Node* curr = mHead;
            Node* pred = nullptr;
            Node* succ;

            // 3) Loop until curr is nullptr
            while(curr != nullptr)
            {
                // a) Save rest of the list
                succ = curr -> mNext;

                // b) Reverse current node's pointer
                curr->mNext = pred;

                // c) Shift pred and curr pointers.
                pred = curr;
                curr = succ;
            }

            // 4) Update mHead

            mHead = pred;

            //Remark:   if mHead == nullptr, loop body does not execute
            //          if mHead -> next == nullptr,    loop body executes once which doesnt change anything of LL.
            //                                          only makes mHead -> mNext = nullptr; which already is.
            // So, only the loop could have been used."


        }



        ~LList()
        {
            while(mSize != 0)
            {
                remove(0);
                //This is updated: In this version there is no remove(Node* node) function defined.
                //But it can also be easily implemented.
            }

            mHead = nullptr;    //this is also unnecessary!

            cout << "LList is deleted" << endl;
        }


};

template<typename T>
std::ostream& operator<<(std::ostream& out, LList<T>& list)
{
    typename LList<T>::Node* curr = list.mHead;
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
    LList<int> linkedList;

    linkedList.insert(12,0);
    linkedList.insert(13,0);
    linkedList.insert(14,0);
    linkedList.insert(15,0);
    linkedList.insert(22,1);
    linkedList.insert(33,2);
    linkedList.insert(44,3);
    linkedList.insert(87,7);
    linkedList.insert(102,8);
    linkedList.insert(65,9);
    linkedList.insert(91,10);
    cout << linkedList << endl;

    linkedList.remove(5);
    linkedList.remove(0);
    cout << linkedList << endl;

    linkedList.reverseLL();
    cout << linkedList << endl;




    return 0;
}










