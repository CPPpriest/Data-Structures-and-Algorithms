#include <iostream>
using std::cout;
using std::cin;
using std::endl;


template<typename K, typename V>    // K: Key type, V: Value type
class Map
{
    private:
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
                if(p->key == k)
                    return p;

                if(p->key > k)
                    p = p -> left;

                else
                    p = p -> right;
            }
            throw "Exception: Key not found in map!";
        }

        Node* findNodeOrInsert(Node** r, K k)
        {
            if(*r == nullptr)
            {
                *r = new Node { .key = k, .left = nullptr, .right = nullptr };
                return *r;
            }
            else
            {
                if( (*r) -> key == k)
                    return *r;
                if( (*r) -> key > k)
                    return findNodeOrInsert( &((*r) -> left), k);
                else
                    return findNodeOrInsert( &((*r) -> right), k);
            }
        }

        Node* extractMax(Node* t, Node** out)
        {
            if(t-> right == nullptr)
            {
                *out = t;
                return t->left;
            }
            else
            {
                t->rigt = extractMax(t->right, out);
                return t;
            }
        }

        Node* erase(Node* r, K k)
        {
            if(r == nullptr)
                return nullptr;

            else if( k < r->key)
            {
                r -> left = erase(r->left, k);
                return r;
            }

            else if( r->key < k)
            {
                r -> right = erase(r->right, k);
                return r;
            }

            else
            {
                if( (r->right == nullptr) && (r->left == nullptr))
                {
                    delete r;
                    return nullptr;
                }

                if( r->left == nullptr )
                {
                    Node* temp = r -> right;
                    delete r;
                    return temp;
                }

                if( r->right == nullptr )
                {
                    Node* temp = r -> left;
                    delete r;
                    return temp;
                }

                else
                {
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

        V get(K k) const
        {
            return ( findNode(k) -> value );
        }

        void set(K k, V v)
        {
            findNodeOrInsert(& mRoot, k) -> value = v;
        }

        void remove(K k)
        {
            mRoot = erase(mRoot, k);
        }

};

int main()
{
    Map<int, float> m;

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
