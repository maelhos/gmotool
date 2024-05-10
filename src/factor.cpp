#include "factor.h"

factorization factor(mpz_ptr n){
    factorization ret{};
    if  (mpz_probab_prime_p(n, 50)){
        fac_pair p{};
        mpz_init(p.first);
        mpz_set(p.first, n);
        p.second = 1;
        ret.push_back(p);

        return ret;
    }
    std::cout << "Couldn't factor n..." << std::endl;
    exit(1);
}