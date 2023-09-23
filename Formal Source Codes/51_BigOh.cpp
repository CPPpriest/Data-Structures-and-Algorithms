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

// *******************
// *** BIG OH EXAMPLES
// *******************

#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

#include <iomanip>
using std::fixed;
using std::setprecision;

#include <iostream>
using std::cout;
using std::endl;


unsigned primesNaive(unsigned n)
{
    unsigned primeCount = 0; // Cost: O(1). Negligible compared to the rest.

    for (unsigned p = 2; p <= n; p++) // Iterates (n - 1) = O(n) times.
    {
        unsigned d;

        for (d = 2; d < p; d++) // Iterates (p - 2) time. Cost: O(p).
        {
            if (p % d == 0)
            {
                break; // We ignore the diminishing effect of this break.
                       // Thus, we have to conclude the analysis with Big O.
            }
        }

        if (d == p) // Cost: O(1). Negligible compared to the rest.
        {
            // cout << p << endl;
            primeCount++;
        }
    }

    // The overall cost is dominated by the total cost of the inner loop.
    // Ignoring constant factors, we can bound this cost as:
    //     2 + 3 + ... + n = (1 + ... + n) - 1 = (n^2 + n) / 2 - 1 = O(n^2)

    return primeCount;
}


unsigned primesClever(unsigned n)
{
    unsigned primeCount = 0; // Cost: O(1)

    for (unsigned p = 2; p <= n; p++) // Cost (ignoring inner loop): O(n)
    {
        unsigned d;

        for (d = 2; d * d <= p; d++) // Cost: O(sqrt(p))
        {
            if (p % d == 0)
            {
                break; // Similar ignorance here.
            }
        }

        if (d * d > p) // Cost: O(1)
        {
            // cout << p << endl;
            primeCount++;
        }
    }

    // The overall cost is dominated by the total cost of the inner loop.
    // Ignoring constant factors, we can bound this cost as:
    //        sqrt(2) + sqrt(3) + ... + sqrt(n)
    //     <= sqrt(n) + sqrt(n) + ... + sqrt(n)
    //      = (n-1) * sqrt(n)
    //      = O(n * sqrt(n))

    return primeCount;
}


// An implementation of ``the sieve of Eratosthenes''.
unsigned primesEratosthenes(unsigned n)
{
    unsigned primeCount = 0; // Cost: O(1)

    bool *mayBePrime = new bool[n + 1]; // Cost: O(1) NOTE: Actually, the cost
                                        //                  depends on the
                                        //                  memory manager.

    for (unsigned p = 2; p <= n; p++) // Cost: O(n).
    {
        mayBePrime[p] = true;
    }

    for (unsigned p = 2; p <= n; p++) // Cost (ignoring inner loop): O(n)
    {
        if (mayBePrime[p])
        {
            // cout << p << endl;
            primeCount++;

            for (unsigned m = p * 2; m <= n; m += p) // Cost: O(n/p).
            {
                mayBePrime[m] = false;
            }
        }
    }

    delete [] mayBePrime; // Cost: O(1) NOTE: Actually, the cost depends on the
                          //                  memory manager.

    // The overall cost is dominated by the total cost of the inner loop.
    // Ignoring constant factors, we can bound this cost as:
    //        n/2 + n/3 + n/5 + n/7 + n/11 + n/13 ... + n/<largest prime <= n>
    //     <= n/2 + n/3 + n/4 + n/5 + ... + n/n
    //      = n * (1/2 + 1/3 + ... + 1/n)
    //      = n * sum_{i=2}^{n}{1/i}
    //     <= n * integral_{i=1}^{n}{1/i di}
    //      = n * (ln n - ln 1)
    //      = n ln n
    //      = O(nlogn)

    return primeCount;
}


// Euclid's GCD Algorithm
unsigned gcd(unsigned small, unsigned large)
{
    cout << "gcd(" << small << "," << large << ") called." << endl;

    if (small == 0)
    {
        return large;
    }
    else
    {
        // A divisor ``d'' of ``small'' and ``large'' also divides
        // ``large % small''. To see this:
        //     small = sd
        //     large = ld
        //     large % small = ld % sd = ld - k * sd = (l - ks)d
        //
        // Thus, in general, gcd(small, large) = gcd(large % small, small).
        return gcd(large % small, small);

        // The exception is when ``large'' is a multiple of ``small'', but this
        // terminates the algorithm:
        //
        //     gcd(small, large) -> gcd(0, small) -> small
        //
        // The ``small'' parameter is guaranteed to shrink at every recursive
        // call. It will eventually reach 0, thus the algorithm guarantees to
        // terminate.
    }
}

// To analyze the time bound, we notice the following: After two recursive
// calls, ``small'' is halved (at the least). Observe that, after two calls,
// we get to:
//
//        gcd(small, large)
//     -> gcd(large % small, small)
//     -> gcd(small % (large % small), large % small)
//
// ``small % (large % small)'' is always <= ``small / 2''.
// (Imagine two cases: (a) ``large % small'' <= ``small / 2'',
//                     (b) ``small / 2'' < ``large % small'' < ``small''.)

// This is enough to see that ``small'' will become zero within ~log2(small)
// steps.

// To formally prove this bound, one can also write a ``recurrence relation''
// for the time bound. Define ``n'' to be the small number. Let T(n) denote
// the highest cost of solving the problem for a particular n. (We say the
// ``highest'' cost, because ``n'' does not fix ``large''.) After two steps of
// recursion, we get down to a problem with size n' <= n/2, by paying an
// (imaginary) cost of 1. Thus, we can write the following recurrence.
//
//     T(n) <= T(k) + 1 where k <= n/2. [For n > 0.]
//     and
//     T(0) = 1
//
// One prove by induction that T(n) <= c * log2(n) for c >= 1 and n >= 1. This
// is enough to argue T(n) = O(logn).
//
// To sum up, gcd(x, y) can be computed in O(log min(x,y)) time, assuming that
// one can apply the modulo operation in constant time.


// A performance measuring code for testing prime algorithms. Details are not
// important.
void measure(const char *name,
             unsigned (*f)(unsigned),
             unsigned n)
{
    cout << "Testing ``" << name << "''..." << endl;

    cout.flush();

    auto start = high_resolution_clock::now();

    unsigned primeCount = f(n);

    auto end = high_resolution_clock::now();

    float elapsedSeconds = duration_cast<duration<float>>(end - start).count();

    cout << fixed << setprecision(4) << elapsedSeconds << " seconds elapsed. "
         << primeCount << " primes found in the range 1.." << n << "." << endl;
}


int main()
{
    const int n = 200000;

    measure("Naive Algorithm", primesNaive, n);
    measure("Clever Algorithm", primesClever, n);
    measure("Sieve of Eratosthenes", primesEratosthenes, n);

    cout << endl << "Running gcd(192, 270):" << endl;

    auto theGcd = gcd(192, 270);

    cout << "The GCD of 192 and 270 is " << theGcd << "." << endl;

    return 0;
}
