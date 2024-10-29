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

class Vartotojas {
    public:
    string vardas;
    string viesasis_raktas;
    int balansas;

    // Konstruktorius
    Vartotojas(const string& vardas, const string& viesasis_raktas, int balansas) 
        : vardas(vardas), viesasis_raktas(viesasis_raktas), balansas(balansas) {}

};

class Transakcija {
    public:
    string transakcijos_id;
    string siuntejo_viesasis_raktas;
    string gavejo_viesasis_raktas;
    int suma;

    // Konstruktorius
    Transakcija(const string& transakcijos_id, const string& siuntejo_viesasis_raktas, 
                const string& gavejo_viesasis_raktas, int suma) 
        : transakcijos_id(transakcijos_id), siuntejo_viesasis_raktas(siuntejo_viesasis_raktas), 
          gavejo_viesasis_raktas(gavejo_viesasis_raktas), suma(suma) {}
};

class Blokas {
public:
    string bloko_id;
    vector<Transakcija> transakcijos;
    int nonce;

    // Konstruktorius
    Blokas(const string& id, const vector<Transakcija>& trans = {}, int nonce = 0)
        : bloko_id(id), transakcijos(trans), nonce(nonce) {}
};

string randomVardas() {
    vector<string> var = {"Tomas", "Andrius", "Algirdas", "Lukas", "Povilas", "Vilius", "Justinas", "Martynas", "Kestutis", "Juozas",
    "Julius", "Rimvydas", "Darius", "Vytis", "Egidijus", "Karolis", "Simonas", "Deividas", "Dominykas", "Edgaras", "Aurimas", "Ricardas", 
    "Paulius", "Arvydas", "Antanas", "Jurgis", "Adomas", "Gediminas", "Arturas", "Eimantas", "Raimondas", "Arnas", "Eugenijus", "Kostas", 
    "Birute", "Irena", "Rasa", "Asta", "Lina", "Diana", "Jolanta", "Agne", "Egle", "Inga", "Dovile", "Gabija", "Greta", "Monika", "Vaida", 
    "Viktorija", "Sandra", "Aurelija", "Rima", "Laima", "Neringa", "Renata", "Justina", "Kristina", "Agne", "Ieva", "Indre", "Marija"};

    vector<string> pav = {"Kazlauskas", "Petrauskas", "Jankauskas", "Zukauskas", "Pavardenis", "Butkus", "Vasiliauskas",
    "Urbonas", "Jonaitis", "Brazaitis", "Balciunas", "Tamulionis", "Kairys", "Saulys", "Grigas", "Vaitkus", "Stankevicius", "Dauksys", 
    "Navickas", "Zilinskas", "Mockus", "Sernas", "Leipus","Vilkas", "Kavaliauskas", "Rimkus", "Matulis", "Sakalauskas", "Girdvainis", "Simutis"};
    
    string vard = var[rand() % var.size()];
    string pavard = pav[rand() % pav.size()];
    return vard + " " + pavard;
}

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
        if (bitai[i] == bitai[binarinis_ilgis - i - 1]) { 
            bitai[i] = 1;
        } else {
            bitai[i] = 0; 
        }
    }
}

string hashFunkcija(string simboliu_seka) {
        int ascii_suma = 0; 
        int daugiklis = 1; 
    
        int ilgis = 8 * simboliu_seka.size(); 

        // Zodis isverciamas i ASCII
        for (char simbolis : simboliu_seka) {
            ascii_suma += (int)simbolis * daugiklis;
            daugiklis++;
        }

        // Ascii_suma isverciama i binary
        bitset<256> binarinis_kodas((int)ascii_suma);
        for (int i = 0; i < 3; i++)
        {
            bitaiKeiciasiVietomis(binarinis_kodas);
        }
        
        string hexKodas = hexPadarymas(binarinis_kodas);
        //cout << "Hash kodo atvaizdavimas hex pavidalu: " << hex << hexKodas << endl;
        return hexKodas;
}

    /////VARTOTOJAI/////

