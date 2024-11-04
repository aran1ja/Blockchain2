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
#include <omp.h>      
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

class UTXO {
public:
    string vartotojo_duomenys;
    string UTXO_id; 
    int suma; 

    UTXO(const string& vartotojo_duomenys, int suma)
        : vartotojo_duomenys(vartotojo_duomenys), suma(suma) { UTXO_id = hashFunkcija(vartotojo_duomenys + to_string(suma)); }
};

class Vartotojas {
    private:
    string vardas;
    string viesasis_raktas;
    vector <UTXO> utxos;

    public:
    // Konstruktorius
    Vartotojas() = default;
    Vartotojas(const string& vardas, const string& viesasis_raktas) 
        : vardas(vardas), viesasis_raktas(viesasis_raktas) {}

    // Destruktorius
    ~Vartotojas() {}

    // Getteriai ir setteriai
    string getVardas() const { return vardas; }
    string getViesasisRaktas() const { return viesasis_raktas; }
    const vector<UTXO>& GetUtxos() const { return utxos; }

    int GetBalance() const {
        int balansas = 0;
        for (const auto& utxo : utxos) {
            balansas += utxo.suma;
        }
        return balansas;
    }

    void addUTXO(const UTXO& utxo) { utxos.push_back(utxo); }
    void removeUTXO(const string& UTXO_id) {
        auto it = find_if(utxos.begin(), utxos.end(), [&UTXO_id](const UTXO& u) {
            return u.UTXO_id == UTXO_id; });
        if (it != utxos.end()) {
            utxos.erase(it);
        }
    }

    void print() const {
        cout << "Vardas: " << vardas << endl;
        cout << "Viesasis raktas: " << viesasis_raktas << endl;
        cout << fixed << setprecision(2);
        cout << "Balansas: " << GetBalance() << endl;
        cout << "UTXOs: " << endl;
        for (const auto& utxo : utxos) {
            cout << "    " << utxo.UTXO_id << " - " << setprecision(2) << utxo.suma << endl;
        }
    }
};

class Transakcija {
    private:
    string transakcijos_id;
    string siuntejo_viesasis_raktas;
    string gavejo_viesasis_raktas;
    int suma;

    public:
    // Konstruktorius
    Transakcija() = default;
    Transakcija(const string& transakcijos_id, const string& siuntejo_viesasis_raktas, 
                const string& gavejo_viesasis_raktas, int suma) 
        : transakcijos_id(transakcijos_id), siuntejo_viesasis_raktas(siuntejo_viesasis_raktas), 
          gavejo_viesasis_raktas(gavejo_viesasis_raktas), suma(suma) {}
    
    // Destruktorius
    ~Transakcija() {}

    // Getteriai ir setteriai
    string getTransakcijosId() const { return transakcijos_id; }
    string getSiuntejoViesasisRaktas() const { return siuntejo_viesasis_raktas; }
    string getGavejoViesasisRaktas() const { return gavejo_viesasis_raktas; }
    int getSuma() const { return suma; }
};

class Blokas {
    private:
    string bloko_id;
    vector<Transakcija> transakcijos;
    
    string prev_block_hash;     // Ankstesnio bloko maisos reiksme 
    time_t timestamp;           // Laiko zyma 
    int version = 1;            // Blokų grandines duomenu strukturos versija
    string merkle_root;         // Visų bloko transakcijų maišos reikšmė, gauta naudojant Merkle medį
    int nonce;                  // Atsitiktinis skaicius, naudojamas tinkamo sudetingumo bloko maisos reiksmei gauti 
    int difficulty_target = 2;  // Bloko maisos reiksmes sudetingumas 

    public:
    // Konstruktorius
    Blokas() = default;
    Blokas(const string& id, const vector<Transakcija>& trans = {}, int nonce = 0,
           const string& prev_hash = "", const string& merkleRoot = "")
        : bloko_id(id), transakcijos(trans), nonce(nonce), timestamp(time(nullptr)),
        prev_block_hash(prev_hash), merkle_root(merkleRoot) {}

    // Destruktorius
    ~Blokas() {}

    // Getteriai ir setteriai
    string getBlokoId() const { return bloko_id; }
    vector<Transakcija>& getTransakcijos() { return transakcijos; }
    const vector<Transakcija>& getTransakcijos() const { return transakcijos; }
    string getPreviousBlockHash() const { return prev_block_hash; }
    const time_t& getTimestamp() const { return timestamp; }
    int getVersion() const { return version; }
    int getNonce() const { return nonce; }
    void setNonce(int newNonce) { nonce = newNonce; }
    int getDifficultyTarget() const { return difficulty_target; }
    string getMerkleRoot() const { return merkle_root; }

