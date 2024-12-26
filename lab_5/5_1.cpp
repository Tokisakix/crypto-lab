#include <string>
#include <stdint.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

// Large-Number-Operation impl
// copyright @Smallorange666
// https://github.com/Smallorange666/Large-Number-Operation/tree/main
// Begin
namespace LNO {
    const int W = 64;
    const int MAXLEN = 64;
    const __uint128_t BASE = (static_cast < __uint128_t > (1) << 64);
    const uint64_t MASK = 0xffffffffffffffff;

    string p;
    uint64_t P[MAXLEN];

    uint64_t R[MAXLEN] = {
        0
    };
    int P_bits = 0;
    int P_words = 0;
    int R_bits = 0;
    int R_words = 0;
    uint64_t R2[MAXLEN] = {
        0
    };
    uint64_t P_[MAXLEN] = {
        0
    };
    uint64_t ZERO[MAXLEN] = {
        0
    };
    uint64_t ONE[MAXLEN] = {
        1
    };
    uint64_t TWO[MAXLEN] = {
        2
    };
    uint64_t POW2[64] = {
        1,
        2,
        4,
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
        4096,
        8192,
        16384,
        32768,
        65536,
        131072,
        262144,
        524288,
        1048576,
        2097152,
        4194304,
        8388608,
        16777216,
        33554432,
        67108864,
        134217728,
        268435456,
        536870912,
        1073741824,
        2147483648,
        4294967296,
        8589934592,
        17179869184,
        34359738368,
        68719476736,
        137438953472,
        274877906944,
        549755813888,
        1099511627776,
        2199023255552,
        4398046511104,
        8796093022208,
        17592186044416,
        35184372088832,
        70368744177664,
        140737488355328,
        281474976710656,
        562949953421312,
        1125899906842624,
        2251799813685248,
        4503599627370496,
        9007199254740992,
        18014398509481984,
        36028797018963968,
        72057594037927936,
        144115188075855872,
        288230376151711744,
        576460752303423488,
        1152921504606846976,
        2305843009213693952,
        4611686018427387904,
        9223372036854775808ULL
    };

    uint32_t K[64] = {
        0x428a2f98,
        0x71374491,
        0xb5c0fbcf,
        0xe9b5dba5,
        0x3956c25b,
        0x59f111f1,
        0x923f82a4,
        0xab1c5ed5,
        0xd807aa98,
        0x12835b01,
        0x243185be,
        0x550c7dc3,
        0x72be5d74,
        0x80deb1fe,
        0x9bdc06a7,
        0xc19bf174,
        0xe49b69c1,
        0xefbe4786,
        0x0fc19dc6,
        0x240ca1cc,
        0x2de92c6f,
        0x4a7484aa,
        0x5cb0a9dc,
        0x76f988da,
        0x983e5152,
        0xa831c66d,
        0xb00327c8,
        0xbf597fc7,
        0xc6e00bf3,
        0xd5a79147,
        0x06ca6351,
        0x14292967,
        0x27b70a85,
        0x2e1b2138,
        0x4d2c6dfc,
        0x53380d13,
        0x650a7354,
        0x766a0abb,
        0x81c2c92e,
        0x92722c85,
        0xa2bfe8a1,
        0xa81a664b,
        0xc24b8b70,
        0xc76c51a3,
        0xd192e819,
        0xd6990624,
        0xf40e3585,
        0x106aa070,
        0x19a4c116,
        0x1e376c08,
        0x2748774c,
        0x34b0bcb5,
        0x391c0cb3,
        0x4ed8aa4a,
        0x5b9cca4f,
        0x682e6ff3,
        0x748f82ee,
        0x78a5636f,
        0x84c87814,
        0x8cc70208,
        0x90befffa,
        0xa4506ceb,
        0xbef9a3f7,
        0xc67178f2
    };

