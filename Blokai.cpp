#include "ManoHash.h"
using namespace std;

struct Vartotojas {
    string vardas;
    string viesasis_raktas;
    int balansas;
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
    srand(time(0));
    
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

    fail.close();
    return 0;
}