    void setBlokoId(const string& id) { bloko_id = id; }
    void setPreviousBlockHash(const string& prevHash) { prev_block_hash = prevHash; }
    void setTimestamp(time_t ts) { timestamp = ts; }
    void setMerkleRoot(const string& merkleRoot) { merkle_root = merkleRoot; }
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

    /////VARTOTOJAI/////

void generuotiVartotojus(vector<Vartotojas>& vartotojai, ofstream& fail) {
    for (int i = 0; i < 1000; i++) {
        string vardas = randomVardas();
        string viesasis_raktas = hashFunkcija(to_string(i) + vardas);
        
        Vartotojas vartotojas(vardas, viesasis_raktas);

        int utxo_kiekis = rand() % 10 + 1; 
        for (int j = 0; j < utxo_kiekis; j++) {
            int suma = rand() % 5000 + 100; 
            UTXO naujas_utxo(viesasis_raktas, suma);
            vartotojas.addUTXO(naujas_utxo);
        }

        vartotojai.push_back(vartotojas);
    }

    for (const auto& vartotojas : vartotojai) {
        fail << "Vardas: " << vartotojas.getVardas() << endl;
        fail << "Viesasis raktas: " << vartotojas.getViesasisRaktas() << endl;
        fail << "Balansas: " << vartotojas.GetBalance() << endl;
        fail << "UTXOs: " << endl;
        for (const auto& utxo : vartotojas.GetUtxos()) {
            fail << "    " << utxo.UTXO_id << " - " << utxo.suma << endl;
        }
        fail << "" << endl;
    }
    fail.close();
}

    /////TRANSAKCIJOS/////

void generuotiTransakcijas(vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai, ofstream& failas) {
    for (int i = 0; i < 10000; i++) {
        int siuntejas;
        do {
            siuntejas = rand() % vartotojai.size();
        } while (vartotojai[siuntejas].GetBalance() < 1);
        string siuntejo_viesasis_raktas = vartotojai[siuntejas].getViesasisRaktas();

        int gavejas;
        do {
            gavejas = rand() % vartotojai.size();
        } while (siuntejas == gavejas);
        string gavejo_viesasis_raktas = vartotojai[gavejas].getViesasisRaktas();

        int suma = rand() % 5000 + 1; 

        if (suma > vartotojai[siuntejas].GetBalance()) {
            continue; // Jei siuntėjas neturi pakankamai balanso, pereinam prie kitos transakcijos
        }

        // Parenkame UTXO objektus transakcijos sumai padengti
        vector<string> panaudoti_utxo_ids;
        int surinkta_suma = 0;
        for (const auto& utxo : vartotojai[siuntejas].GetUtxos()) {
            panaudoti_utxo_ids.push_back(utxo.UTXO_id);
            surinkta_suma += utxo.suma;
            if (surinkta_suma >= suma) break;
        }

        string transakcijos_id = hashFunkcija(siuntejo_viesasis_raktas + gavejo_viesasis_raktas + to_string(suma));

        // Patikrinkimas, ar transakcijos ID atitinka transakcijos maisos reiksme
        string patikrinimas = hashFunkcija(siuntejo_viesasis_raktas + gavejo_viesasis_raktas + to_string(suma));
        if (transakcijos_id != patikrinimas) {
            cout << "Klaida! Generuotas ID neatitinka transakcijos maisos reiksmes." << endl;
            continue; 
        } 

        // Pasalinamas panaudotus UTXO is siuntejo saraso
        for (const auto& utxo_id : panaudoti_utxo_ids) {
            vartotojai[siuntejas].removeUTXO(utxo_id);
        }

        // Pridedamas naujas UTXO gavejui
        UTXO naujas_utxo(gavejo_viesasis_raktas, suma);
        vartotojai[gavejas].addUTXO(naujas_utxo);

        // Issaugoma transakcijos informacija
        transakcijos.push_back({transakcijos_id, siuntejo_viesasis_raktas, gavejo_viesasis_raktas, suma});
    }

    for (const auto& transakcija : transakcijos) {
        failas << "Transakcijos ID: " << transakcija.getTransakcijosId() << endl;
        failas << "Siuntejo viesasis raktas: " << transakcija.getSiuntejoViesasisRaktas() << endl;
        failas << "Gavejo viesasis raktas: " << transakcija.getGavejoViesasisRaktas() << endl;
        failas << "Suma: " << transakcija.getSuma() << endl;
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
        transakciju_failas << "Transakcijos ID: " << tr.getTransakcijosId() << endl;
        transakciju_failas << "Siuntejo viesasis raktas: " << tr.getSiuntejoViesasisRaktas() << endl;
        transakciju_failas << "Gavejo viesasis raktas: " << tr.getGavejoViesasisRaktas() << endl;
        transakciju_failas << "Suma: " << tr.getSuma() << endl;
        transakciju_failas << "" << endl;
    }
    transakciju_failas.close();
}

