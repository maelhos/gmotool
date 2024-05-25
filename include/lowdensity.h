#pragma once
#include "stdinc.h"
#include <queue>
#include <unordered_set>

struct dfs_node {
    int weight;
    mpz_t p;
    mpz_t q;

    dfs_node(int weight, mpz_srcptr ps, mpz_srcptr qs) : weight(weight) {
        mpz_init_set(p, ps);
        mpz_init_set(q, qs);
    }
    ~dfs_node(){
        //mpz_clears(p, q, 0);
    }
};

using zz_pair = std::pair<mpz_srcptr, mpz_srcptr>;
using prio_q = std::priority_queue<dfs_node, std::vector<dfs_node>, 
        decltype([](dfs_node& a, dfs_node& b) {
            return a.weight > b.weight;
        })>;

void iter_k_mod(mpz_srcptr n, prio_q& queue, int k);
void factor_low_density(mpz_ptr p, mpz_srcptr n);
