#include "Blokai.h"
using namespace std;

int main() {
    srand(time(0));
    vector<Vartotojas> vartotojai;
    vector<Transakcija> transakcijos;
    vector<Blokas> blokai;
    char pasirinkimas;
    int rinktis;

    cout << "Ar norite generuoti naujus vartotojus ir transakcijas? (t/n): ";
    cin >> pasirinkimas;

    if (pasirinkimas == 't') {
        ofstream fail("Vartotojai.txt");
        ofstream failas("Transakcijos.txt");
        ofstream failiukas("Blokai.txt");

        cout << "Generuojami vartotojai..." << endl;
        generuotiVartotojus(vartotojai, fail);
        cout << "Generuojamos transakcijos..." << endl;
        generuotiTransakcijas(transakcijos, vartotojai, failas);
        cout << "Ar norite kasti blokus (1) ar imituoti 'decentralizuota' bloku kasima (2)? " << endl;
        cout << "Spauskite atitinkama skaiciu." << endl;
        cout << "Jusu pasirinkimas: "; cin >> rinktis;
        if (rinktis == 1) {
            cout << "Bloku generavimas prasideda..." << endl;
            cout << "Galima is kart paspausti keleta 't', kad iskasti keleta bloku." << endl;
            generuotiBlokus(blokai, transakcijos, failiukas);
            cout << "Atnaujinami balansai..." << endl;
            atnaujintiBalansus(vartotojai, blokai);
            ekranas(blokai, transakcijos);
        } else if (rinktis == 2) {
            cout << "Imituojamas 'decentralizuotas' bloku kasimas..." << endl;
            generuotiBlokeliusKandidatus(blokai, transakcijos, failiukas);
            cout << "Atnaujinami balansai..." << endl;
            atnaujintiBalansus(vartotojai, blokai);
        }
        
        fail.close();
        failas.close();
        failiukas.close();
    } 
    return 0;
}