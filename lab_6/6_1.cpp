#include <string>
#include <stdint.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

// Large-Number-Operation impl
// copyright @Smallorange666
// https://github.com/Smallorange666/Large-Number-Operation/tree/main
// Begin
namespace Bigint {
    const int W = 64;
    const int MAXLEN = 4;
    const uint128_t BASE = (static_cast < uint128_t > (1) << 64);
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

    int getBits(const uint64_t a[MAXLEN]) {
        for (int i = MAXLEN - 1; i >= 0; i--) {
            if (a[i])
                for (int j = W - 1; j >= 0; j--)
                    if (a[i] & POW2[j])
                        return i * W + j;
        }

        return 0;
    }

    void remove_leading_zeros(string & s) {
        size_t end = s.find_last_not_of('0');
        if (end != string::npos)
            s.erase(end + 1);
        else
            s = "0";
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

    void str_div2(string & s) {
        int carry = 0;
        for (int i = s.length() - 1; i >= 0; i--) {
            int x = s[i] - '0';
            s[i] = (x + carry * 10) / 2 + '0';
            carry = x % 2;
        }

        remove_leading_zeros(s);
    }

    void str_mul2(string & s) {
        int carry = 0;
        for (std::string::size_type i = 0; i < s.length(); i++) {
            int x = s[i] - '0';
            s[i] = (x * 2 + carry) % 10 + '0';
            carry = (x * 2 + carry) / 10;
        }

        if (carry)
            s = s + "1";
    }

    void str_add1(string & s) {
        int carry = 1;
        for (std::string::size_type i = 0; i < s.length(); i++) {
            int x = s[i] - '0';
            s[i] = (x + carry) % 10 + '0';
            carry = (x + carry) / 10;
        }
        if (carry)
            s = s + "1";
    }

    void str2bi(uint64_t res[MAXLEN], string & s) {
        int count = 0;
        while (s != "0") {
            if ((s[0] - '0') & 1)
                res[count / W] += POW2[count % W];
            str_div2(s);
            count++;
        }
    }

    string bi2str(uint64_t res[MAXLEN]) {
        string s = "0";
        for (int i = MAXLEN - 1; i >= 0; i--)
            for (int j = W - 1; j >= 0; j--) {
                str_mul2(s);
                if (res[i] & POW2[j])
                    str_add1(s);
            }

        reverse(s.begin(), s.end());
        return s;
    }

    bool add_overflow(uint64_t a, uint64_t b, uint64_t & result) {
        result = a + b;
        // Check for overflow: if result is less than either operand, overflow occurred
        if (result < a || result < b) {
            return true; // Overflow occurred
        }
        return false; // No overflow
    }

    void add(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        uint64_t carry = 0;
        uint64_t temp[MAXLEN] = {
            0
        };
        for (int i = 0; i < MAXLEN; i++) {
            uint64_t sum;
            if (add_overflow(a[i], carry, sum) || add_overflow(sum, b[i], sum)) {
                temp[i] = ((uint128_t) a[i] + b[i] + carry) & MASK;
                carry = 1;
            } else {
                temp[i] = sum;
                carry = 0;
            }
        }

        for (int i = 0; i < MAXLEN; i++)
            res[i] = temp[i];
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
                uint128_t sum = (uint128_t) a[i] * b[j] + temp[i + j] + carry;
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

        uint64_t d = BASE / ((uint128_t) b[n] + 1);
        uint64_t u_[MAXLEN + 1] = {
            0
        }, v_[MAXLEN] = {
            0
        };
        uint64_t carry = 0;
        // scale a
        for (int i = 0; i < MAXLEN; i++) {
            uint128_t temp = (uint128_t) a[i] * d + carry;
            u_[i] = temp & MASK;
            carry = temp >> W;
        }
        if (carry)
            u_[MAXLEN] = carry;

        // scale b
        for (int i = 0; i < MAXLEN; i++) {
            uint128_t temp = (uint128_t) b[i] * d + carry;
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

            uint128_t q_hat = (tem[n + 1] * BASE + tem[n]) / v_[n];
            if (q_hat > BASE - 1)
                q_hat = BASE - 1;

            uint64_t qv[MAXLEN] = {
                0
            };
            carry = 0;
            for (int i = 0; i < MAXLEN; i++) {
                uint128_t temp = q_hat * v_[i] + carry;
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

    void mod_add(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        add(res, a, b);
        mod(res, res);
    }

    void mod_sub(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        if (bigger(a, b)) {
            sub(res, a, b);
            mod(res, res);
        } else {
            sub(res, b, a);
            mod(res, res);
            sub(res, P, res);
        }
    }

    void mod_div(uint64_t res[MAXLEN], uint64_t a[MAXLEN], uint64_t b[MAXLEN]) {
        div(res, a, b);
        mod(res, res);
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

    void inv_exculid(uint64_t a[MAXLEN], uint64_t b[MAXLEN], uint64_t x[MAXLEN], uint64_t y[MAXLEN]) {
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

        inv_exculid(b, temp, y, x);

        uint64_t temp2[MAXLEN] = {
            0
        };
        div(temp2, a, b);
        mul(temp2, temp2, x);

        if (bigger(y, temp2)) {
            sub(temp2, y, temp2);
            mod(temp2, temp2);
        } else {
            sub(temp2, temp2, y);
            mod(temp2, temp2);
            sub(temp2, P, temp2);
        }

        for (int i = 0; i < MAXLEN; i++)
            y[i] = temp2[i];
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
            uint128_t mi = ((uint128_t) T_[i] * P_[0]) & MASK;
            for (int j = 0; j < s; j++) {
                uint128_t temp = mi * P[j] + T_[i + j] + carry;
                T_[i + j] = temp & MASK;
                carry = temp >> W;
            }

            int count = 0;
            while (carry) {
                uint128_t temp = (uint128_t) T_[i + s + count] + carry;
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
        // cout << "mod_mul_mont" << endl;

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

void print_res(int128_t x){
    if(x < 0){
        x = -x;
        putchar('-');
    }
    if(x > 9)
        print_res(x / 10);
    putchar(x % 10 + '0');
    return;
}

int mod_3(uint64_t a[Bigint::MAXLEN]);
int128_t gcd(int128_t a, int128_t b, int128_t & x, int128_t & y);
void func(uint64_t x[Bigint::MAXLEN], int128_t & a, int128_t & b, int128_t n, uint64_t alpha[Bigint::MAXLEN], uint64_t beta[Bigint::MAXLEN]);

void run(int128_t & res, int128_t n, uint64_t alpha[Bigint::MAXLEN], uint64_t beta[Bigint::MAXLEN]) {
    uint64_t x[Bigint::MAXLEN] = {
        1
    };
    int128_t a = 0;
    int128_t b = 0;

    func(x, a, b, n, alpha, beta);
    uint64_t xx[Bigint::MAXLEN];
    memcpy(xx, x, Bigint::MAXLEN * sizeof(uint64_t));
    int128_t aa = a;
    int128_t bb = b;

    func(xx, aa, bb, n, alpha, beta);

    while (Bigint::equal(x, xx) == false) {
        func(x, a, b, n, alpha, beta);
        func(xx, aa, bb, n, alpha, beta);
        func(xx, aa, bb, n, alpha, beta);
    }

    int128_t tempb = (bb - b) % n;
    int128_t tempa = (a - aa) % n;
    if (tempa < 0)
        tempa += n;
    if (tempb < 0)
        tempb += n;

    int128_t y, z;
    int128_t yy, zz;
    int128_t d = gcd(tempb, n, yy, zz);
    int128_t dd = gcd(tempb, n, y, z);
    if (tempa % dd == 0){
        int128_t k = tempa / dd;
        y *= k;
        z *= k;
    }

    res = y % n;

    if (res < 0)
        res += n;
    if (n % d == 0) {
        int128_t t = n / d;
        res = res % t;
        uint64_t sx[Bigint::MAXLEN];
        memset(sx, 0, Bigint::MAXLEN * sizeof(uint64_t));
        sx[0] = res;
        uint64_t T[Bigint::MAXLEN];
        memset(T, 0, Bigint::MAXLEN * sizeof(uint64_t));
        T[0] = t;
        for (int i = 0; i < d; i++) {
            uint64_t tm[Bigint::MAXLEN];
            Bigint::mod_pow_mont(tm, alpha, sx);
            if (Bigint::equal(tm, beta)) {
                res = sx[0];
                break;
            }
            Bigint::mod_add(sx, sx, T);
        }
    }
}

int mod_3(uint64_t a[Bigint::MAXLEN]) {
    uint128_t d = 0;
    for (int i = Bigint::MAXLEN - 1; i >= 0; i--) {
        d = (d * Bigint::BASE + a[i]) % 3;
    }
    return static_cast < int > (d);
}

void func(uint64_t x[Bigint::MAXLEN], int128_t & a, int128_t & b, int128_t n, uint64_t alpha[Bigint::MAXLEN], uint64_t beta[Bigint::MAXLEN]) {
    int d = mod_3(x);
    if (d == 1) {
        Bigint::mod_mul_mont(x, beta, x);
        b += 1;
        b %= n;
    }
    if (d == 0) {
        Bigint::mod_mul_mont(x, x, x);
        a *= 2;
        a %= n;
        b *= 2;
        b %= n;
    }
    if (d == 2) {
        Bigint::mod_mul_mont(x, alpha, x);
        a += 1;
        a %= n;
    }
}

int128_t gcd(int128_t a, int128_t b, int128_t & x, int128_t & y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int128_t d = gcd(b, a % b, x, y);
    int128_t temp = x;
    x = y;
    y = temp - a / b * y;
    return d;
}

uint64_t n;
string a;
string b;
uint64_t alpha[Bigint::MAXLEN] = {0};
uint64_t beta[Bigint::MAXLEN] = {0};

void get_input(){
    cin >> Bigint::p >> n >> a >> b;
    reverse(Bigint::p.begin(), Bigint::p.end());
    Bigint::str2bi(Bigint::P, Bigint::p);
    Bigint::P_bits = Bigint::getBits(Bigint::P);
    Bigint::pre_cal();

    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    Bigint::str2bi(alpha, a);
    Bigint::str2bi(beta, b);
    return;
}

int main() {
    get_input();

    int128_t res;
    run(res, n, alpha, beta);
    print_res(res);
    cout << endl;

    return 0;
}
// a = 3107382411142271813235322646657672922264748410711464860476
// b = 2 * 2 * 23 * 8783 * 2419781956425763 * 192888768642311611 * 22331109 = 8422173351577032824121190124284383421929457332
// p = 3768901521908407201157691198029711972876087647970824596533
// n = 9993115456385501509
// alpha = 1766577746365105652744150503398894335527611770321820080096
// beta  = 2120553873612439845419858696451540936395844505496867133711