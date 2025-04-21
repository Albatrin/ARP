#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

ofstream out;

string inputText(const string& pot) {
    ifstream input(pot);
    stringstream sstream;

    if (!input.is_open()) {
        return string();
    }

    sstream << input.rdbuf();
    return sstream.str();
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