    int getBits(const uint64_t a[MAXLEN]) {
        for (int i = MAXLEN - 1; i >= 0; i--) {
            if (a[i])
                for (int j = W - 1; j >= 0; j--)
                    if (a[i] & POW2[j])
                        return i * W + j;
        }

        return 0;
    }

    bool bigger(uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        for (int i = MAXLEN - 1; i >= 0; i--) {
            if (a[i] > b[i])
                return true;
            else if (a[i] < b[i])
                return false;
        }
        return false;
    }

    bool equal(uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        for (int i = 0; i < MAXLEN; i++)
            if (a[i] != b[i])
                return false;
        return true;
    }

    bool sub_overflow(uint64_t a, uint64_t b, uint64_t & result) {
        if (b > a) {
            return true; // Overflow occurred
        }
        result = a - b;
        return false; // No overflow
    }

    void sub(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        uint64_t borrow = 0;
        uint64_t temp[MAXLEN] = {
            0
        };
        for (int i = 0; i < MAXLEN; i++) {
            uint64_t gap;
            if (sub_overflow(a[i], borrow, gap) || sub_overflow(gap, b[i], gap)) {
                temp[i] = (BASE + a[i] - borrow - b[i]) & MASK;
                borrow = 1;
            } else {
                temp[i] = gap;
                borrow = 0;
            }
        }

        for (int i = 0; i < MAXLEN; i++)
            res[i] = temp[i];
    }

    void mul(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        uint64_t temp[2 * MAXLEN] = {
            0
        };

        for (int i = 0; i < MAXLEN; i++) {
            uint64_t carry = 0;
            for (int j = 0; j < MAXLEN; j++) {
                __uint128_t sum = (__uint128_t) a[i] * b[j] + temp[i + j] + carry;
                temp[i + j] = sum & MASK;
                carry = sum >> W;
            }
            if (carry)
                temp[i + MAXLEN] += carry;
        }

        for (int i = 0; i < MAXLEN; i++)
            res[i] = temp[i];
    }

    void div(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        int n = getBits(b) / W;
        int m = getBits(a) / W - n; // iterate times

        uint64_t d = BASE / ((__uint128_t) b[n] + 1);
        uint64_t u_[MAXLEN + 1] = {
            0
        }, v_[MAXLEN] = {
            0
        };
        uint64_t carry = 0;
        // scale a
        for (int i = 0; i < MAXLEN; i++) {
            __uint128_t temp = (__uint128_t) a[i] * d + carry;
            u_[i] = temp & MASK;
            carry = temp >> W;
        }
        if (carry)
            u_[MAXLEN] = carry;

        // scale b
        for (int i = 0; i < MAXLEN; i++) {
            __uint128_t temp = (__uint128_t) b[i] * d + carry;
            v_[i] = temp & MASK;
            carry = temp >> W;
        }

        int j = m;
        while (j >= 0) {
            uint64_t tem[MAXLEN + 1] = {
                0
            };
            for (int i = 0; i <= n + 1; i++)
                tem[i] = u_[i + j];

            __uint128_t q_hat = (tem[n + 1] * BASE + tem[n]) / v_[n];
            if (q_hat > BASE - 1)
                q_hat = BASE - 1;

            uint64_t qv[MAXLEN] = {
                0
            };
            carry = 0;
            for (int i = 0; i < MAXLEN; i++) {
                __uint128_t temp = q_hat * v_[i] + carry;
                qv[i] = temp & MASK;
                carry = temp >> W;
            }
            while (bigger(qv, tem)) {
                q_hat--;
                sub(qv, qv, v_);
            }

            sub(tem, tem, qv);
            for (int i = 0; i <= n + 1; i++)
                u_[i + j] = tem[i];

            res[j] = q_hat;

            j--;
        }
    }

    void mod(uint64_t res[MAXLEN], uint64_t a[MAXLEN]) {
        uint64_t temp[MAXLEN] = {
            0
        };
        div(temp, a, P);
        mul(temp, temp, P);
        sub(res, a, temp);
    }


