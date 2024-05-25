#include "lowdensity.h"

static int MAX_KEEP = 2000;

// stolen on stackoverflow ...
static constexpr std::size_t pi_size_t() {
    if (sizeof(std::size_t) == 4) {
        return 0xc90fdaa2; // floor(pi/4 * 2^32)
    } else if (sizeof(std::size_t) == 8) {
        return 0xc90fdaa22168c234; // floor(pi/4 * 2^64)
    } else {
        throw std::logic_error(
            "this sizeof(size_t) is not supported. only 32 or 64 bits are supported.");
    }
}

static inline std::size_t scramble(std::size_t v) {
    return v ^ (pi_size_t() + (v << 6) + (v >> 2));
}

template<>
std::size_t std::hash<mpz_srcptr>::operator()(const mpz_srcptr x) const {
    std::string_view view {reinterpret_cast<char*>(x->_mp_d), abs(x->_mp_size)
            * sizeof(mp_limb_t) };
    size_t result = hash<std::string_view>{}(view);

    // produce different hashes for negative x
    if (x->_mp_size < 0) 
        result = scramble(result);
    
    return result;
}

template<>
struct std::hash<zz_pair>
{
    std::size_t operator()(const zz_pair& s) const noexcept{
        return std::hash<mpz_srcptr>{}(static_cast<mpz_srcptr>(s.first)) ^ 
            std::hash<mpz_srcptr>{}(static_cast<mpz_srcptr>(s.second));
    }
};

void iter_k_mod(mpz_srcptr n, prio_q& queue, int k){
    std::unordered_set<zz_pair> seen{};

    prio_q new_q;

    mpz_t n_mod_k, p2, q2, n1, n2, n3, n4;
    mpz_inits(n_mod_k, p2, q2, n1, n2, n3, n4, 0);
    mpz_tdiv_r_2exp(n_mod_k, n, k);

    auto it = &queue.top();
    for (int i = 0; i < std::min((int)queue.size(), MAX_KEEP); i++){
        mpz_ui_pow_ui(p2, 2, k);
        mpz_set(q2, p2);
        mpz_add(p2, p2, it[i].p);
        mpz_add(q2, q2, it[i].q);

        mpz_mul(n1, it[i].p, it[i].q);
        mpz_fdiv_r_2exp(n1, n1, k);

        mpz_mul(n2, p2, it[i].q);
        mpz_fdiv_r_2exp(n2, n2, k);

        mpz_mul(n3, it[i].p, q2);
        mpz_fdiv_r_2exp(n1, n1, k);

        mpz_mul(n4, p2, q2);
        mpz_fdiv_r_2exp(n1, n1, k);

        auto pa = std::make_pair((mpz_srcptr)it[i].p, (mpz_srcptr)it[i].q);
        if (!mpz_cmp(n1, n_mod_k) && !seen.count(pa)){
            new_q.emplace(dfs_node(it[i].weight, it[i].p, it[i].q));
            auto pa2 = std::make_pair((mpz_srcptr)it[i].q, (mpz_srcptr)it[i].p);
            seen.insert(pa);
            seen.insert(pa2);
        }

        pa = std::make_pair((mpz_srcptr)it[i].p, (mpz_srcptr)q2);
        if (!mpz_cmp(n1, n_mod_k) && !seen.count(pa)){
            new_q.emplace(dfs_node(it[i].weight + 1, it[i].p, q2));
            auto pa2 = std::make_pair((mpz_srcptr)q2, (mpz_srcptr)it[i].p);
            seen.insert(pa);
            seen.insert(pa2);
        }

        pa = std::make_pair((mpz_srcptr)p2, (mpz_srcptr)it[i].q);
        if (!mpz_cmp(n1, n_mod_k) && !seen.count(pa)){
            new_q.emplace(dfs_node(it[i].weight + 1, p2, it[i].q));
            auto pa2 = std::make_pair((mpz_srcptr)it[i].q, (mpz_srcptr)p2);
            seen.insert(pa);
            seen.insert(pa2);
        }

        pa = std::make_pair((mpz_srcptr)p2, (mpz_srcptr)q2);
        if (!mpz_cmp(n1, n_mod_k) && !seen.count(pa)){
            new_q.emplace(dfs_node(it[i].weight + 2, p2, q2));
            auto pa2 = std::make_pair((mpz_srcptr)q2, (mpz_srcptr)p2);
            seen.insert(pa);
            seen.insert(pa2);
        }
    }
    mpz_clears(n_mod_k, p2, q2, n1, n2, n3, n4, 0);
    queue = new_q;
}

void factor_low_density(mpz_ptr p, mpz_srcptr n){
    prio_q Q;
    mpz_t one;
    mpz_init_set_ui(one, 1);
    mpz_set_ui(p, 0);

    Q.emplace(dfs_node(2, one, one));
    int iters = mpz_sizeinbase(n, 2) + 1;
    for (int i = 1; i < iters; i++)
        iter_k_mod(n, Q, i);
    
    auto it = &Q.top();
    for (uint i = 0; i < Q.size(); i++){
        //printf("p : %s\nq : %s\n", mpz_get_str(0, 2, it[i].p), mpz_get_str(0, 2, it[i].q));
        if(mpz_divisible_p(n, it[i].p)){
            mpz_set(p, it[i].p);
            return;
        }
        if(mpz_divisible_p(n, it[i].q)){
            mpz_set(p, it[i].q);
            return;
        }
    }
    
}