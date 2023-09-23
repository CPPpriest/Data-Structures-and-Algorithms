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
// *** AN EXAMPLE BINARY SEARCH TREE BASED MAP
// *******************************************

#include <algorithm> // max, swap.
#include <chrono> // duration, duration_cast, high_resolution_clock.
#include <cstdlib> // rand, srand.
#include <ctime> // time.
#include <iostream> // cout, endl.

using std::cout;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::endl;
using std::max;
using std::rand;
using std::srand;
using std::swap;
using std::time;

// The exception class thrown when a key is not found in the map.
class KeyNotFoundException : public std::exception {};

// An implementation of a ``map'' (also known as a ``dictionary'' or
// ``associative array'') using a binary search tree. The restriction imposed by
// the search tree implementation is that the key type K has to support the
// comparison operators.
template<typename K, typename V>
class BstMap
{
    private:
        // Represents a node of the tree. This declaration is in the private
        // section, because we do not want to reveal the details of the search
        // tree to the users of this map.
        struct Node
        {
            K key;
            V value;
            Node *left;
            Node *right;
        };

        // Pointer to the root node, or nullptr if the tree is empty.
        Node *mRoot;

        // The following private static functions query/modify the binary search
        // tree underlying the map. They are used by the public instance
        // functions at the end of the Map class.

        // A helper function that returns the node that hosts the key k. If
        // there is no such node, a KeyNotFoundException is thrown.
        static Node * findNodeForKey(
            Node *root, // The root of the subtree in which the key is searched.
            K k) // The key to search for.
        {
            Node *t = root;

            while (t != nullptr)
            {
                if (t -> key == k)
                {
                    return t;
                }
                else
                {
                    if (k < t -> key)
                    {
                        t = t -> left;
                    }
                    else
                    {
                        t = t -> right;
                    }
                }
            }

            throw KeyNotFoundException();
        }

        // A helper function that returns the node that hosts the key k. If
        // there is no such node, a new node is created. In contrast to the
        // previous code, this code is written recursively rather than
        // iteratively.
        static Node * findOrInsertNodeForKey(
            Node **root, // The pointer to the root of the subtree, passed by
                         // reference (via a pointer). The reason we pass this
                         // by-reference (with a pointer) is to be able to
                         // modify original link that took us here. In
                         // particular, when we create a new node, we want to
                         // connect it to the original tree and we do it by
                         // updating the link we came through.
                         // [NOTE] This could have been done more cleanly
                         //        (similarly to the functions below) but we
                         //        want to emphasize the usage of a pointer to
                         //        pointer in this example.
            K k) // The key to look for.
        {
            if ((*root) == nullptr)
            {
                *root = new Node { .key = k,
                                   .left = nullptr,
                                   .right = nullptr };
                return *root;
            }
            else
            {
                if ((*root) -> key == k)
                {
                    return *root;
                }
                else
                {
                    if ((*root) -> key < k)
                    {
                        return findOrInsertNodeForKey(&((*root)->right), k);
                    }
                    else
                    {
                        return findOrInsertNodeForKey(&((*root)->left), k);
                    }
                }
            }
        }

        // Extracts the maximum node in the given tree. The address of the
        // extracted node is placed in the pointer pointed by ``out''
        // parameter. This function returns the (new) root of the (sub)tree
        // that underwent the extraction. The root changes in particular if
        // the root itself is extracted. When this function is called,
        // it should be guaranteed that the tree pointed by root is non-empty.
        static Node * extractMax(Node *root, Node **out)
        {
            if (root -> right == nullptr)
            {
                *out = root;
                return root -> left;
            }
            else
            {
                root -> right = extractMax(root -> right, out);
                return root;
            }
        }