    void mod_R(uint64_t res[MAXLEN], uint64_t a[MAXLEN]) {
        uint64_t temp[MAXLEN] = {
            0
        };
        for (int i = 0; i < R_words; i++)
            temp[i] = a[i];
        for (int i = 0; i < MAXLEN; i++)
            res[i] = temp[i];
    }

    void div_R(uint64_t res[MAXLEN], uint64_t a[MAXLEN + 1]) {
        uint64_t temp[MAXLEN] = {
            0
        };
        for (int i = 0; i + R_words < MAXLEN + 1; i++)
            temp[i] = a[i + R_words];
        for (int i = 0; i < MAXLEN; i++)
            res[i] = temp[i];
    }


    void exculid(uint64_t a[MAXLEN], uint64_t b[MAXLEN], uint64_t x[MAXLEN], uint64_t y[MAXLEN]) {
        if (equal(b, ZERO)) {
            x[0] = 1;
            y[0] = 0;
            return;
        }

        uint64_t temp[MAXLEN] = {
            0
        };
        div(temp, a, b);
        mul(temp, b, temp);
        sub(temp, a, temp);

        exculid(b, temp, y, x);

        uint64_t temp2[MAXLEN] = {
            0
        };
        div(temp2, a, b);
        mul(temp2, temp2, x);

        if (bigger(y, temp2)) {
            sub(temp2, y, temp2);
            mod_R(temp2, temp2);
        } else {
            sub(temp2, temp2, y);
            mod_R(temp2, temp2);
            sub(temp2, R, temp2);
        }

        for (int i = 0; i < MAXLEN; i++)
            y[i] = temp2[i];
    }

    void pre_cal() {
        P_words = P_bits / W;
        R_words = P_words + 1;
        R[R_words] = 1;
        R_bits = getBits(R);

        mod(R2, R);
        mul(R2, R2, R2);
        mod(R2, R2);

        uint64_t negP[MAXLEN] = {
            0
        };
        sub(negP, R, P);

        uint64_t x[MAXLEN] = {
                0
            },
            y[MAXLEN] = {
                0
            };
        exculid(negP, R, x, y);
        for (int i = 0; i < MAXLEN; i++)
            P_[i] = x[i];
    }
    
    uint32_t func_ch(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (~x & z);
    }

    uint32_t func_maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    uint32_t func_sum_0(uint32_t x) {
        return ((x << 30) | (x >> 2)) ^ ((x << 19) | (x >> 13)) ^ ((x << 10) | (x >> 22));
    }

    uint32_t func_sum_1(uint32_t x) {
        return ((x << 26) | (x >> 6)) ^ ((x << 21) | (x >> 11)) ^ ((x << 7) | (x >> 25));
    }

    uint32_t func_xigama_0(uint32_t x) {
        return ((x << 25) | (x >> 7)) ^ ((x << 14) | (x >> 18)) ^ (x >> 3);
    }

    uint32_t func_xigama_1(uint32_t x) {
        return ((x << 15) | (x >> 17)) ^ ((x << 13) | (x >> 19)) ^ (x >> 10);
    }

    void func_8to32(uint32_t M[16], uint8_t a[64]) {
        for (size_t i = 0; i < 16; ++i) {
            M[i] = (static_cast < uint32_t > (a[i * 4]) << 24) |
                (static_cast < uint32_t > (a[i * 4 + 1]) << 16) |
                (static_cast < uint32_t > (a[i * 4 + 2]) << 8) |
                (static_cast < uint32_t > (a[i * 4 + 3]));
        }
        return;
    }

    void func_32to8(uint32_t H[8], uint8_t h[32]) {
        for (int i = 0; i < 8; i++) {
            h[i * 4] = (H[i] >> 24) & 0xFF;
            h[i * 4 + 1] = (H[i] >> 16) & 0xFF;
            h[i * 4 + 2] = (H[i] >> 8) & 0xFF;
            h[i * 4 + 3] = H[i] & 0xFF;
        }
        return;
    }

