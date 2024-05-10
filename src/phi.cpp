#include "phi.h"

void mpz_phi(mpz_ptr ret, factorization& fac){
    mpz_t tmp;
    mpz_set_ui(ret, 1);
    mpz_init(tmp);

    for(auto&& p : fac){
        mpz_pow_ui(tmp, p.first, p.second - 1);
        mpz_mul(ret, ret, tmp);

        mpz_sub_ui(tmp, p.first, 1);
        mpz_mul(ret, ret, tmp);
    }
    mpz_clear(tmp);
}