        // Erases the entry with key k from the given tree, if any. This
        // function returns the (new) root of the (sub)tree that underwent the
        // erasure. The root changes in particular if the root itself is deleted
        // from the tree.
        static Node * erase(Node *root, K k)
        {
            if (root == nullptr)
            {
                return nullptr;
            }
            else if (k < root -> key)
            {
                root -> left = erase(root -> left, k);
                return root;
            }
            else if (root -> key < k)
            {
                root -> right = erase(root -> right, k);
                return root;
            }
            else
            {
                if (root -> left == nullptr && root -> right == nullptr)
                {
                    delete root;
                    return nullptr;
                }
                else if (root -> left == nullptr)
                {
                    Node *temp = root -> right;
                    delete root;
                    return temp;
                }
                else if (root -> right == nullptr)
                {
                    Node *temp = root -> left;
                    delete root;
                    return temp;
                }
                else
                {
                    Node *extracted;

                    root -> left = extractMax(root->left, &extracted);
                    root -> key = extracted -> key;
                    root -> value = extracted -> value;

                    delete extracted;

                    return root;
                }
            }
        }

        // Fills the array pointed by ``out'' with the keys of the tree pointed
        // by ``root'', arranged as in an ``in-order'' traversal. Recall that
        // the ``in-order'' traversal gives the sorted order of the keys. The
        // function returns a new pointer pointing to where the filling should
        // continue.
        static K * keysToArray(const Node *root, K *out)
        {
            if (root == nullptr)
            {
                return out;
            }
            else
            {
                out = keysToArray(root -> left, out);
                out[0] = root -> key;
                return keysToArray(root -> right, out + 1);
            }
        }

        // Calculates the height (depth) of the tree pointed by root.
        static unsigned height(const Node *root)
        {
            if (root == nullptr)
            {
                return 0;
            }
            else
            {
                return max(height(root -> left), height(root -> right)) + 1;
            }
        }

        // Recursively destroys the tree. This is an example of a ``post-order''
        // traversal.
        static void destroy(Node *root)
        {
            if (root != nullptr)
            {
                destroy(root -> left);
                destroy(root -> right);
                delete root;
            }
        }

    public:
        // Constructs an empty map.
        BstMap() : mRoot(nullptr) {}

        // Sets map[k] = v.
        void set(K k, V v)
        {
            findOrInsertNodeForKey(&mRoot, k) -> value = v;
        }

        // Gets map[k].
        V get(K k) const
        {
            return findNodeForKey(mRoot, k) -> value;
        }

        // Erases map[k]. This will free memory and call destructors as needed.
        void erase(K k)
        {
            mRoot = erase(mRoot, k);
        }

        // Returns the height of the binary search tree underlying the map. Yes,
        // this function goes against the idea of hiding the search tree details
        // but we needed it to show you the logarithmic height of the tree on
        // random situations.
        unsigned height() const
        {
            return height(mRoot);
        }

        // Fills the ``out'' array with the keys of this map, in sorted order.
        void keysToArray(K out[]) const
        {
            keysToArray(mRoot, out);
        }

        // Destructor. Destroys the tree.
        ~BstMap()
        {
            destroy(mRoot);
        }
};


// Randomly permutes an array a of size n. The details are not important.
template<typename T>
void permuteRandomly(T a[], unsigned n)
{
    srand(time(0));

    for (unsigned i = 0; i < n; i++)
    {
        unsigned randomIndex = rand() % (n - i) + i;
        swap(a[i], a[randomIndex]);
    }
}

// Our binary search tree sort. Since we are using the Map's search tree for
// this purpose, this sort will not work if the array contains duplicate values.
// Of course, this would not be an issue if we had written a search tree that
// allowed duplicate keys (but we could not use that for a map).
template<typename T>
void bstSort(T a[], unsigned n)
{
    permuteRandomly(a, n); // Permute randomly to increase the likelihood of
                           // logarithmic depth. O(n) worst-case time.

    BstMap<T, char> map;

    for (unsigned i = 0; i < n; i++)
    {
        map.set(a[i], '\0'); // The value is not important. We simply want
                             // the sorted order of the keys.
    }

    // O(nlogn) ``expected'' ``worst-case'' time.
    // (When randomization is intentionally introduced by an algorithm, we
    // prefer to use the term "expected time" rather than "average-case time".
    // "Average-case time'' usually refers to the expected time complexity over
    // the ``probability distribution of the input''. Here, we intentionally
    // permute the input as part of the algorithm, so that we will have good
    // ``expected time'' in the ``worst-case''. This is an example of a
    // ``randomized algorithm''.

    map.keysToArray(a); // Output keys back to ``a'' in sorted ``in-order''
                        // order. O(n) worst-case time.
}