void generuotiVartotojus(vector<Vartotojas>& vartotojai, ofstream& fail) {
    for (int i = 0; i < 1000; i++) {
        string vardas = randomVardas();
        int balansas = rand() % 1000000 + 100;
        string viesasis_raktas = hashFunkcija(vardas + to_string(i) + to_string(balansas));
        
        vartotojai.push_back({vardas, viesasis_raktas, balansas});
    }

    for (const auto& vartotojas : vartotojai) {
        fail << "Vardas: " << vartotojas.vardas << endl;
        fail << "Viesasis raktas: " << vartotojas.viesasis_raktas << endl;
        fail << "Balansas: " << vartotojas.balansas << endl;
        fail << "" << endl;
    }
    fail.close();
}

    /////TRANSAKCIJOS/////

void generuotiTransakcijas(vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai, ofstream& failas) {
    for (int i = 0; i < 10000; i++) {
        
        // Siuntejas ir jo viesasis raktas
        int siuntejas;
        do {
            siuntejas = rand() % vartotojai.size();
        } while (vartotojai[siuntejas].balansas < 1);

        string siuntejo_viesasis_raktas = vartotojai[siuntejas].viesasis_raktas;

        // Gavejas ir jo viesasis raktas
        int gavejas;
        do {
            gavejas = rand() % vartotojai.size();
        } while (siuntejas == gavejas);

        string gavejo_viesasis_raktas = vartotojai[gavejas].viesasis_raktas;

        // Suma, kuri yra pervedama
        int suma;
        suma = rand() % 10000 + 1; 

        // Transakcijos unikalusis kodas
        string transakcijos_id = hashFunkcija(siuntejo_viesasis_raktas + gavejo_viesasis_raktas + to_string(suma));

        // Patikriname, ar uztenka siuntejo balanso transakcijai vykdyti
        if (suma > vartotojai[siuntejas].balansas || vartotojai[siuntejas].balansas <= 0) {
            cout << "Klaida!" << endl;
            cout << "Transakcijos ID: " << transakcijos_id << endl;
            cout << "Siuntejo viesasis raktas: " << siuntejo_viesasis_raktas << endl;
            cout << "Gavejo viesasis raktas: " << gavejo_viesasis_raktas << endl;
            cout << "Suma: " << suma << endl; 
            cout << "Siuntejo balanso nepakanka transakcijai vykdyti." << endl;
            cout << "" << endl;
            continue; 
        }

        transakcijos.push_back({transakcijos_id, siuntejo_viesasis_raktas, gavejo_viesasis_raktas, suma});
    
        //vartotojai[siuntejas].balansas -= suma; 
        //vartotojai[gavejas].balansas += suma;
    }

    for (const auto& transakcija : transakcijos) {
        failas << "Transakcijos ID: " << transakcija.transakcijos_id << endl;
        failas << "Siuntejo viesasis raktas: " << transakcija.siuntejo_viesasis_raktas << endl;
        failas << "Gavejo viesasis raktas: " << transakcija.gavejo_viesasis_raktas << endl;
        failas << "Suma: " << transakcija.suma << endl;
        failas << "" << endl;
    }
    failas.close();
}

void filtruotiTransakcijas(vector<Transakcija>& visos, vector<Transakcija>& pasirinktos) {
    for (auto it = visos.begin(); it != visos.end() && pasirinktos.size() < 100;) {
        pasirinktos.push_back(*it);
        it = visos.erase(it); 
    }
}

void atnaujintiTransakcijuFaila(const vector<Transakcija>& likusios_transakcijos) {
    ofstream transakciju_failas("Transakcijos.txt");
    for (const auto& tr : likusios_transakcijos) {
        transakciju_failas << "Transakcijos ID: " << tr.transakcijos_id << endl;
        transakciju_failas << "Siuntejo viesasis raktas: " << tr.siuntejo_viesasis_raktas << endl;
        transakciju_failas << "Gavejo viesasis raktas: " << tr.gavejo_viesasis_raktas << endl;
        transakciju_failas << "Suma: " << tr.suma << endl;
        transakciju_failas << "" << endl;
    }
    transakciju_failas.close();
}

    /////BLOKAI/////

