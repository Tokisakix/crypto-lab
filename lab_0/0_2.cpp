#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using namespace std;

const uint32_t N = 1e5;

int main() {

#ifdef ONLINE_JUDGE
    uint8_t xor_value;
    uint32_t data_len;
    cin.read(reinterpret_cast<char*>(&xor_value), sizeof(xor_value));
    cin.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));

    uint8_t res[N];
    uint32_t len = 0;
    while(data_len > 0){
        if(data_len >= N){
            len = N;
            data_len -= N;
        }else{
            len = data_len;
            data_len = 0;
        }

        cin.read(reinterpret_cast<char*>(res), len * sizeof(uint8_t));
        for(uint32_t idx = 0;idx < len;idx ++){
            res[idx] ^= xor_value;
        }
        cout.write(reinterpret_cast<char*>(res), len * sizeof(uint8_t));
    }
#else
    ifstream file("data/0_2.bin", ios::binary);

    uint8_t xor_value;
    uint32_t data_len;
    file.read(reinterpret_cast<char*>(&xor_value), sizeof(xor_value));
    file.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));

    uint8_t res[N];
    uint32_t len = 0;
    while(data_len > 0){
        if(data_len >= N){
            len = N;
            data_len -= N;
        }else{
            len = data_len;
            data_len = 0;
        }

        file.read(reinterpret_cast<char*>(res), len * sizeof(uint8_t));
        for(uint32_t idx = 0;idx < len;idx ++){
            res[idx] ^= xor_value;
        }
        cout.write(reinterpret_cast<char*>(res), len * sizeof(uint8_t));
    }
#endif

    return 0;
}
