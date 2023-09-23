#include <iostream>

using namespace std;



unsigned primesNaive(unsigned n)
{
    unsigned primeCount = 0;

    for(unsigned p = 2; p < n; ++p)
    {

        unsigned d;

        for(d = 2; d < p; d++)


        {
            if(p % d == 0)
            {
                break;
            }
        }

        if( d == p )
        {
            primeCount++;
        }

    }

    return primeCount;
}
//overall complexity of the algorithm is O(n^2)



unsigned primesClever(unsigned n)
{
    unsigned primeCount = 0;

    for(unsigned p = 2; p < n; ++p)
    {

        unsigned d;

        for(d = 2; d*d < p; d++)


        {
            if(p % d == 0)
            {
                break;
            }
        }

        if( d == p )
        {
            primeCount++;
        }

    }

    return primeCount;
}
//overall complexity of the algorithm is O( n.sqrt(n) )



// The sieve pf Eratosthenes
unsigned primesEratosthenes(unsigned n)
{
    unsigned primeCount = 0;

    // 1. Create a bool array up to n. (index == number)
    // true for prime dividers, false for non_prime, non_devider

    bool *mayBePrime = new bool[n+1];

    // set all to true
    for(unsigned p=2; p<=n; p++)
    {
        mayBePrime[p] = true;
    }

    for(unsigned p=2; p<=n; p++)
    {
        if(mayBePrime[p])
        {
            primeCount++;
            //if true, remove all multiples of that divider.
            for(unsigned m = p*2; m <= n; m+=p)
            {
                mayBePrime[m] =  false;
            }
        }
    }

    /* Since we start from 2 and iterate;
     * the first cycle will start with 2
     * and flag all multiples of 2
     *
     * the next number will be either true or false,
     * if it is true, it means that doesnt have a
     * lessere number that devides it.
     * it is a prime number !
     *
     */

    delete[] mayBePrime;

    return primeCount;
}


int main()
{
    cout << "Hello World!" << endl;
    return 0;
}










