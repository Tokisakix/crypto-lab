#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using namespace std;

int main() {

#ifdef ONLINE_JUDGE
    uint8_t xor_value;
    uint32_t data_len;
    cin >> xor_value >> data_len;

    uint8_t temp_byte;
    for(uint32_t i = 0;i < data_len;i ++){
        cin >> temp_byte;
        printf("%02X ", temp_byte ^ xor_value);
    }
    printf("\n");
#else
    ifstream file("data/0_2.bin", ios::binary);

    uint8_t xor_value;
    uint32_t data_len;
    file.read(reinterpret_cast<char*>(&xor_value), sizeof(xor_value));
    file.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));

    uint8_t temp_byte;
    for(uint32_t i = 0;i < data_len;i ++){
        file.read(reinterpret_cast<char*>(&temp_byte), sizeof(temp_byte));
        printf("%02X ", temp_byte ^ xor_value);
    }
    printf("\n");
#endif

    return 0;
}
