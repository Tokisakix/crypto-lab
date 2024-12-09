#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <cstring>

using namespace std;

uint64_t b_r = 0;
uint64_t tot_b = 0;
uint8_t H_in[64];
uint32_t H[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const uint32_t KEY[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


void hash_func(uint8_t* H_in) {
    uint32_t a[64];

    for (int b = 0; b < 16; ++b) {
        a[b] = (H_in[4 * b] << 24) | (H_in[4 * b + 1] << 16) | (H_in[4 * b + 2] << 8) | H_in[4 * b + 3];
    }

    for (int b = 16; b < 64; ++b) {
        uint32_t c = ((a[b - 15] >> 7) | (a[b - 15] << 25)) ^
                     ((a[b - 15] >> 18) | (a[b - 15] << 14)) ^
                     (a[b - 15] >> 3);
        uint32_t d = ((a[b - 2] >> 17) | (a[b - 2] << 15)) ^
                     ((a[b - 2] >> 19) | (a[b - 2] << 13)) ^
                     (a[b - 2] >> 10);
        a[b] = a[b - 16] + c + a[b - 7] + d;
    }

    uint32_t b = H[0], c = H[1], d = H[2], e = H[3];
    uint32_t f = H[4], g = H[5], a1 = H[6], a2 = H[7];

    for (int b1 = 0; b1 < 64; ++b1) {
        uint32_t c1 = ((f >> 6) | (f << 26)) ^
                      ((f >> 11) | (f << 21)) ^
                      ((f >> 25) | (f << 7));
        uint32_t d1 = (f & g) ^ ((~f) & a1);
        uint32_t e1 = a2 + c1 + d1 + KEY[b1] + a[b1];
        uint32_t f1 = ((b >> 2) | (b << 30)) ^
                      ((b >> 13) | (b << 19)) ^
                      ((b >> 22) | (b << 10));
        uint32_t g1 = (b & c) ^ (b & d) ^ (c & d);
        uint32_t a3 = f1 + g1;

        a2 = a1;
        a1 = g;
        g = f;
        f = e + e1;
        e = d;
        d = c;
        c = b;
        b = e1 + a3;
    }

    H[0] += b;
    H[1] += c;
    H[2] += d;
    H[3] += e;
    H[4] += f;
    H[5] += g;
    H[6] += a1;
    H[7] += a2;
}

int main(){
    uint8_t* tot_b_r = (uint8_t*)(&tot_b);
    while(true){
        b_r = fread(H_in, 1, sizeof(H_in), stdin);
        // if (b_r == 0) {
        //     break;
        // }

        if (b_r == 64) {
            tot_b += 64;
            hash_func(H_in);
            continue;
        }

        tot_b = (tot_b + b_r) * 8;
        H_in[b_r] = 0x80;

        if (b_r <= 55) {
            memset(H_in + b_r + 1, 0, 55 - b_r);
            for (int i = 0; i < 8; ++i) {
                H_in[56 + i] = tot_b_r[7 - i];
            }
            hash_func(H_in);
        } else {
            memset(H_in + b_r + 1, 0, 63 - b_r);
            hash_func(H_in);

            uint8_t H_in_end[64] = {0};
            for (int i = 0; i < 8; ++i) {
                H_in_end[56 + i] = tot_b_r[7 - i];
            }
            hash_func(H_in_end);
        }

        break;
    }

    uint8_t* H_out = (uint8_t*)&H;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 4; ++j){
            fwrite(H_out + 4 * i + 3 - j, sizeof(uint8_t), 1, stdout);
        }
    }

    return 0;
}
