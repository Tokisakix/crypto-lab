#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <tuple>

using namespace std;

float alphaRate[26] = {
    0.082, 0.015, 0.028, 0.043, 0.127,
    0.022, 0.020, 0.061, 0.070, 0.002,
    0.008, 0.040, 0.024, 0.067, 0.075,
    0.019, 0.001, 0.060, 0.063, 0.091,
    0.028, 0.010, 0.023, 0.001, 0.020,
    0.001, 
};
string encode;
float freq[26] = {};
float decryptedScore = 0.0;

vector<vector<char>> splitText(string& encode, int length) {
    vector<vector<char>> res(length);
    int idx = 0;
    for (char ch : encode) {
        if (isalpha(ch)) {
            res[idx % length].push_back(ch);
            idx++;
        }
    }
    return res;
}

void calFreq(vector<char>& text) {
    float tot = text.size();
    for (char ch : text) {
        if (isalpha(ch)) {
            freq[toupper(ch) - 'A'] += 1.0;
        }
    }
    for (int i = 0; i < 26; i++) {
        freq[i] /= tot;
    }
    return;
}

int findBestShift(vector<char>& text) {
    float minChiSquare = 1e9;
    int bestShift = 0;

    for (int shift = 0; shift < 26; shift++) {
        vector<char> shiftedText = text;
        for (char &ch : shiftedText) {
            ch = (toupper(ch) - 'A' - shift + 26) % 26 + 'A';
        }

        calFreq(shiftedText);
        float chiSquare = 0.0;
        for (int i = 0; i < 26; i++) {
            float temp = freq[i] - alphaRate[i];
            chiSquare += (temp * temp) / alphaRate[i];
        }
        
        if (chiSquare < minChiSquare) {
            minChiSquare = chiSquare;
            bestShift = shift;
        }
    }

    decryptedScore += minChiSquare;
    
    return bestShift;
}

void decryptVigenere(string& ciphertext, string& key) {
    int keyLen = key.length();
    
    for (int i = 0, j = 0; i < (int)ciphertext.length(); i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            ciphertext[i] = (ciphertext[i] - base - (toupper(key[j % keyLen]) - 'A') + 26) % 26 + base;
            j++;
        }
        printf("%c", ciphertext[i]);
    }
    
    return;
}

int main() {
    encode = "";
    string sub_str;
    while(getline(cin, sub_str)){
        sub_str += "\n";
        encode += sub_str;
    }

    string res_key = "";
    float res_score = 1e9;
    int key_max_length = encode.size() / 50;

    for(int keyLength = 4;keyLength <= key_max_length;keyLength ++){
        vector<vector<char>> split_text = splitText(encode, keyLength);
        decryptedScore = 0.0;
        
        string key = "";
        for (int i = 0; i < keyLength; i++) {
            int bestShift = findBestShift(split_text[i]);
            key += (char)(bestShift + 'A');
        }
        
        decryptedScore /= keyLength;
        if(decryptedScore < res_score){
            res_score = decryptedScore;
            res_key = key;
        }
    }

    cout << res_key << endl;
    decryptVigenere(encode, res_key);
    
    return 0;
}