    void REDC(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        uint64_t T[MAXLEN] = {
            0
        };
        mul(T, a, b);

        int s = P_words + 1;
        uint64_t T_[MAXLEN + 1] = {
            0
        };
        for (int i = 0; i < MAXLEN; i++)
            T_[i] = T[i];

        for (int i = 0; i < s; i++) {
            uint64_t carry = 0;
            __uint128_t mi = ((__uint128_t) T_[i] * P_[0]) & MASK;
            for (int j = 0; j < s; j++) {
                __uint128_t temp = mi * P[j] + T_[i + j] + carry;
                T_[i + j] = temp & MASK;
                carry = temp >> W;
            }

            int count = 0;
            while (carry) {
                __uint128_t temp = (__uint128_t) T_[i + s + count] + carry;
                T_[i + s + count] = temp & MASK;
                carry = temp >> W;
                count++;
            }
        }

        div_R(res, T_);

        if (bigger(res, P) || equal(res, P))
            sub(res, res, P);
    }

    void mod_mul_mont(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        uint64_t x_[MAXLEN] = {
            0
        }, y_[MAXLEN] = {
            0
        };

        REDC(x_, a, R2);
        REDC(y_, b, R2);
        REDC(x_, x_, y_);
        REDC(x_, x_, ONE);

        for (int i = 0; i < MAXLEN; i++)
            res[i] = x_[i];
    }

    void mod_pow_mont(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {

        uint64_t temp[MAXLEN] = {
            0
        };
        uint64_t base[MAXLEN] = {
            0
        };
        REDC(temp, ONE, R2);
        REDC(base, a, R2);

        for (int i = 0; i < MAXLEN; i++) {
            if (bigger(temp, ONE) && b[i] == 0)
                break;
            for (int j = 0; j < W; j++) {
                if (b[i] & POW2[j])
                    REDC(temp, temp, base);

                REDC(base, base, base);
            }
        }

        REDC(res, temp, ONE);
    }
}
// End
// Large-Number-Operation impl

void func_extend(uint32_t W[64], uint32_t M[16]) {
    memcpy(W, M, 16 * sizeof(uint32_t));
    for (int i = 16; i < 64; i++) {
        W[i] = LNO::func_xigama_1(W[i - 2]) + W[i - 7] + LNO::func_xigama_0(W[i - 15]) + W[i - 16];
    }
    return;
}

void func_hash(uint32_t M[16], uint32_t H[8]) {
    uint32_t a = H[0];
    uint32_t b = H[1];
    uint32_t c = H[2];
    uint32_t d = H[3];
    uint32_t e = H[4];
    uint32_t f = H[5];
    uint32_t g = H[6];
    uint32_t h = H[7];
    uint32_t W[64];
    func_extend(W, M);
    for (int i = 0; i < 64; i++) {
        uint32_t t1 = h + LNO::func_sum_1(e) + LNO::func_ch(e, f, g) + LNO::K[i] + W[i];
        uint32_t t2 = LNO::func_sum_0(a) + LNO::func_maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
    return;
}

void func_hash_1(uint8_t m[36], uint8_t h[32]) {
    uint8_t state[64];
    for (int i = 0; i < 36; i++) {
        state[i] = m[i];
    }
    state[36] = 0x80;
    for (int i = 37; i < 62; i++) {
        state[i] = 0x00;
    }
    state[62] = 0x01;
    state[63] = 0x20;

    uint32_t M[16];
    LNO::func_8to32(M, state);

    uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };
    func_hash(M, H);

    LNO::func_32to8(H, h);
    return;
}

void func_hash2(uint8_t m[227], uint8_t h[32]) {
    uint8_t state[64];
    uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };
    uint32_t M[16];
    for (int i = 0; i < 64; i++) {
        state[i] = m[i];
    }
    LNO::func_8to32(M, state);
    func_hash(M, H);
    for (int i = 64; i < 128; i++) {
        state[i - 64] = m[i];
    }
    LNO::func_8to32(M, state);
    func_hash(M, H);
    for (int i = 128; i < 192; i++) {
        state[i - 128] = m[i];
    }
    LNO::func_8to32(M, state);
    func_hash(M, H);
    for (int i = 192; i < 227; i++) {
        state[i - 192] = m[i];
    }
    state[35] = 0x80;
    for (int i = 36; i < 62; i++) {
        state[i] = 0x00;
    }
    state[62] = 0x07;
    state[63] = 0x18;
    LNO::func_8to32(M, state);
    func_hash(M, H);
    LNO::func_32to8(H, h);
    return;
}

