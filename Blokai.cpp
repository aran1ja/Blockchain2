#include "ManoHash.h"
using namespace std;

struct Vartotojas {
    string vardas;
    string viesasis_raktas;
    int balansas;
};

struct Transakcija {
    string transakcijos_id;
    string siuntejo_viesasis_raktas;
    string gavejo_viesasis_raktas;
    int suma;
};

struct Blokas {
    string bloko_id;
    vector<Transakcija> transakcijos;
    int nonce;
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
        string viesasis_raktas = hashFunkcija(vardas + to_string(i));
        int balansas = rand() % 1000000 + 100;
        
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
        int suma = rand() % 10000 + 1;

        // Transakcijos unikalusis kodas
        string transakcijos_id = hashFunkcija(siuntejo_viesasis_raktas + gavejo_viesasis_raktas + to_string(suma));

        // Patikriname, ar uztenka siuntejo balanso transakcijai vykdyti
        if (suma > vartotojai[siuntejas].balansas) {
            cout << "Klaida!" << endl;
            cout << "Transakcijos ID: " << transakcijos_id << endl;
            cout << "Siuntejo viesasis raktas: " << siuntejo_viesasis_raktas << endl;
            cout << "Gavejo viesasis raktas: " << gavejo_viesasis_raktas << endl;
            cout << "Suma: " << suma << endl; 
            cout << "Siuntejo balanso nepakanka transakcijai vykdyti." << endl;
            cout << "" << endl;
            continue; 
        }

        // Pakeiciame siunteju ir gaveju balansus
        vartotojai[siuntejas].balansas -= suma;
        vartotojai[gavejas].balansas += suma;

        transakcijos.push_back({transakcijos_id, siuntejo_viesasis_raktas, gavejo_viesasis_raktas, suma});
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

    /////BLOKAI/////

void generuotiBlokus(vector<Blokas>& blokai, vector<Transakcija>& transakcijos, ofstream& failiukas) {
    vector<Transakcija> isrinktos_transakcijos;
    vector<string> transakciju_unikalus_kodas;
    
    for (int i = 0; i < 100; i++) {
        int random100transakciju = rand() % transakcijos.size();
        isrinktos_transakcijos.push_back(transakcijos[random100transakciju]);
        transakciju_unikalus_kodas.push_back(transakcijos[random100transakciju].transakcijos_id);
    }

    string sujungtasTransakcijuID;
    for (const auto& id : transakciju_unikalus_kodas) {
        sujungtasTransakcijuID += id;
    }

    Blokas naujas_blokas;
    naujas_blokas.bloko_id = hashFunkcija(sujungtasTransakcijuID);
    naujas_blokas.transakcijos = isrinktos_transakcijos;

    // Pridedame naujus blokus i bloku sarasa
    blokai.push_back(naujas_blokas);

    for (const auto& blokas : blokai) {
        failiukas << "Bloko ID: " << blokas.bloko_id << endl;
        failiukas << "Transakcijos: " << endl;
        failiukas << "_______________________________________________________________________________________" << endl;
        failiukas << " " << endl;
        for (const auto& tr : blokas.transakcijos) {
            failiukas << "Transakcijos ID: " << tr.transakcijos_id << endl;
            failiukas << "Siuntejo viesasis raktas: " << tr.siuntejo_viesasis_raktas << endl;
            failiukas << "Gavejo viesasis raktas: " << tr.gavejo_viesasis_raktas << endl;
            failiukas << "Suma: " << tr.suma << endl;
            failiukas << "" << endl;
        }
    }
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

// NUSKAITOMI VARTOTOJAI //
void nuskaitytiVartotojus(vector<Vartotojas>& vartotojai) {
    ifstream fail("Vartotojai.txt");
    if (!fail.is_open()) {
        cout << "Nepavyko atidaryti 'Vartotojai.txt' failo." << endl;
        return;
    }

    Vartotojas vartotojas;
    while (getline(fail, vartotojas.vardas)) {
        getline(fail, vartotojas.viesasis_raktas);
        fail >> vartotojas.balansas;
        fail.ignore();
        vartotojai.push_back(vartotojas);
    }

    fail.close();
}

// NUSKAITOMOS TRANSAKCIJOS //
void nuskaitytiTransakcijas(vector<Transakcija>& transakcijos) {
    ifstream fail("Transakcijos.txt");
    if (!fail.is_open()) {
        cout << "Nepavyko atidaryti 'Transakcijos.txt' failo." << endl;
        return;
    }

    Transakcija transakcija;
    while (getline(fail, transakcija.transakcijos_id)) {
        getline(fail, transakcija.siuntejo_viesasis_raktas);
        getline(fail, transakcija.gavejo_viesasis_raktas);
        fail >> transakcija.suma;
        fail.ignore();
        transakcijos.push_back(transakcija);
    }

    fail.close();
}

// NUSKAITOMI BLOKAI //
void nuskaitytiBlokus(vector<Blokas>& blokai) {
    ifstream fail("Blokai.txt");
    if (!fail.is_open()) {
        cout << "Nepavyko atidaryti 'Blokai.txt' failo." << endl;
        return;
    }

    Blokas blokas;
    while (getline(fail, blokas.bloko_id)) {
        string line;
        while (getline(fail, line) && !line.empty()) {
            Transakcija transakcija;
            transakcija.transakcijos_id = line;
            getline(fail, transakcija.siuntejo_viesasis_raktas);
            getline(fail, transakcija.gavejo_viesasis_raktas);
            fail >> transakcija.suma;
            fail.ignore();
            blokas.transakcijos.push_back(transakcija);
        }
        blokai.push_back(blokas);
    }

    fail.close();
}

const int DifficultyTarget = 2;

int pridetiNonce(Blokas& blokas) {
    int nonce = 0;
    string hashas;
    if (int i = 0; i < DifficultyTarget, i++) {
        nonce++;
        hashas = hashFunkcija(blokas.bloko_id + to_string(nonce));
    }

    blokas.nonce = nonce;
    blokas.bloko_id = hashas;
    return nonce;
}

int main() {
    srand(time(0));
    vector<Vartotojas> vartotojai;
    vector<Transakcija> transakcijos;
    vector<Blokas> blokai;
    char pasirinkimas;
    
    cout << "Ar norite generuoti naujus vartotojus, transakcijas ir blokus? (t/n): ";
    cin >> pasirinkimas;

    // Blogai rodo naujus balansus jeigu leisti dar karta ta pati
    if (pasirinkimas == 't') {
        ofstream fail("Vartotojai.txt");
        ofstream failas("Transakcijos.txt");
        ofstream failiukas("Blokai.txt");

        generuotiVartotojus(vartotojai, fail);
        generuotiTransakcijas(transakcijos, vartotojai, failas);
        generuotiBlokus(blokai, transakcijos, failiukas);
        issaugotiBalansus(vartotojai);

        fail.close();
        failas.close();
        failiukas.close();
    } else {
        cout << "Naudojami esami failai (Vartotojai.txt, Transakcijos.txt, Blokai.txt)." << endl;
        nuskaitytiVartotojus(vartotojai);
        nuskaitytiTransakcijas(transakcijos);
        nuskaitytiBlokus(blokai);
        issaugotiBalansus(vartotojai);
    }

    return 0;
}