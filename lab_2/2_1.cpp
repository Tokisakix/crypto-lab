#include <iostream>
#include <fstream>
#include <stdint.h>
#include <immintrin.h>

#define M 131

using namespace std;

#ifdef __GNUC__
#pragma GCC target("pclmul,sse2")
#endif

void gf_mod(uint64_t res[5], const uint64_t f1[5]);
void gf_mul(uint64_t res[5], const uint64_t f1[5], const uint64_t f2[5]);
void gf_pow(uint64_t res[5], const uint64_t f1[5]);

void gf_mul(uint64_t res[5], const uint64_t f1[5], const uint64_t f2[5]){
    __m128i A0 = _mm_set_epi64x(0, f1[0]);
    __m128i A1 = _mm_set_epi64x(0, f1[1]);
    __m128i A2 = _mm_set_epi64x(0, f1[2]);
    __m128i B0 = _mm_set_epi64x(0, f2[0]);
    __m128i B1 = _mm_set_epi64x(0, f2[1]);
    __m128i B2 = _mm_set_epi64x(0, f2[2]);

    __m128i P0 = _mm_clmulepi64_si128(A0, B0, 0x00);
    __m128i P1 = _mm_clmulepi64_si128(A1, B1, 0x00);
    __m128i P2 = _mm_clmulepi64_si128(A2, B2, 0x00);
    __m128i P3 = _mm_xor_si128(_mm_clmulepi64_si128(_mm_xor_si128(A0, A1), _mm_xor_si128(B0, B1), 0x00), _mm_xor_si128(P0, P1));
    __m128i P4 = _mm_xor_si128(_mm_clmulepi64_si128(_mm_xor_si128(A0, A2), _mm_xor_si128(B0, B2), 0x00), _mm_xor_si128(P0, P2));
    __m128i P5 = _mm_xor_si128(_mm_clmulepi64_si128(_mm_xor_si128(A1, A2), _mm_xor_si128(B1, B2), 0x00), _mm_xor_si128(P1, P2));

    uint64_t temp[5] = {0};
    temp[0] = _mm_extract_epi64(P0, 0);
    temp[1] = _mm_extract_epi64(P3, 0) ^ _mm_extract_epi64(P0, 1);
    temp[2] = _mm_extract_epi64(_mm_xor_si128(P1, P4), 0) ^ _mm_extract_epi64(P3, 1);
    temp[3] = _mm_extract_epi64(P5, 0) ^ _mm_extract_epi64(_mm_xor_si128(P1, P4), 1);
    temp[4] = _mm_extract_epi64(P2, 0) ^ _mm_extract_epi64(P5, 1);

    gf_mod(res, temp);
    return;
}

void gf_mod(uint64_t res[5], const uint64_t f1[5]){
    uint64_t temp[5];
    for (int i = 0; i < 5; i++){
        temp[i] = f1[i];
    }

    for (int i = 4; i >= 3; i--){
        uint64_t t = temp[i];
        temp[i - 3] = temp[i - 3] ^ (t << 61) ^ (t << 62) ^ (t << 63);
        temp[i - 2] = temp[i - 2] ^ (t << 10) ^ (t >> 1) ^ (t >> 2) ^ (t >> 3);
        temp[i - 1] = temp[i - 1] ^ (t >> 54);
    }

    uint64_t t = temp[2] >> 3;
    res[0] = temp[0] ^ (t << 13) ^ t ^ (t << 1) ^ (t << 2);
    res[1] = temp[1] ^ (t >> 51);
    res[2] = temp[2] & 0x7;
    return;
}

void gf_pow(uint64_t res[5], const uint64_t f1[5]){
    __m128i A0 = _mm_set_epi64x(0, f1[0]);
    __m128i A1 = _mm_set_epi64x(0, f1[1]);
    __m128i A2 = _mm_set_epi64x(0, f1[2]);

    __m128i P0 = _mm_clmulepi64_si128(A0, A0, 0x00);
    __m128i P1 = _mm_clmulepi64_si128(A1, A1, 0x00);
    __m128i P2 = _mm_clmulepi64_si128(A2, A2, 0x00);

    uint64_t temp[5] = {0};
    temp[0] = _mm_extract_epi64(P0, 0);
    temp[1] = _mm_extract_epi64(P0, 1);
    temp[2] = _mm_extract_epi64(P1, 0);
    temp[3] = _mm_extract_epi64(P1, 1);
    temp[4] = _mm_extract_epi64(P2, 0);

    gf_mod(res, temp);
    return;
}

int main(){

#ifndef ONLINE_JUDGE
ifstream file("data/2_1.bin", ios::binary);
#define cin file
#endif

    uint32_t op_num;
    cin.read(reinterpret_cast<char*>(&op_num), sizeof(op_num));
    while(op_num--){
        uint8_t op;
        uint64_t f1[5] = {0};
        uint64_t f2[5] = {0};
        uint64_t res[5] = {0};

        cin.read(reinterpret_cast<char*>(&op), sizeof(op));
        cin.read(reinterpret_cast<char*>(&f1), 3 * sizeof(uint64_t));
        cin.read(reinterpret_cast<char*>(&f2), 3 * sizeof(uint64_t));


        if (op == 0x00){
            for (int i = 0; i < 3; i++){
                res[i] = f1[i] ^ f2[i];
            }
        }
        else if (op == 0x01){
            gf_mul(res, f1, f2);
        }
        else if (op == 0x02){
            gf_pow(res, f1);
        }
        else if (op == 0x03){
            uint64_t f2[5];
            for (int i = 0; i < 5; i++) {
                f2[i] = f1[i];
            }

            uint64_t n = M - 1;
            uint64_t mask = 0x40;
            const int totalBits = 8;
            int powCount = 1;

            for (int bit = 0; bit < totalBits - 1; bit++) {
                uint64_t temp[5] = {0};
                for (int i = 0; i < 5; i++) {
                    temp[i] = f2[i];
                }

                if (n & mask) {
                    for (int i = 0; i < powCount; i++) {
                        gf_pow(temp, temp);
                    }
                    gf_mul(temp, f2, temp);
                    gf_pow(temp, temp);
                    gf_mul(f2, f1, temp);
                    powCount = powCount * 2 + 1;
                } else {
                    for (int i = 0; i < powCount; i++) {
                        gf_pow(temp, temp);
                    }
                    gf_mul(f2, f2, temp);
                    powCount *= 2;
                }

                mask >>= 1;
            }

            gf_pow(res, f2);
        }
        
        cout.write(reinterpret_cast<char*>(res), 3 * sizeof(uint64_t));
    }

    return 0;
}