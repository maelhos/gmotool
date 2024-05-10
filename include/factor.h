#include "stdinc.h"

using fac_pair = std::pair<mpz_t, uint32_t>;
using factorization = std::vector<fac_pair>;

factorization factor(mpz_ptr n);