const int DifficultyTarget = 2;

int pridetiNonce(Blokas& blokas) {
    int nonce = 0;
    string hashas;
    do {
        nonce++;
        hashas = hashFunkcija(blokas.bloko_id + to_string(nonce));
    } while (hashas.substr(0, DifficultyTarget) != string(DifficultyTarget, '0'));

    blokas.nonce = nonce;
    blokas.bloko_id = hashas;
    return nonce;
}

void generuotiBlokus(vector<Blokas>& blokai, vector<Transakcija>& transakcijos, ofstream& failiukas) {
    char pasirinkimas;

    do {
        cout << "Liko " << transakcijos.size() << " transakciju." << endl;
        cout << "Ar norite sukurti nauja bloka? (t/n): ";
        cin >> pasirinkimas;

        if (pasirinkimas == 't' && !transakcijos.empty()) {
            vector<Transakcija> isrinktos_transakcijos;

            filtruotiTransakcijas(transakcijos, isrinktos_transakcijos);

            string sujungtasTransakcijuID;
            for (const auto& tr : isrinktos_transakcijos) {
                sujungtasTransakcijuID += tr.transakcijos_id;
            }

            Blokas naujas_blokas(hashFunkcija(sujungtasTransakcijuID), isrinktos_transakcijos, 0);
            naujas_blokas.transakcijos = isrinktos_transakcijos;
            

            failiukas << "Iskastas blokas " << (blokai.size() + 1) << endl;

            pridetiNonce(naujas_blokas);

            // Pridedame naujus blokus i bloku sarasa
            blokai.push_back(naujas_blokas);
            
            failiukas << "Bloko ID: " << naujas_blokas.bloko_id << endl;
            failiukas << "Nonce: " << naujas_blokas.nonce << endl;
            failiukas << "Transakcijos: " << endl;
            failiukas << "_______________________________________________________________________________________" << endl;
            failiukas << " " << endl;
            for (const auto& tr : naujas_blokas.transakcijos) {
                failiukas << "Transakcijos ID: " << tr.transakcijos_id << endl;
                failiukas << "Siuntejo viesasis raktas: " << tr.siuntejo_viesasis_raktas << endl;
                failiukas << "Gavejo viesasis raktas: " << tr.gavejo_viesasis_raktas << endl;
                failiukas << "Suma: " << tr.suma << endl;
                failiukas << "" << endl;
            }

            atnaujintiTransakcijuFaila(transakcijos);
        }

    } while (pasirinkimas == 't' && !transakcijos.empty());

    failiukas.close();
}

void issaugotiBalansus(vector<Vartotojas>& vartotojai) {
    ofstream balansu_failas("NaujiVartotojuBalansai.txt");
    for (const auto& vartotojas : vartotojai) {
        balansu_failas << "Vardas: " << vartotojas.vardas << endl;
        balansu_failas << "Viesasis raktas: " << vartotojas.viesasis_raktas << endl;
        balansu_failas << "Balansas: " << vartotojas.balansas << endl;
        balansu_failas << "" << endl;
    }
    balansu_failas.close();
}

void atnaujintiBalansus(vector<Vartotojas>& vartotojai, const vector<Blokas>& blokai) {
    unordered_map<string, int> vartotojuIndexai;
    for (size_t i = 0; i < vartotojai.size(); ++i) {
        vartotojuIndexai[vartotojai[i].viesasis_raktas] = i;
    }

    for (const auto& blokas : blokai) {
        for (const auto& tr : blokas.transakcijos) {
            int siuntejas = vartotojuIndexai[tr.siuntejo_viesasis_raktas];
            int gavejas = vartotojuIndexai[tr.gavejo_viesasis_raktas];

            vartotojai[siuntejas].balansas -= tr.suma;
            vartotojai[gavejas].balansas += tr.suma;
        }
    }

    issaugotiBalansus(vartotojai);
}

