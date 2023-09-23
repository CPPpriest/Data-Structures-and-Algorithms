#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

//Implement a BST that is used as a map/dictionary
//map[KEY] = Value;

template<typename K, typename V>    // K: Key type, V: Value type
class Map
{
    private:
        //BST Node
        struct Node
        {
            K key;
            V value;
            Node* left;
            Node* right;
        };

        Node* mRoot;

        Node* findNode(K k) const   //Find node with Key "k" and return Node*
        {
            Node* p = mRoot;

            while(p != nullptr)
            {
                //if key is found
                if(p->key == k)
                    return p;

                //Otherwise
                if(p->key > k)
                    p = p -> left;
                else
                    p = p -> right;
            }
            //return nullptr;   //key is not in map -->> supposed to throw an exception
            throw "Exception: Key not found in map!";
        }


        //Very similar to findNode function, But creates a node when it does not exists
        //Search algorithm can be defined iteratively or recursively
        //We use recursive one here -->> it has to take an additional Node* node parameter

        Node* findNodeOrInsert(Node** r, K k)
        //(Node* r, K k)    -->> when r is nullptr, we assigning a Node to it,
        //Since it is a local variable and not connected to anything, it will be a memory link with no usage
        //Take "pointer to Node*" is one way -->> update the pointer and not a copy of it (which is a local variable)
        {
            if(*r == nullptr)//Node is supposed to be inserted here
            {
                *r = new Node { .key = k, .left = nullptr, .right = nullptr };
                // rest init to default datatype values:
                // .value = V(), .left = nullptr, .right = nullptr
                return *r;
            }
            else
            {
                if( (*r) -> key == k)
                    return *r;
                if( (*r) -> key > k)
                {
                    return findNodeOrInsert( &((*r) -> left), k);
                }
                else //if(*r -> key < k)
                {
                    return findNodeOrInsert( &((*r) -> right), k);
                }
            }
        }


        Node* extractMax(Node* t, Node** out)
        {
            //if(t == nullptr){}    // extractMax function is called in the case where t alreadt exists,
                                    // Otherwise it wouldnt be called.

            if(t-> right == nullptr)
            {
                *out = t;
                return t->left; //root of the tree is changed, it becomes the left child.
                                //left child can be nullptr or a node, works either ways
            }

            else
            {
                t->rigt = extractMax(t->right, out);
                return t;
            }


        }

        Node* erase(Node* r, K k)
        {
            if(r == nullptr)//empty tree
            {
                return nullptr; //tree and as a result root is not changed.
            }

            else if( k < r->key)
            {
                r -> left = erase(r->left, k);
                return r;   //there is no change in root
            }

            else if( r->key < k)
            {
                r -> right = erase(r->right, k);
                return r;
            }

            else    // root has the key
            {
                if( (r->right == nullptr) && (r->left == nullptr))
                //Leaf: No left or right subtrees
                {
                    delete r;
                    return nullptr;
                }

                if( r->left == nullptr )
                //One sided: No left subtree
                {
                    Node* temp = r -> right;
                    delete r;
                    return temp;
                }

                if( r->right == nullptr )
                //One sided: No right subtree
                {
                    Node* temp = r -> left;
                    delete r;
                    return temp;
                }

                else
                //Both Left and Right subtrees
                {
                    //Method I: Find largest key on the left

                    /*
                    Node* extracted = extractMax(r -> left);    //Find largest node, isolate it and return
                    r-> key = extracted -> key;
                    r->value = extracted -> value;
                    delete extracted;
                    */

                    //Again r-> left itself can be extracted
                    //So we send the pointer to new root

                    Node* extracted;
                    r->left = extractMax(r -> left, &extracted);    //assign extracted node and return new node
                    r-> key = extracted -> key;
                    r->value = extracted -> value;
                    delete extracted;

                }







            }


        }

    public:
        Map() : mRoot(nullptr) {}

        V get(K k) const //Get value of Key "k".
        {
            return ( findNode(k) -> value );
            //if value found -->> no problem
            //if exception -->> it will return exception here and continue resolving the stack
            //until catch block or termination after halting the first main function that made a call
        }

        void set(K k, V v) //Set value "v" for Key "k".
        {
            //if key/node exists/found node -->> change its value
            //if key/node doesnt exist -->> insert it to the appopriate position

            findNodeOrInsert(& mRoot, k) -> value = v;
            //either finds the node of creates it / then return a pointer to it
        }

        void remove(K k)    //Remove a particular key from map / node from BST
        {
            //could be implemented similar to findNodeOrInsert
            //where address of the root node is sent.
            //Here we return a new root instead: former root if root is not removed, new root if root is the deleted key
            mRoot = erase(mRoot, k);
        }
};


int main()
{
    Map<int, float> m;


    //Better insert in random order.
    for(int i=0; i < 100; ++i)
    {
        m.set(i, (float)i);
    };

    for(int i=0; i < 100; ++i)
    {
        cout << m.get(i) << endl;
    };

    return 0;
}










