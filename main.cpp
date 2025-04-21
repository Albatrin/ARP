#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

static unsigned char M[16][16];

void pripravaMatrike(const string& kljuc) {
    vector<unsigned char> ASC(256);
    for (int i = 0; i < 256; ++i) {
        ASC[i] = (unsigned char)i;
    }

    string unikaten;
    for (unsigned char c : kljuc) {
        if (unikaten.find(c) == string::npos)
            unikaten += c;
    }

    for (size_t i = 0; i < unikaten.size(); ++i) {
        swap(ASC[unikaten[i]], ASC[i]);
    }

    sort(ASC.begin() + unikaten.size(), ASC.end());

    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            M[i][j] = ASC[i * 16 + j];
}

static vector<unsigned char> preobdelava(const string& input) {
    vector<unsigned char> out;
    size_t i = 0;
    while (i < input.size()) {
        unsigned char a = input[i];
        out.push_back(a);

        if (i + 1 < input.size()) {
            unsigned char b = input[i + 1];
            if (a == b) {
                out.push_back(0x00);
                i += 1;
            }
            else {
                out.push_back(b);
                i += 2;
            }
        }
        else {
            out.push_back(0x03);
            break;
        }
    }
    return out;
}

string playfairEncrypt(const string& kljuc, const string& plain) {
    pripravaMatrike(kljuc);
    vector<unsigned char> P = preobdelava(plain);
    string cipher;
    for (size_t idx = 0; idx < P.size(); idx += 2) {
        unsigned char c1 = P[idx];
        unsigned char c2 = (idx + 1 < P.size() ? P[idx + 1] : 0x03);

        int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
        for (int x = 0; x < 16; ++x)
            for (int y = 0; y < 16; ++y) {
                if (M[x][y] == c1) { x1 = x; y1 = y; }
                if (M[x][y] == c2) { x2 = x; y2 = y; }
            }
        if (x1 < 0 || x2 < 0) continue;

        unsigned char e1, e2;
        if (x1 != x2 && y1 != y2) {
            e1 = M[x1][y2];
            e2 = M[x2][y1];
        }
        else if (x1 == x2) {
            e1 = M[x1][(y1 + 1) % 16];
            e2 = M[x2][(y2 + 1) % 16];
        }
        else {
            e1 = M[(x1 + 1) % 16][y1];
            e2 = M[(x2 + 1) % 16][y2];
        }

        cipher.push_back(char(e1));
        cipher.push_back(char(e2));
    }
    return cipher;
}

string playfairDecrypt(const string& kljuc, const string& cipher) {
    pripravaMatrike(kljuc);
    string plain;
    for (size_t idx = 0; idx + 1 < cipher.size(); idx += 2) {
        unsigned char c1 = cipher[idx];
        unsigned char c2 = cipher[idx + 1];

        int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
        for (int x = 0; x < 16; ++x)
            for (int y = 0; y < 16; ++y) {
                if (M[x][y] == c1) { x1 = x; y1 = y; }
                if (M[x][y] == c2) { x2 = x; y2 = y; }
            }
        if (x1 < 0 || x2 < 0) continue;

        unsigned char d1, d2;
        if (x1 != x2 && y1 != y2) {
            d1 = M[x1][y2];
            d2 = M[x2][y1];
        }
        else if (x1 == x2) {
            d1 = M[x1][(y1 + 15) % 16];
            d2 = M[x2][(y2 + 15) % 16];
        }
        else {
            d1 = M[(x1 + 15) % 16][y1];
            d2 = M[(x2 + 15) % 16][y2];
        }

        plain.push_back(char(d1));
        if (d2 == 0x00) {
            plain.push_back(char(d1));
        }
        else if (d2 == 0x03) {
            break;  // konec
        }
        else {
            plain.push_back(char(d2));
        }
    }
    return plain;
}

string inputText(const string& path) {
    ifstream in(path, ios::binary);
    if (!in) return {};
    stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uporaba: " << argv[0] << " <e|d> <ključ> <datoteka>\n";
        return 1;
    }

    string mode = argv[1];
    string key = argv[2];
    string file = argv[3];

    string data = inputText(file);
    if (data.empty()) {
        cerr << "Napaka pri branju datoteke!\n";
        return 2;
    }

    if (mode == "e") {
        string enc = playfairEncrypt(key, data);
        ofstream out("out_e.txt", ios::binary);
        out << enc;
    }
    else if (mode == "d") {
        string dec = playfairDecrypt(key, data);
        ofstream out("out_d.txt", ios::binary);
        out << dec;
    }
    else {
        cerr << "Prvi argument mora biti 'e' ali 'd'.\n";
        return 3;
    }

    return 0;
}