    /////BLOKAI/////

const int DifficultyTarget = 2;

pair<string, int> pridetiNonce(const string& id) {
    int nonce = 0;
    string hashas;

    auto start = chrono::high_resolution_clock::now();
    
    do {
        hashas = hashFunkcija(id + to_string(nonce));
        nonce++;

        if (nonce % 10000 == 0) {
            auto finish = chrono::high_resolution_clock::now();
            auto skirtumas = chrono::duration_cast<chrono::seconds>(finish - start).count();
            cout << "Nonce: " << nonce << ", Laikas: " << skirtumas << " s, Hashas: " << hashas << endl;
        }

    } while (hashas.substr(0, DifficultyTarget) != string(DifficultyTarget, '0'));

    return {hashas, nonce};
}

string merkleRoot(const vector<string>& transakcijosID) {
    if (transakcijosID.empty()) return "";

    vector<string> hashiukai = transakcijosID;

    while (hashiukai.size() > 1) {
        vector<string> naujiHashiukai;

        for (size_t i = 0; i < hashiukai.size(); i += 2) {
            if (i + 1 < hashiukai.size()) {
                string sujungtasHash = hashiukai[i] + hashiukai[i + 1];
                naujiHashiukai.push_back(hashFunkcija(sujungtasHash));
            } else {
                naujiHashiukai.push_back(hashiukai[i]);
            }
        }

        hashiukai = naujiHashiukai; 
    }

    return hashiukai[0]; 
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

            vector<string> transakcijuID;
            for (const auto& tr : isrinktos_transakcijos) {
                transakcijuID.push_back(tr.getTransakcijosId());
            }

            string merkle_root = merkleRoot(transakcijuID);
            string prev_hash = blokai.empty() ? "" : blokai.back().getBlokoId(); 

            Blokas naujas_blokas(merkle_root, isrinktos_transakcijos, 0);
            naujas_blokas.setMerkleRoot(merkle_root);
            naujas_blokas.setPreviousBlockHash(prev_hash);

            pair<string, int> result = pridetiNonce(naujas_blokas.getBlokoId());
            naujas_blokas.setBlokoId(result.first); 
            naujas_blokas.setNonce(result.second);
            
            //pridetiNonce(naujas_blokas);
            failiukas << "Iskastas blokas " << (blokai.size() + 1) << endl;

            time_t timestamp = naujas_blokas.getTimestamp();

            blokai.push_back(naujas_blokas);
            
            failiukas << "Bloko ID: " << naujas_blokas.getBlokoId() << endl;
            failiukas << "Previous Block Hash: " << naujas_blokas.getPreviousBlockHash() << endl;
            failiukas << "Timestamp: " << ctime(&timestamp);
            failiukas << "Version: " << naujas_blokas.getVersion() << endl;
            failiukas << "Merkle Root: " << naujas_blokas.getMerkleRoot() << endl;
            failiukas << "Nonce: " << naujas_blokas.getNonce() << endl;
            failiukas << "Difficulty Target: " << naujas_blokas.getDifficultyTarget() << endl;
            failiukas << "Transakcijos: " << endl;
            failiukas << "_______________________________________________________________________________________" << endl;
            failiukas << " " << endl;
            for (const auto& tr : naujas_blokas.getTransakcijos()) {
                failiukas << "Transakcijos ID: " << tr.getTransakcijosId() << endl;
                failiukas << "Siuntejo viesasis raktas: " << tr.getSiuntejoViesasisRaktas() << endl;
                failiukas << "Gavejo viesasis raktas: " << tr.getGavejoViesasisRaktas() << endl;
                failiukas << "Suma: " << tr.getSuma() << endl;
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
        balansu_failas << "Vardas: " << vartotojas.getVardas() << endl;
        balansu_failas << "Viesasis raktas: " << vartotojas.getViesasisRaktas() << endl;
        balansu_failas << "Balansas: " << vartotojas.GetBalance() << endl;
        balansu_failas << "" << endl;
    }
    balansu_failas.close();
}

void atnaujintiBalansus(vector<Vartotojas>& vartotojai, vector<Blokas>& blokai) {
    unordered_map<string, int> vartotojuIndexai;

    for (size_t i = 0; i < vartotojai.size(); i++) {
        vartotojuIndexai[vartotojai[i].getViesasisRaktas()] = i;
    }

    for (auto& blokas : blokai) {
        auto& transakcijos = blokas.getTransakcijos();

        for (auto it = transakcijos.begin(); it != transakcijos.end(); ) {
            auto itSiuntejas = vartotojuIndexai.find(it->getSiuntejoViesasisRaktas());
            auto itGavejas = vartotojuIndexai.find(it->getGavejoViesasisRaktas());

            if (itSiuntejas != vartotojuIndexai.end() && itGavejas != vartotojuIndexai.end()) {
                int siuntejasIndex = (*itSiuntejas).second; 
                int gavejasIndex = (*itGavejas).second;

                if (vartotojai[siuntejasIndex].GetBalance() >= it->getSuma()) {
                    vartotojai[siuntejasIndex].setBalansas(vartotojai[siuntejasIndex].GetBalance() - it->getSuma()); 
                    vartotojai[gavejasIndex].setBalansas(vartotojai[gavejasIndex].GetBalance() + it->getSuma()); 
                    it++;
                } else {
                    cout << "Klaida! Siuntejo " << vartotojai[siuntejasIndex].getVardas() << ", viesasis raktas: " << it->getSiuntejoViesasisRaktas() 
                    << " nepakankamas balansas: " << vartotojai[siuntejasIndex].GetBalance() << ". Suma: " << it->getSuma() << endl;
                    it = transakcijos.erase(it);
                }
            } else {
                cout << "Klaida: Nezinomas siuntejas ar gavejas." << endl;
            }
        }
    }
    issaugotiBalansus(vartotojai);
}

void rastiTransakcija(const vector<Transakcija>& transakcijos, const vector<Blokas>& blokai, const string& id) {
    auto it = find_if(transakcijos.begin(), transakcijos.end(), [&](const Transakcija& t) {
        return t.getTransakcijosId() == id;
    });

    if (it != transakcijos.end()) {
        const Transakcija& transakcija = *it; 
        cout << "Transakcijos ID: " << transakcija.getTransakcijosId() << endl;
        cout << "Siuntejo viesasis raktas: " << transakcija.getSiuntejoViesasisRaktas() << endl;
        cout << "Gavejo viesasis raktas: " << transakcija.getGavejoViesasisRaktas() << endl;
        cout << "Suma: " << transakcija.getSuma() << endl << endl;
        return; 
    }

    for (const auto& blokas : blokai) {
        auto tr_it = find_if(blokas.getTransakcijos().begin(), blokas.getTransakcijos().end(), [&](const Transakcija& t) { // Use getter
            return t.getTransakcijosId() == id; 
        });
        if (tr_it != blokas.getTransakcijos().end()) {
            const Transakcija& transakcija = *tr_it; 
            cout << "Bloko ID: " << blokas.getBlokoId() << endl; 
            cout << "Transakcijos ID: " << transakcija.getTransakcijosId() << endl;
            cout << "Siuntejo viesasis raktas: " << transakcija.getSiuntejoViesasisRaktas() << endl;
            cout << "Gavejo viesasis raktas: " << transakcija.getGavejoViesasisRaktas() << endl;
            cout << "Suma: " << transakcija.getSuma() << endl << endl;
            return; 
        }
    }
    cout << "Transakcija su tokiu ID nerasta." << endl;
}

void rastiBloka(const vector<Blokas>& blokai, const string& id) {
    auto it = find_if(blokai.begin(), blokai.end(), [&](const Blokas& b) {
        return b.getBlokoId() == id;
    });
    if (it != blokai.end()) {
        const Blokas& blokas = *it; 
        cout << "Bloko ID: " << blokas.getBlokoId() << endl;
        cout << "Previous Block Hash: " << blokas.getPreviousBlockHash() << endl; 
        cout << "Timestamp: " << ctime(&blokas.getTimestamp()); 
        cout << "Version: " << blokas.getVersion() << endl; 
        cout << "Merkle Root: " << blokas.getMerkleRoot() << endl;
        cout << "Nonce: " << blokas.getNonce() << endl;
        cout << "Difficulty Target: " << blokas.getDifficultyTarget() << endl; 
        /*cout << "Transakcijos:" << endl;
        for (const auto& tr : blokas.getTransakcijos()) {
            cout << "  Transakcijos ID: " << tr.getTransakcijosId() << endl;
            cout << "  Siuntejo viesasis raktas: " << tr.getSiuntejoViesasisRaktas() << endl;
            cout << "  Gavejo viesasis raktas: " << tr.getGavejoViesasisRaktas() << endl;
            cout << "  Suma: " << tr.getSuma() << endl;
            cout << " " << endl;
        }*/
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
/*
pair<string, int> pridetiNonca(const string& id, int laikoLimitas, int bandymuLimitas) {
    int nonce = 0;
    string hashas;
    auto start = chrono::high_resolution_clock::now();

    while (nonce < bandymuLimitas) {
        hashas = hashFunkcija(id + to_string(nonce));
        
        if (hashas.substr(0, 3) == "000") {
            return {hashas, nonce};
        }
        
        nonce++;
        auto dabartinisLaikas = chrono::high_resolution_clock::now();
        auto skirtumas = chrono::duration_cast<chrono::seconds>(dabartinisLaikas - start).count();

        if (skirtumas >= laikoLimitas) {
            cout << "Nepavyko iskasti bloko per " << skirtumas << " sekundes. Bandymu skaicius: " << nonce << endl;
            return {"", -1};  // Nesekme
        }
    }
    return {"", -1}; 
}

bool bandytiKastiBloka(Blokas& blokas, int laikoLimitas, int bandymuLimitas) {
    pair<string, int> result = pridetiNonca(blokas.getBlokoId(), laikoLimitas, bandymuLimitas);
    if (result.second != -1) {  
        blokas.setBlokoId(result.first);
        blokas.setNonce(result.second);
        cout << "Blokas sekmingai iskastas!" << endl;
        return true;
    }
    return false;  
}

void generuotiPotencialiusBlokus(vector<Blokas>& potencialusBlokai, vector<Transakcija>& transakcijos) {
    for (int i = 0; i < 5; i++) {
        vector<Transakcija> isrinktos_transakcijos;
        for (int j = 0; j < 100; j++) {
            int randomIndex = rand() % transakcijos.size();
            isrinktos_transakcijos.push_back(transakcijos[randomIndex]);
        }
        
        vector<string> transakcijuID;
        for (const auto& tr : isrinktos_transakcijos) {
            transakcijuID.push_back(tr.getTransakcijosId());
        }
        
        string merkle_root = merkleRoot(transakcijuID);
        Blokas kandidatas(merkle_root, isrinktos_transakcijos, 0);
        potencialusBlokai.push_back(kandidatas);
    }
}

void generuotiBlokeliusKandidatus(vector<Blokas>& blokai, vector<Transakcija>& transakcijos, ofstream& failiukas, int laikoLimitas = 5, int bandymuLimitas = 100000) {
    vector<Blokas> potencialusBlokai;
    generuotiPotencialiusBlokus(potencialusBlokai, transakcijos);

    omp_set_num_threads(2); 

    bool iskasta = false;

    #pragma omp parallel for
    for (int i = 0; i < 5; ++i) {
        bool found = bandytiKastiBloka(potencialusBlokai[i], laikoLimitas, bandymuLimitas);

    
        cout << "Bandoma kasti bloka-kandidata Nr. " << (i + 1) << endl;
        if (found) {
            #pragma omp critical
            {
                blokai.push_back(potencialusBlokai[i]);
                failiukas << "Iskastas blokas " << blokai.size() << endl;
                iskasta = true; 
            }
        }
    }

    // Patikrinkite, ar kas nors buvo rastas
    #pragma omp single
    {
        if (!iskasta) {
            laikoLimitas *= 2;
            bandymuLimitas *= 2;
            cout << "Nepavyko iskasti jokio bloko. Pailginamas laiko limitas iki " << laikoLimitas
                 << " sekundziu ir bandymu limitas iki " << bandymuLimitas << "." << endl;

            generuotiBlokeliusKandidatus(blokai, transakcijos, failiukas, laikoLimitas, bandymuLimitas);
        }
    }
}
*/