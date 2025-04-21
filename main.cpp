#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

ofstream out;

char M[16][16];

void pripravaMatrike(string kljuc) {
    vector<unsigned char> ASC(256);
    for (int i = 0; i < 256; ++i) {
        ASC[i] = (unsigned char)i;
    }
    
    string unikaten;
    for (char c : kljuc) {
        if (unikaten.find(c) == string::npos) {
            unikaten += c;
        }
    }

    for (int i = 0; i < unikaten.length(); ++i) {
        unsigned char znak = unikaten[i];
        swap(ASC[znak], ASC[i]);
    }

    sort(ASC.begin() + unikaten.length(), ASC.end());



    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            M[i][j] = ASC[i * 16 + j];
        }
    }

}
string inputText(const string& pot) {
    ifstream input(pot);
    stringstream sstream;

    if (!input.is_open()) {
        return string();
    }

    sstream << input.rdbuf();
    return sstream.str();
}

string predobdelava(const string& vhod) {
    string obdelan;
    for (int i = 0; i < vhod.size(); ) {
        if (i + 1 >= vhod.size()) {
            obdelan += vhod[i];
            obdelan += '\x03';  // ASCII EOF
            break;
        }

        char prvi = vhod[i];
        char drugi = vhod[i + 1];

        if (prvi == drugi) {
            obdelan += prvi;
            obdelan += '\x00';  
            i += 1;  
        }
        else {
            obdelan += prvi;
            obdelan += drugi;
            i += 2;
        }
    }
    return obdelan;
}


int main(int argc, const char* const argv[]) {
    if (argc != 4) {
        cerr << "Uporaba: " << argv[0] << " <e|d> <ključ> <datoteka>" << endl;
        return -1;  
    }

    // Preberi argumente
    string mode = argv[1];
    string key = argv[2];
    string filePath = argv[3];

    if (mode == "e") {
        out.open("out_e.txt");
    }
    else if (mode == "d") {
        out.open("out_d.txt");
    }
    else {
        cerr << "Napaka: prvi argument mora biti 'e' za šifriranje ali 'd' za dešifriranje." << endl;
        return -2;  
    }

    if (!out) {
        cerr << "Napaka pri odpiranju izhodne datoteke!" << endl;
        return -3; 
    }

    if (mode == "e") {
        // encryptPlayfair(key, filePath, out);
    }
    else {
        // decryptPlayfair(key, filePath, out);
    }

    out.close();
    return 0;
}