void func_mgf1(uint8_t m[32], uint8_t result[223]) {
    uint8_t h[32];
    for (int i = 0; i < 7; i++) {
        uint8_t M[36];
        for (int j = 0; j < 32; j++) {
            M[j] = m[j];
        }
        M[32] = 0x00;
        M[33] = 0x00;
        M[34] = 0x00;
        M[35] = static_cast < uint8_t > (i);
        func_hash_1(M, h);
        for (int j = 0; j < 32; j++) {
            if (32 * i + j < 223) result[32 * i + j] = h[j];
        }
    }
    return;
}

void func_mgf2(uint8_t m[223], uint8_t result[32]) {
    uint8_t M[227];
    for (int i = 0; i < 223; i++) {
        M[i] = m[i];
    }
    M[223] = 0x00;
    M[224] = 0x00;
    M[225] = 0x00;
    M[226] = 0x00;
    func_hash2(M, result);
    return;
}

void func_fill(uint8_t * m, uint8_t len, uint8_t em[256]) {
    uint8_t seed[32] = {
        0x5F,
        0x7F,
        0x00,
        0x3F,
        0x9F,
        0x1C,
        0x95,
        0xF2,
        0x90,
        0x33,
        0x53,
        0x90,
        0x3F,
        0xAC,
        0xC8,
        0x18,
        0xD2,
        0xF8,
        0x52,
        0x97,
        0xDB,
        0x4B,
        0xB6,
        0xD1,
        0xB8,
        0xAA,
        0x27,
        0x79,
        0x42,
        0xC0,
        0x78,
        0x59
    };

    uint8_t L[32] = {
        0xE3,
        0xB0,
        0xC4,
        0x42,
        0x98,
        0xFC,
        0x1C,
        0x14,
        0x9A,
        0xFB,
        0xF4,
        0xC8,
        0x99,
        0x6F,
        0xB9,
        0x24,
        0x27,
        0xAE,
        0x41,
        0xE4,
        0x64,
        0x9B,
        0x93,
        0x4C,
        0xA4,
        0x95,
        0x99,
        0x1B,
        0x78,
        0x52,
        0xB8,
        0x55
    };
    uint8_t DB[223];
    for (int i = 0; i < 32; i++) {
        DB[i] = L[i];
    }
    for (int i = 32; i < 223 - len - 1; i++) {
        DB[i] = 0x00;
    }
    DB[222 - len] = 0x01;
    for (int i = 0; i < len; i++) {
        DB[223 - len + i] = m[i];
    }

    uint8_t MaskedDB[223];
    func_mgf1(seed, MaskedDB);
    for (int i = 0; i < 223; i++) {
        MaskedDB[i] ^= DB[i];
    }

    uint8_t MaskedSeed[32];
    func_mgf2(MaskedDB, MaskedSeed);
    for (int i = 0; i < 32; i++) {
        MaskedSeed[i] ^= seed[i];
    }

    em[0] = 0x00;
    for (int i = 1; i < 33; i++) {
        em[i] = MaskedSeed[i - 1];
    }
    for (int i = 33; i < 256; i++) {
        em[i] = MaskedDB[i - 33];
    }
    return;
}