// Some functions that we wrote for testing.

// Generates 1, 2, ..., n on out[]. The details are not important.
void generateIncreasingSequence(int out[], unsigned n)
{
    for (unsigned i = 0; i < n; i++)
    {
        out[i] = (int) i + 1;
    }
}

// Checks if a[] is 1, 2, ..., n. The details are not important.
bool isIncreasingSequence(int a[], unsigned n)
{
    for (unsigned i = 0; i < n; i++)
    {
        if (a[i] != (int) i + 1)
        {
            return false;
        }
    }

    return true;
}

// ``Exchange-Sort'', probably the simplest sort algorithm.
// Runs in O(n^2). The details are not important.
template<typename T>
void exchangeSort(T *a, unsigned n)
{
    for (unsigned i = 0; i < n; i++)
    {
        for (unsigned j = i + 1; j < n; j++)
        {
            if (a[j] < a[i])
            {
                std::swap(a[i], a[j]);
            }
        }
    }
}

// Function to test a given sort function on a randomly permuted integer array
// of size n. The details are not important.
void testSortFunction(void (*sort)(int[], unsigned), unsigned n)
{
    int *a = new int[n];

    generateIncreasingSequence(a, n);
    permuteRandomly(a, n);

    auto start = high_resolution_clock::now();

    sort(a, n);

    auto end = high_resolution_clock::now();

    auto seconds = (duration_cast<duration<float>>(end - start)).count();

    cout << "Sorting took " << seconds << " seconds." << endl;

    if (isIncreasingSequence(a, n))
    {
        cout << "The array is correctly sorted." << endl;
    }
    else
    {
        cout << "The array is NOT correctly sorted." << endl;
    }

    delete [] a;
}

// Code to test sorting.
void testSorting()
{
    cout << "Testing exchangeSort on a randomly permuted "
         << "array of 50000 integers..."
         << endl;

    testSortFunction(exchangeSort, 50000);

    cout << endl;

    cout << "Testing bstSort on a randomly permuted "
         << "array of 50000 integers..."
         << endl;

    testSortFunction(bstSort, 50000);
}

// Code to test Map.
void testMap()
{
    const int n = 1000000;
    int keys[n];

    generateIncreasingSequence(keys, n);
    permuteRandomly(keys, n);

    BstMap<int, double> map;

    for (unsigned i = 0; i < n; i++)
    {
        map.set(keys[i], double(keys[i]));
    }

    cout << "Height of the BST after " << n
         << " random insertions is " << map.height()
         << "." << endl;

    permuteRandomly(keys, n);

    for (unsigned i = 0; i < n; i += 2) // Erase even-indexed keys.
    {
        map.erase(keys[i]);
    }

    for (unsigned i = 0; i < n; i++)
    {
        bool isCorrect = false;

        if (i % 2 == 0)
        {
            try
            {
                map.get(keys[i]);
            }
            catch (const KeyNotFoundException &)
            {
                isCorrect = true; // Expected exception for erased keys.
            }
        }
        else
        {
            try
            {
                if (double(keys[i]) == map.get(keys[i]))
                {
                    isCorrect = true; // Original value for other keys.
                }
            }
            catch (const KeyNotFoundException &) {}
        }

        if (!isCorrect)
        {
            cout << "get() result incorrect. Map broken?" << endl;
            return;
        }
    }

    cout << "All Map tests passed." << endl;
}

int main()
{
    testSorting();

    cout << endl;

    testMap();

    return 0;
}
