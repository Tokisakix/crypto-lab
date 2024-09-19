#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <tuple>

using namespace std;

string encode;
// K ccpkbgu fdp hq tyavinrrt mvg rkdnbv fd etdgi ltxrgu ddk o tfmb pvge G ltg ck qracqc. Wdn awcrxiza kft lewrpt ycqkyvx ch kft poncq qr hjv ajuwe tmcmspkq dy hjv dahct rls vskcgcz qqdzxgsf rls wcwsjt bh. Afs iasp rjahk jrju mvg kmitz hfp dispzlvl gwtf pl kkebdpg ceb shctj. Rwxb afs pezqn rwx cvycg aonw ddk ackawbbi kft iovkcgg. Hjvl nhi ffsqes vyc lacnv rwbbi repbb vf exos c dygzwp fd tkfqi. Ycw hjvl nhi qibtk hjv npist.
// S lotabgy kqmasbaew qy n eizuwuggakgy kknrem lbj dkeanevfo zuw iagzmtgakogq wl qaoogst srkagtwa ue vwihemtgk. I bndqj qaoogst yvyvggmzk bf i srkagtw ooiwa g ewkocamtg uwtsalkaum zusb zuw ukfkimr uisr xzuz s akavmx xfwca lw zuw zkpaxorfb.  Jvyqznd aotfizhjmy njm g flitqszj rdmsrfb us ewyg uzeclwmesxnvu xxblwibd aavlmy, nfl gew kuzewtyq cyrv nue kwlgoixr vqygjqhhlqua, xqtnfkond bxnfagplquak, kualzgpl ugasokzwvz fgnzjszk, nfl oa gbnrj kgfwa cuwzk vl qy vexuelitg lw jrlmig xwxtwze bj bgzhmxvfo.

float alphaRate[26] = {
    0.082, 0.015, 0.028, 0.043, 0.127,
    0.022, 0.020, 0.061, 0.070, 0.002,
    0.008, 0.040, 0.024, 0.067, 0.075,
    0.019, 0.001, 0.060, 0.063, 0.091,
    0.028, 0.010, 0.023, 0.001, 0.020,
    0.001, 
};

vector<vector<char>> splitText(string encode, int length) {
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

vector<float> calFreq(vector<char> text) {
    vector<float> freq(26, 0.0);
    float tot = 0.0;
    for (char ch : text) {
        if (isalpha(ch)) {
            freq[toupper(ch) - 'A'] += 1.0;
            tot += 1.0;
        }
    }
    for (int i = 0; i < 26; i++) {
        freq[i] /= tot;
    }
    return freq;
}

float calcChiSquare(vector<float> freq) {
    float chiSquare = 0.0;
    for (int i = 0; i < 26; i++) {
        chiSquare += pow(freq[i] - alphaRate[i], 2) / alphaRate[i];
    }
    return chiSquare;
}

tuple<int, float> findBestShift(vector<char> text) {
    float minChiSquare = 1e9;
    int bestShift = 0;

    for (int shift = 0; shift < 26; shift++) {
        vector<char> shiftedText = text;
        for (char &ch : shiftedText) {
            ch = (toupper(ch) - 'A' - shift + 26) % 26 + 'A';
        }

        vector<float> freq = calFreq(shiftedText);
        float chiSquare = calcChiSquare(freq);
        
        if (chiSquare < minChiSquare) {
            minChiSquare = chiSquare;
            bestShift = shift;
        }
    }

    tuple<int, float> res(bestShift, minChiSquare);
    
    return res;
}

string decryptVigenere(string ciphertext, string key) {
    string plaintext = ciphertext;
    int keyLen = key.length();
    
    for (int i = 0, j = 0; i < (int)ciphertext.length(); i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - base - (toupper(key[j % keyLen]) - 'A') + 26) % 26 + base;
            j++;
        }
    }
    
    return plaintext;
}

int main() {
    getline(cin, encode);

    string res_text = "";
    string res_key = "";
    float res_score = 1e9;

    for(int keyLength = 1;keyLength <= 10;keyLength ++){
        vector<vector<char>> split_text = splitText(encode, keyLength);
        
        string key = "";
        float decryptedScore = 0.0;
        for (int i = 0; i < keyLength; i++) {
            tuple<int, float> bestres = findBestShift(split_text[i]);
            int bestShift = get<0>(bestres);
            decryptedScore += get<1>(bestres);
            key += (char)(bestShift + 'A');
        }
        
        string decryptedText = decryptVigenere(encode, key);
        if(decryptedScore < res_score){
            res_score = decryptedScore;
            res_text = decryptedText;
            res_key = key;
        }
    }

    cout << res_key << endl;
    cout << res_text << endl;
    
    return 0;
}