void func_tran(uint8_t a[256], uint64_t b[32]) {
    for (size_t i = 0; i < 32; ++i) {
        b[i] = (static_cast < uint64_t > (a[i * 8]) << 56) |
            (static_cast < uint64_t > (a[i * 8 + 1]) << 48) |
            (static_cast < uint64_t > (a[i * 8 + 2]) << 40) |
            (static_cast < uint64_t > (a[i * 8 + 3]) << 32) |
            (static_cast < uint64_t > (a[i * 8 + 4]) << 24) |
            (static_cast < uint64_t > (a[i * 8 + 5]) << 16) |
            (static_cast < uint64_t > (a[i * 8 + 6]) << 8) |
            (static_cast < uint64_t > (a[i * 8 + 7]));
    }
    return;
}

void func_little(uint64_t a[32]) {
    for (size_t i = 0; i < 32; ++i) {
        uint64_t value = a[i];
        a[i] = ((value & 0xFF00000000000000) >> 56) |
            ((value & 0x00FF000000000000) >> 40) |
            ((value & 0x0000FF0000000000) >> 24) |
            ((value & 0x000000FF00000000) >> 8) |
            ((value & 0x00000000FF000000) << 8) |
            ((value & 0x0000000000FF0000) << 24) |
            ((value & 0x000000000000FF00) << 40) |
            ((value & 0x00000000000000FF) << 56);
    }
    uint64_t temp[32];
    memcpy(temp, a, 32 * sizeof(uint64_t));
    for (int i = 0; i < 32; i++) {
        a[i] = temp[31 - i];
    }
    return;
}

void func_little_em(uint64_t a[32]) {
    uint64_t temp[32];
    memcpy(temp, a, 32 * sizeof(uint64_t));
    for (int i = 0; i < 32; i++) {
        a[i] = temp[31 - i];
    }
    return;
}

void func_kuo(uint64_t a[32], uint64_t b[64]) {
    memcpy(b, a, 32 * sizeof(uint64_t));
    for (int i = 32; i < 64; i++) {
        b[i] = 0;
    }
    return;
}

uint8_t ignore_1[16];
uint64_t n[32];
uint64_t e[32];
uint8_t ignore_2[256];
uint8_t message_length;
uint8_t encoded_message[256];
uint64_t encoded_message_blocks[32];
uint64_t modulus_blocks[64];
uint64_t public_exponent_blocks[64];
uint64_t message_blocks[64];
uint64_t cipher_blocks[64];

void func_run(uint8_t* message, uint8_t length) {
    func_fill(message, length, encoded_message);

    func_little(n);
    func_little(e);
    func_tran(encoded_message, encoded_message_blocks);
    func_little_em(encoded_message_blocks);

    func_kuo(n, modulus_blocks);
    func_kuo(e, public_exponent_blocks);
    func_kuo(encoded_message_blocks, message_blocks);

    memcpy(LNO::P, modulus_blocks, 64 * sizeof(uint64_t));
    LNO::P_bits = LNO::getBits(LNO::P);

    LNO::pre_cal();

    LNO::mod_pow_mont(cipher_blocks, message_blocks, public_exponent_blocks);

    func_little(cipher_blocks);
    return;
}

int main(void) {
    fread(ignore_1, sizeof(uint8_t), 16, stdin);
    fread(n, sizeof(uint64_t), 32, stdin);
    fread(e, sizeof(uint64_t), 32, stdin);
    fread(ignore_2, sizeof(uint8_t), 256, stdin);
    fread(&message_length, sizeof(uint8_t), 1, stdin);
    uint8_t message[message_length];
    fread(message, sizeof(uint8_t), message_length, stdin);

    func_run(message, message_length);

    fwrite(cipher_blocks, sizeof(uint64_t), 32, stdout);

    return 0;
}