void rastiTransakcija(const vector<Transakcija>& transakcijos, const vector<Blokas>& blokai, const string& id) {
    auto it = find_if(transakcijos.begin(), transakcijos.end(), [&](const Transakcija& t) {
        return t.transakcijos_id == id;
    });

    if (it != transakcijos.end()) {
        const Transakcija& transakcija = *it; 
        cout << "Transakcijos ID: " << transakcija.transakcijos_id << endl;
        cout << "Siuntejo viesasis raktas: " << transakcija.siuntejo_viesasis_raktas << endl;
        cout << "Gavejo viesasis raktas: " << transakcija.gavejo_viesasis_raktas << endl;
        cout << "Suma: " << transakcija.suma << endl << endl;
        return; 
    }

    for (const auto& blokas : blokai) {
        auto tr_it = find_if(blokas.transakcijos.begin(), blokas.transakcijos.end(), [&](const Transakcija& t) {
            return t.transakcijos_id == id;
        });
        if (tr_it != blokas.transakcijos.end()) {
            const Transakcija& transakcija = *tr_it; 
            cout << "Bloko ID: " << blokas.bloko_id << endl; 
            cout << "Transakcijos ID: " << transakcija.transakcijos_id << endl;
            cout << "Siuntejo viesasis raktas: " << transakcija.siuntejo_viesasis_raktas << endl;
            cout << "Gavejo viesasis raktas: " << transakcija.gavejo_viesasis_raktas << endl;
            cout << "Suma: " << transakcija.suma << endl << endl;
            return; 
        }
    }
    cout << "Transakcija su tokiu ID nerasta." << endl;
}

void rastiBloka(const vector<Blokas>& blokai, const string& id) {
    auto it = find_if(blokai.begin(), blokai.end(), [&](const Blokas& b) {
        return b.bloko_id == id;
    });
    if (it != blokai.end()) {
        const Blokas& blokas = *it; 
        cout << "Bloko ID: " << blokas.bloko_id << endl;
        cout << "Nonce: " << blokas.nonce << endl;
        cout << "Transakcijos:" << endl;
        for (const auto& tr : blokas.transakcijos) {
            cout << "  Transakcijos ID: " << tr.transakcijos_id << endl;
            cout << "  Siuntejo viesasis raktas: " << tr.siuntejo_viesasis_raktas << endl;
            cout << "  Gavejo viesasis raktas: " << tr.gavejo_viesasis_raktas << endl;
            cout << "  Suma: " << tr.suma << endl;
            cout << " " << endl;
        }
    } else {
        cout << "Blokas su tokiu ID nerastas." << endl;
    }
}

void ekranas(const vector<Blokas>& blokai, const vector<Transakcija>& transakcijos) {
        int pasirinkimas; 

        while (true) {
        cout << "Pasirinkite viena veiksma is nurodytu zemiau." << endl;
        cout << "1. Ieskoti transakcijos pagal ID." << endl;
        cout << "2. Ieskoti bloko pagal ID." << endl;
        cout << "Bet koks kitas sakicius baigia programa." << endl;
        cout << "Jusu pasirinkimas: "; cin >> pasirinkimas;

        if (pasirinkimas == 1) {
            cout << "Iveskite transakcijos ID: ";
            string transakcijos_id;
            cin >> transakcijos_id;
            rastiTransakcija(transakcijos, blokai, transakcijos_id);
        } else if (pasirinkimas == 2) {
            cout << "Iveskite bloko ID: ";
            string bloko_id;
            cin >> bloko_id;
            rastiBloka(blokai, bloko_id);
        } else break;    
    }
}