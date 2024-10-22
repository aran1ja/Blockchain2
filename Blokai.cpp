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

int main() {
    ofstream fail("Vartotojai.txt");
    ofstream failas("Transakcijos.txt");
    srand(time(0));
    
    /////VARTOTOJAI/////

    vector<Vartotojas> vartotojai;
    
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

    /////TRANSAKCIJOS/////

    vector<Transakcija> transakcijos;

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
        int suma = rand() % (static_cast<int>(vartotojai[siuntejas].balansas) - 100 + 1) + 100;

        // Transakcijos unikalusis kodas
        string transakcijos_id = hashFunkcija(siuntejo_viesasis_raktas + gavejo_viesasis_raktas + to_string(suma));

        transakcijos.push_back({transakcijos_id, siuntejo_viesasis_raktas, gavejo_viesasis_raktas, suma});
    }

    for (const auto& transakcija : transakcijos) {
        failas << "Transakcijos ID: " << transakcija.transakcijos_id << endl;
        failas << "Siuntejo viesasis raktas: " << transakcija.siuntejo_viesasis_raktas << endl;
        failas << "Gavejo viesasis raktas: " << transakcija.gavejo_viesasis_raktas << endl;
        failas << "Suma: " << transakcija.suma << endl;
        failas << "" << endl;
    }

    fail.close();
    failas.close();

    return 0;
}