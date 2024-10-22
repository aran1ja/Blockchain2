#include <iostream>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>   
#include <unordered_map>      

using namespace std;

string hexPadarymas(bitset<256>& bitai) {
    stringstream ss;

    for (int i = 0; i < 256; i += 4) // Paemame po 4 bitus is 256 tam, kad konvertuoti 4 bitus i 1 hex
    {
        int reiksme = 0; 
        reiksme += bitai[i] * 8; 
        reiksme += bitai[i + 1] * 4;
        reiksme += bitai[i + 2] * 2;
        reiksme += bitai[i + 3] * 1;

        ss << hex << reiksme;
    }
    return ss.str();
}

void pakeitimasPo16bitus(bitset<256>& bitai, int reiksme_1, int reiksme_2) {
    for (int i = 0; i < 16; i++) {
        bool laikinas = bitai[reiksme_1 + i];
        bitai[reiksme_1 + i] = bitai[reiksme_2 + i]; 
        bitai[reiksme_2 + i] = laikinas;  
    }
}

void bitaiKeiciasiVietomis(bitset<256>& bitai) {
    int binarinis_ilgis = bitai.size(); 
    for (int i = 0; i < binarinis_ilgis / 2; i++) 
    // Dalinu is 2, nes jei turime 10 bitu, tai sukeiciama pagal toki principa: 1-10, 2-9, 3-8, 4-7, 5-6
    {
        int laikinas = bitai[i];
        bitai[i] = bitai[binarinis_ilgis - i - 1]; 
        bitai[binarinis_ilgis - i - 1] = laikinas;
    }

    for (int i = 0; i < binarinis_ilgis; i++) {
        if (bitai[i] == 1) { 
            bitai[(i + 15) % 256].flip();    
        }
    }
    
    // Bandymas sugeneruoti daugiau 1 ir 0
    for (int i = 0; i < binarinis_ilgis / 2; i++) {
        if (bitai[i] == bitai[binarinis_ilgis / 2 + i]) { // Patikriname ar pirmoji puse kodo lygi su kitaja
            bitai[i] = 1; // Jeigu taip - rasome 1
        } else {
            bitai[i] = 0; // Jeigu ne - rasome 0
        }
    }
    
    pakeitimasPo16bitus(bitai,  0, 256);
    pakeitimasPo16bitus(bitai, 16, 240);
    pakeitimasPo16bitus(bitai, 32, 224);
    pakeitimasPo16bitus(bitai, 48, 208);
    pakeitimasPo16bitus(bitai, 64, 192);
    pakeitimasPo16bitus(bitai, 80, 176);
    pakeitimasPo16bitus(bitai, 96, 160);
    pakeitimasPo16bitus(bitai, 112, 144);

    for (int i = 0; i < binarinis_ilgis / 2; i++) {
        if (bitai[i] == bitai[binarinis_ilgis - i - 1]) { // Patikriname ar pirmoji puse kodo lygi kodo galui
            bitai[i] = 1; // Jeigu taip - rasome 1
        } else {
            bitai[i] = 0; // Jeigu ne - rasome 0
        }
    }
    
    //cout << "Binarinis kodas: " << bitai << endl;
}

string hashFunkcija(string simboliu_seka) {
        int ascii_suma = 0; // Naudojama ascii reiksmiu sumai
        int daugiklis = 1; 
    
        int ilgis = 8 * simboliu_seka.size(); // Simboliu sekos ilgis bitais
        //cout << "Simboliu seka yra: " << simboliu_seka << endl;
        //cout << "-----------------------------------------------------------------" << endl;
        // cout << "Jusu zodis susideda is " << ilgis << " bitu." << endl;

        // Zodis isverciamas i ASCII
        for (char simbolis : simboliu_seka) {
            //cout << "Simbolis " << simbolis << " uzkoduotas kaip " << (int)simbolis << " pagal ASCII" << endl;
            ascii_suma += (int)simbolis * daugiklis;
            daugiklis++;
        }

        // Ascii_suma isverciama i binary
        bitset<256> binarinis_kodas((int)ascii_suma);
        //bitset<256> binarinis_kodas_kopija = binarinis_kodas; // Kuriama kopija
        //cout << "Padauginta ASCII suma pavaizduota kaip binarinis kodas: " << binarinis_kodas << endl;
        for (int i = 0; i < 3; i++)
        {
            //bitaiKeiciasiVietomis(binarinis_kodas_kopija);
            bitaiKeiciasiVietomis(binarinis_kodas);
        }
        
        //string hexKodas = hexPadarymas(binarinis_kodas_kopija);
        string hexKodas = hexPadarymas(binarinis_kodas);
        //cout << "Hash kodo atvaizdavimas hex pavidalu: " << hex << hexKodas << endl;
        return hexKodas;
}

