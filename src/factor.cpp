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

    double density = (double)mpz_popcount(n) / (double)mpz_sizeinbase(n, 2);
    if (density < 0.1){
        std::cout << "Low density detected : " << density << std::endl;
        fac_pair p{};
        fac_pair q{};

        mpz_inits(p.first, q.first, 0);
        factor_low_density(p.first, n);

        if(mpz_cmp_ui(p.first, 0)){
            p.second = 1;
            mpz_divexact(q.first, n, p.first);
            q.second = 1;
            ret.push_back(p);
            ret.push_back(q);

            return ret;
        }
        mpz_clears(p.first, q.first, 0);
        std::cout << "DFS failed ..." << std::endl;
    }

    std::cout << "Couldn't factor n..." << std::endl;
    exit(1);
}

void print_fac(factorization& fac){
    if (fac.size() == 2 && fac[0].second == 1 && fac[1].second == 1){
        // simple RSA case
        printf("[+] p = %s\n", mpz_get_str(0, 10, fac[0].first));
        printf("[+] q = %s\n", mpz_get_str(0, 10, fac[1].first));

        return;
    }

    if (fac.size() == 3 && fac[0].second == 1 && fac[1].second == 1 && fac[2].second == 1){
        // multi-prime RSA case
        printf("[+] p = %s\n", mpz_get_str(0, 10, fac[0].first));
        printf("[+] q = %s\n", mpz_get_str(0, 10, fac[1].first));
        printf("[+] r = %s\n", mpz_get_str(0, 10, fac[2].first));

        return;
    }
    printf("[+] N =");
    for (uint32_t i = 0; i < fac.size(); i++){
        if (!fac[i].second) continue;

        printf("%s", mpz_get_str(0, 10, fac[i].first));
        if (fac[i].second != 1) printf("^%d", fac[i].second);

        if (i != fac.size() - 1) printf(" * ");
    } 
}