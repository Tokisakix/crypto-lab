#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/types.h>

using namespace std;

// Bigint impl
namespace Bigint {
    void initField(
        uint64_t * p,
        uint64_t * p_2,
        uint64_t * R2modp,
        uint64_t & g,
        int & N
    ) {
        g = p[0];
        for (int i = 0; i < 5; ++i) {
            g = g * (2 - p[0] * g);
        }
        g = -g;
        memcpy(p_2, p, 256);
        uint64_t borrow = 2, diff;
        for (int i = 0; i < 32; ++i) {
            diff = p_2[i] - borrow;
            if (p_2[i] < borrow) {
                borrow = 1;
            } else {
                borrow = 0;
            }
            p_2[i] = diff;
        }
        uint64_t buffer[33] = {
            0
        };
        buffer[0] = 1;
        for (int i = 0; i < (N << 7); ++i) {
            for (int j = 32; j > 0; --j) {
                // tmd Ofast 导致奇怪的问题
                fread(buffer, sizeof(uint8_t), 1, stdout);
                buffer[j] = (buffer[j] << 1) | (buffer[j - 1] >> 63);
            }
            buffer[0] = buffer[0] << 1;
            bool flag = true;
            for (int j = 31; j >= 0; --j) {
                if (buffer[j] > p[j]) {
                    break;
                } else if (buffer[j] < p[j]) {
                    flag = false;
                    break;
                }
            }
            if (buffer[32] != 0) {
                flag = true;
            }
            if (flag) {
                uint64_t borrow = 0, diff;
                for (int j = 0; j < 32; ++j) {
                    diff = buffer[j] - p[j] - borrow;
                    if (buffer[j] < p[j] || buffer[j] < borrow) {
                        borrow = 1;
                    } else {
                        borrow = 0;
                    }
                    buffer[j] = diff;
                }
                if (borrow)
                    buffer[32] = 0;
            }
        }
        memcpy(R2modp, buffer, 256);
    }

    class Field {
        public: uint64_t p_arr[32] = {
            0
        };
        uint64_t p_2_arr[32];
        uint64_t R2modp_arr[32] = {
            0
        };
        uint64_t * p = p_arr; // p
        uint64_t * p_2 = p_2_arr; // p-2
        uint64_t * R2modp = R2modp_arr; // R^2 mod p, here R = 2^64
        uint64_t g = 0; // ( - p[0] ^ (-1) ) mod 2^64
        int N = 2; // how many limbs p really uses

        Field() {}

        Field(uint64_t * p_) {
            memcpy(p, p_, sizeof(uint64_t) * 32);
            initField(p, p_2, R2modp, g, N);
        }

        friend std::istream & operator >> (std::istream & is, Field & f) {
            char * b = (char * ) f.p;
            for (int i = 0; i < f.N; ++i) {
                fread(b + i * 8, sizeof(uint8_t), 8, stdin);
            }
            // is.read(...);  // tmd Ofast 导致奇怪的问题
            initField(f.p, f.p_2, f.R2modp, f.g, f.N);
            return is;
        }

        friend std::ostream & operator << (std::ostream & os,
            const Field & f) {
            char * b = (char * ) f.p;
            for (int i = 0; i < f.N; ++i) {
                fwrite(b + i * 8, sizeof(uint8_t), 8, stdout);
            }
            return os;
        }

        class Int {
            public: Field & F;
            uint64_t a_arr[32] = {
                0
            };
            uint64_t * a = a_arr;

            Int(Field & F_ref): F(F_ref) {}

            Int(const Int & other): F(other.F) {
                memcpy(a, other.a, sizeof(uint64_t) * 32);
            }

            Int(Field & F_ref, uint64_t * a_): F(F_ref) {
                memcpy(a, a_, sizeof(uint64_t) * 32);
            }

            friend std::istream & operator >> (std::istream & is, Int & a) {
                char * b = (char * ) a.a;
                for (int i = 0; i < a.F.N; ++i) {
                    fread(b + i * 8, sizeof(uint8_t), 8, stdin);
                }
                return is;
            }

            friend std::ostream & operator << (std::ostream & os,
                const Int & a) {
                char * b = (char * ) a.a;
                for (int i = 0; i < a.F.N; ++i) {
                    fwrite(b + i * 8, sizeof(uint8_t), 8, stdout);
                }
                return os;
            }

            inline bool operator == (const Int & other) const {
                for (int i = 0; i < 32; ++i) {
                    if (other.a[i] != a[i])
                        return false;
                }
                return true;
            }

            inline bool operator != (const Int & other) const {
                return !operator == (other);
            }

            Int & operator = (const Int & other) {
                if (this != & other) {
                    // F = other.F;  // not needed if it will only be the same field
                    memcpy(a, other.a, 256);
                }
                return * this;
            }

            inline bool operator >= (const Int & other) const {
                for (int i = 31; i >= 0; --i) {
                    if (a[i] < other.a[i])
                        return false;
                    else if (a[i] > other.a[i])
                        return true;
                }
                return true;
            }

            inline bool operator <= (const Int & other) const {
                for (int i = 31; i >= 0; --i) {
                    if (a[i] > other.a[i])
                        return false;
                    else if (a[i] < other.a[i])
                        return true;
                }
                return true;
            }