string druskosPridejimas(int ilgis) {
    char simboliai[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string druska;

    for (int i = 0; i < ilgis; i++) {
        druska += simboliai[rand() % (sizeof(simboliai) - 1)]; 
    }

    return druska;
}

string hashFunkcijaSuDruska(string simboliu_seka) {
        int ascii_suma = 0; 
        int daugiklis = 1; 

        string druska = druskosPridejimas(16);
        string simboliu_seka_su_druska = simboliu_seka + druska;
        int ilgis = 8 * simboliu_seka_su_druska.size();

        // Zodis isverciamas i ASCII
        for (char simbolis : simboliu_seka_su_druska) {
            ascii_suma += (int)simbolis * daugiklis;
            daugiklis++;
        }

        bitset<256> binarinis_kodas((int)ascii_suma);
        bitset<256> binarinis_kodas_kopija = binarinis_kodas; 
        for (int i = 0; i < 5; i++)
        {
            bitaiKeiciasiVietomis(binarinis_kodas_kopija);
        }
        
        string hexKodas = hexPadarymas(binarinis_kodas_kopija);
        //cout << "Hash kodas su druska hex pavidalu:    " << hex << hexKodas << endl;
        return druska + hexKodas;
}

string nuskaitymasIsFailo(string failoPavadinimas) {
    ifstream failas(failoPavadinimas);
    if (!failas.is_open()) { 
        cout << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return " "; 
    }

    string turinys;
    while (getline(failas, turinys))  

    failas.close();

    return turinys;
}

string failoGeneravimas (string failoPavadinimas, int simboliu_sk) {
    ofstream fail(failoPavadinimas);
    string random_simboliai;
    
    for (int i = 0; i < simboliu_sk; i++) {
        random_simboliai = rand() % 95 + 32; 
        fail << random_simboliai;
    }
    
    fail.close();
    cout << "Sugeneruoto failo pavadinimas: " << failoPavadinimas << endl;
}

double laikoMatavimas(vector<string>& linijos) {
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; i++)
    {
        for (const auto& linija : linijos) {
            hashFunkcija(linija);
        }
    }
    auto pabaiga = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> skirtumas = pabaiga - start;
    return skirtumas.count() / 5; 
}

void konstitucija(const string& filename) {
    ifstream file(filename);
    vector<string> linijos; // Cia issaugomi duomenys
    string linija;

    while (getline(file, linija)) {
        linijos.push_back(linija);
    }

    int linijuKiekis;
    cout << "Kiek eiluciu norite hashuoti? (Max: " << linijos.size() << "): "; cin >> linijuKiekis;

    vector<string> hashuojamos_eilutes(linijos.begin(), linijos.begin() + linijuKiekis);

    double vidutinis_laikas = laikoMatavimas(hashuojamos_eilutes);
    cout << "Vidutinis hashavimo laikas: " << vidutinis_laikas << " milisekundziu." << endl;
}

