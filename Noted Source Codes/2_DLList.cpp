#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;


class InvalidParameterException : std::exception{};

//in C++, class definitions can be nested
//To access members of Node from DLList, declare DLList as friend

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
        //Node def. is nested inside the list def. -->> conserving encapsulation

    private:
        Node* mHead;
        Node* mTail;
        int mSize;

    public:     //Access specifier can be used multiple times in the definition.

        //Constructor (construct an empty list)
        DLList():   mHead(nullptr),
                    mTail(nullptr),
                    mSize(0)
        {
        }

        Node* getHead() const {return mHead;}
        Node* getTail() const {return mTail;}
        int getSize() const {return mSize;}

        //All members of Node is private so outer code cannot modify it
        //Insert returns a Node* just for outer code to examine it (it cannot modify the node anyways)
        //Could also return a const pointer but no need due to this definition

        Node* insert(T t, int pos) //data and position
        {
            if(pos > mSize )    // if position is not valid:
                throw InvalidParameterException();

            mSize++;

            //To make pointer updates of predeccor and successor Nodes + pointer sets of new Node
            //We define two pointers

            Node* pred;
            Node* succ;


            //Edge Cases    -->>    To assign pred and succ
            if(pos == 0)
            {
                pred = nullptr;
                succ = mHead;   //if mSize==0 -->> then mHead is nullptr so it also works


                //mHead is the pointer to first element,
                //so successor of this new node is the former first node
                //then the new pointer becomes the first node (Which should be pointed by mHead)
            }
            else
            {
                pred = mHead;

                for(int i=0; i < pos-1 ; ++i)
                //To reach from Node 1 to Node i,
                //we make (i-1) jumps == we use (i-1) links
                //Loop stops when pos-2 jumps made
                //It does Not execute the body when i = (pos-1) meaning (pos-1)th jump is gonna be made
                //So the last value of pred is the Node at position (pos-1) (Not jump)
                {
                    pred = pred -> mNext;
                }

                succ = pred -> mNext;    // new Node will be in between
                //if pred is the last element, then succ will be nullptr
            }

            //Create new Node and set pointers
            Node* newNode = new Node(t, succ, pred);

            //Update pointers of pred and succ



            //Edge cases
            //Update pointers of pred and succ Nodes + mTail and mHead if edge cases
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

            //  if Node is inserted to 0 position of an empty LL -->> it will fall to both if cases and Done.


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
            if(pos >= mSize || pos < 0)	//pos can be at most mSize-1 (since mSize=n means last position is (n-1) )
                throw InvalidParameterException();

            Node* node = mHead;

            for(int i=0; i < pos; ++i)
            // in here, we find the node on position pos
            // make pos-1 jumps
            // if i < (pos-1) was used: it would make pos-2 jumps
            // and find predecessor of the node at pos which is node at poisition (pos-1)
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

            //update pred and succ pointers + mHead and mTail
            if(pred == nullptr) //meaning given Node is the mHead
                mHead = succ;
            else
            {
                pred -> mNext = succ;
            }
            if(succ == nullptr) //meaning given Node is the mTail
                mTail = succ;
            else
            {
                succ -> mPrev = pred;
            }
            //succ,pred,mHead,mTail is updated, update size
            mSize--;

            //delete node
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
                //delete p; //-->> might result in errors:
                removeNode(p);
                p = q;

                //deleting process may delete next node as well making it garbage
                //as a result, next loop crashes trying to delete an ambigous memory area
                //so make prev and next pointers null before deleting : the process in removeNode function !!!
            }

            mHead = nullptr;
            mTail = nullptr;
            //These two lines are unnecessary: its already done in removeNode function

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










