#include "stdinc.h"
#include "factor.h"
#include "phi.h"
#include "longtobyte.h"

void mpz_set_str_ext(mpz_ptr n, const char* str){
    if (str[0] == '0' && str[1] == 'x')
        mpz_init_set_str(n, str + 2, 16);
    else if (str[0] == '0' && str[1] == 'b')
        mpz_init_set_str(n, str + 2, 2);
    else 
        mpz_init_set_str(n, str, 10);
}

int main(int argc, char* argv[]){
    std::string N, E, C;

    std::cout << "[+] GMO Tool v1.0" << std::endl;
    std::cout << "[?] N : ";
    std::cin >> N;
    std::cout << "[?] e: ";
    std::cin >> E;
    std::cout << "[?] c: ";
    std::cin >> C;

    mpz_t n, e, c;
    mpz_set_str_ext(n, N.c_str());
    mpz_set_str_ext(e, E.c_str());
    mpz_set_str_ext(c, C.c_str());

    // factor N
    factorization fac = factor(n);
    print_fac(fac);

    // compute stuff and print flag
    mpz_t phi, d, m;
    mpz_inits(phi, d, m, 0);
    mpz_phi(phi, fac);
    if (!mpz_invert(d, e, phi)){
        std::cout << "e not invertible mod phi..." << std::endl;
        // TODO: recovery for the different cases
        exit(1);
    }
    mpz_powm(m, c, d, n);
    printf("[+] m = %s\n", mpz_get_str(0, 10, m));
    std::cout << "[!] Flag : " << long_to_bytes(m) << std::endl;

    return 0;
}