string randomSimboliuGeneravimas(int poru_ilgis) {
    char simboliukai[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string sukurtasStringas;
    
    for (int i = 0; i < poru_ilgis; i++) {
        sukurtasStringas += simboliukai[rand() % (sizeof(simboliukai) - 1)]; 
    }

    return sukurtasStringas;
} 

void failuKurimas(string failoPavadinimas) { // Kolizijos
    srand(time(0)); 
    ofstream failiukas(failoPavadinimas); 
    if (!failiukas) {
        cout << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return;
    }

    int poros_ilgis[] = {10, 100, 500, 1000}; 
    int poru_kiekis[] = {25000, 25000, 25000, 25000}; 
    int kolizijuSkaicius = 0; 

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < poru_kiekis[i]; j++) {
            string pirmasIsPoru = randomSimboliuGeneravimas(poros_ilgis[i]);
            string antrasIsPoru = randomSimboliuGeneravimas(poros_ilgis[i]);

            string pirmasHash = hashFunkcija(pirmasIsPoru);
            string antrasHash = hashFunkcija(antrasIsPoru);

            if (pirmasHash == antrasHash) {
                kolizijuSkaicius++; 
            }

            failiukas << pirmasIsPoru << " " << antrasIsPoru << endl;
        }
    }

    failiukas.close();
    cout << "Koliziju skaicius: " << kolizijuSkaicius << endl;
}

void druskosKolizijos(string failoPavadinimas) {
    srand(time(0)); 
    ofstream failiukas(failoPavadinimas); 
    if (!failiukas) {
        cout << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return;
    }

    int poros_ilgis[] = {10, 100, 500, 1000}; 
    int poru_kiekis[] = {25000, 25000, 25000, 25000}; 
    int kolizijuSkaicius = 0;
    int saltKolizijuSkaicius = 0; 

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < poru_kiekis[i]; j++) {
            string pirmasIsPoru = randomSimboliuGeneravimas(poros_ilgis[i]);
            string antrasIsPoru = pirmasIsPoru;

            string pirmasHash = hashFunkcija(pirmasIsPoru);
            string antrasHash = hashFunkcija(antrasIsPoru);

            if (pirmasHash == antrasHash) {
                kolizijuSkaicius++; 
            }
            
            string pirmasHashSalt = hashFunkcijaSuDruska(pirmasIsPoru);
            string antrasHashSalt = hashFunkcijaSuDruska(antrasIsPoru);

            if (pirmasHashSalt == antrasHashSalt) {
                saltKolizijuSkaicius++; 
            }

            failiukas << pirmasIsPoru << " " << antrasIsPoru << endl;
        }
    }

    failiukas.close();
    cout << "Hash koliziju skaicius: " << kolizijuSkaicius << endl;
    cout << "Hash + salt koliziju skaicius: " << saltKolizijuSkaicius << endl;
}

void puzzleFriendlinessKolizijos(string failoPavadinimas, int skaicius) {
    unordered_map<string, string> hashai; // Hash'ams saugoti
    int kolizijuSkaicius = 0;

    ofstream failiukas(failoPavadinimas); 
    if (!failiukas) {
        cout << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return;
    }

    for (int i = 0; i < skaicius; i++) {
        string ivestis = "labas" + to_string(i);
        string ivestiesHash = hashFunkcijaSuDruska(ivestis);

        if (hashai.find(ivestiesHash) != hashai.end()) {
            kolizijuSkaicius++; 
        } else {
            hashai[ivestiesHash] = ivestis; 
        }

        failiukas << ivestis << " " << ivestiesHash << endl; 
    }

    failiukas.close();
    cout << "Koliziju skaicius: " << kolizijuSkaicius << endl;
}