            inline bool operator > (const Int & other) const {
                return !operator <= (other);
            }

            inline bool operator < (const Int & other) const {
                return !operator >= (other);
            }

            Int MontgomeryMultiply(const uint64_t * b) const {
                auto add2z = [](uint64_t * z, uint64_t & b, int i) {
                    uint64_t carry = b, sum;
                    for (; i < 3; ++i) {
                        sum = z[i] + carry;
                        if (sum < carry || sum < z[i]) {
                            carry = 1;
                        } else {
                            carry = 0;
                        }
                        z[i] = sum;
                    }
                };
                Int d(F);
                uint64_t d_h[2] = {
                    0
                };
                for (int i = 0; i < F.N; ++i) {
                    uint64_t f = (d.a[0] + a[i] * b[0]) * F.g;
                    uint64_t c[2] = {
                        0
                    };
                    for (int j = 0; j < F.N; ++j) {
                        uint64_t z[3] = {
                            c[0],
                            c[1],
                            0
                        };
                        __uint128_t ai = static_cast < __uint128_t > (a[i]);
                        __uint128_t bj = static_cast < __uint128_t > (b[j]);
                        __uint128_t aibj = ai * bj;
                        uint64_t aibj_l = static_cast < uint64_t > (aibj);
                        uint64_t aibj_h = static_cast < uint64_t > (aibj >> 64);
                        __uint128_t f_ = static_cast < __uint128_t > (f);
                        __uint128_t mj = static_cast < __uint128_t > (F.p[j]);
                        __uint128_t f_mj = f_ * mj;
                        uint64_t f_mj_l = static_cast < uint64_t > (f_mj);
                        uint64_t f_mj_h = static_cast < uint64_t > (f_mj >> 64);
                        add2z(z, aibj_l, 0);
                        add2z(z, aibj_h, 1);
                        add2z(z, f_mj_l, 0);
                        add2z(z, f_mj_h, 1);
                        add2z(z, d.a[j], 0);
                        if (j > 0) {
                            d.a[j - 1] = z[0];
                        }
                        c[0] = z[1];
                        c[1] = z[2];
                    }
                    uint64_t z[3] = {
                        d_h[0],
                        d_h[1],
                        0
                    };
                    add2z(z, c[0], 0);
                    add2z(z, c[1], 1);
                    d.a[F.N - 1] = z[0];
                    d_h[0] = z[1];
                    d_h[1] = z[2];
                }
                bool flag = true;
                for (int i = 31; i >= 0; --i) {
                    if (d.a[i] > F.p[i]) {
                        break;
                    } else if (d.a[i] < F.p[i]) {
                        flag = false;
                        break;
                    }
                }
                if (d_h[0] != 0)
                    flag = true;
                if (flag) {
                    uint64_t borrow = 0, diff;
                    for (int j = 0; j < F.N; ++j) {
                        diff = d.a[j] - F.p[j] - borrow;
                        if (d.a[j] < F.p[j] + borrow) {
                            borrow = 1;
                        } else {
                            borrow = 0;
                        }
                        d.a[j] = diff;
                    }
                }
                return d;
            }

            Int operator * (const Int & other) const {
                Int aR = this -> MontgomeryMultiply(F.R2modp);
                return aR.MontgomeryMultiply(other.a);
            }

            Int operator ^ (const Int & other) const {
                Int c(F), tmp(F, a);
                Int aR = this -> MontgomeryMultiply(F.R2modp);
                if ( * this == c)
                    return c;
                c.a[0] = 1;
                bool start = false;
                for (int i = 31; i >= 0; --i) {
                    for (int j = 63; j >= 0; --j) {
                        if (start) {
                            c = c * c;
                        }
                        if (other.a[i] & (1ULL << j)) {
                            if (!start) {
                                start = true;
                                c = * this;
                            } else {
                                c = c.MontgomeryMultiply(aR.a);
                            }
                        }
                    }
                }
                return c;
            }
        };
    };
}

// Lab 7.1 start here

using Bigint::Field;

Field f;
Field::Int G(f);
Field::Int S(f);
Field::Int P(f);
uint32_t len;
uint32_t num_1 = 0;
uint32_t num_0 = 0;

uint8_t run() {
    uint8_t res = 0;

    for (int j = 0; j < 8; ++j) {
        S = G ^ S;

        if (S >= P) {
            res |= (1 << j);
            num_1++;
        } else {
            num_0++;
        }
    }

    return res;
}

int main() {
    cin >> f >> G >> S;
    fread( & len, sizeof(uint32_t), 1, stdin);

    memcpy(P.a, f.p, sizeof(uint64_t) * 2);
    P.a[0] = (P.a[1] << 63) | (P.a[0] >> 1);
    P.a[1] = P.a[1] >> 1;

    uint8_t result[1024];
    for (uint32_t i = 0; i < len; ++i) {
        result[i] = run();
    }
    fwrite( & result, sizeof(uint8_t), len, stdout);

    fwrite( & num_0, sizeof(uint32_t), 1, stdout);
    fwrite( & num_1, sizeof(uint32_t), 1, stdout);

    return 0;
}