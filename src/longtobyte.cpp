#include "longtobyte.h"

std::string long_to_bytes(mpz_ptr n){
    mpz_t np, tmp;
    mpz_init_set(np, n);
    mpz_init(tmp);

    std::string ret{};

    while (mpz_cmp_ui(np, 0)){
        char c = mpz_tdiv_qr_ui(np, tmp, np, 256);
        ret += c;
    }
    mpz_clears(np, tmp, 0);
    return std::string(ret.rbegin(), ret.rend());
}