string pakeistiVienasSimboli(string eilute) {
    char simboliai[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int pozicija = rand() % eilute.size(); // Atsitiktineje vietoje
    char naujasSimbolis;

    //Atsitiktinai generuojame nauja simboli naujasSimbolis iš simboliai masyvo, 
    //Taciau dabar is karto patikrina ar naujas simbolis nelygus senajam
    do {
        naujasSimbolis = simboliai[rand() % (sizeof(simboliai) - 1)]; // Keiciamas simbolis
    } while (naujasSimbolis == eilute[pozicija]); 

    eilute[pozicija] = naujasSimbolis; 
    return eilute;
}

bitset<256> hexIBitus(string hexString) {
    bitset<256> bituktukai; 

    for (size_t i = 0; i < hexString.size(); ++i) {
        char hexChar = hexString[i];
        int value;

        switch (hexChar) {
            case '0': value = 0; break;
            case '1': value = 1; break;
            case '2': value = 2; break;
            case '3': value = 3; break;
            case '4': value = 4; break;
            case '5': value = 5; break;
            case '6': value = 6; break;
            case '7': value = 7; break;
            case '8': value = 8; break;
            case '9': value = 9; break;
            case 'a': value = 10; break;
            case 'b': value = 11; break;
            case 'c': value = 12; break;
            case 'd': value = 13; break;
            case 'e': value = 14; break;
            case 'f': value = 15; break;
            case 'A': value = 10; break;
            case 'B': value = 11; break;
            case 'C': value = 12; break;
            case 'D': value = 13; break;
            case 'E': value = 14; break;
            case 'F': value = 15; break;
            default : cout << "Neteisinga reiksme." << endl;
        }

        int startBitIndex = i * 4; // Kiekvienas hex simbolis atitinka 4 bitams binary sistemoje
        bituktukai[startBitIndex + 3] = (value >> 3) & 1; // 8
        bituktukai[startBitIndex + 2] = (value >> 2) & 1; // 4
        bituktukai[startBitIndex + 1] = (value >> 1) & 1; // 2
        bituktukai[startBitIndex]     = (value >> 0) & 1; // 1
    }

    return bituktukai; 
}

void generuotiPoras(string failoPavadinimas) { // Lavinos efektas
    ofstream failiukas(failoPavadinimas); 
    if (!failiukas) {
        cout << "Nepavyko sukurti failo." << endl;
        return;
    }

    vector<int> ilgiai = {10, 100, 500, 1000}; 
    int poruKiekis = 25000; 

    float minBit = 100, maxBit = 0, vidBit = 0;
    float minHex = 100, maxHex = 0, vidHex = 0;

    for (const auto& maxIlgis : ilgiai) {
        for (int i = 0; i < poruKiekis; i++) {
            string pirmas = randomSimboliuGeneravimas(maxIlgis);
            string antras = pakeistiVienasSimboli(pirmas);
            failiukas << pirmas << " " << antras << endl;

            string hash1 = hashFunkcija(pirmas);
            string hash2 = hashFunkcija(antras);

            auto bitai1 = hexIBitus(hash1);
            auto bitai2 = hexIBitus(hash2);
            
            // Hex skirtumas
            size_t hexKiekis = 0;
            for (size_t j = 0; j < hash1.length(); j++) {
                if (hash1[j] != hash2[j])
                    hexKiekis++;
            }
            float hexSkirtumas = (hexKiekis * 100.0) / 64; 
            minHex = min(minHex, hexSkirtumas);
            maxHex = max(maxHex, hexSkirtumas);
            vidHex += hexSkirtumas;

            // Bitu skirtumas
            size_t bitKiekis = 0;
            for (size_t j = 0; j < bitai1.size(); j++) {
                if (bitai1[j] != bitai2[j]) {
                    bitKiekis++; 
                }
            }
            float bitSkirtumas = (bitKiekis * 100.0) / 256; 
            minBit = min(minBit, bitSkirtumas);
            maxBit = max(maxBit, bitSkirtumas);
            vidBit += bitSkirtumas;
        }
    }

    vidBit = vidBit / (poruKiekis * ilgiai.size());
    vidHex = vidHex / (poruKiekis * ilgiai.size());

    cout << "Bit skirtumas. " << endl;
    cout << "Min skirtumas: " << minBit << "%" << endl;
    cout << "Max skirtumas: " << maxBit << "%" << endl;
    cout << "Vid skirtumas: " << vidBit << "%" << endl;
    cout << "----------------------------" << endl;
    cout << "Hex skirtumas. " << endl;
    cout << "Min skirtumas: " << minHex << "%" << endl;
    cout << "Max skirtumas: " << maxHex << "%" << endl;
    cout << "Vid skirtumas: " << vidHex << "%" << endl;

    failiukas.close();
}