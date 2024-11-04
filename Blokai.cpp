#include "Blokai.h"
using namespace std;

int main() {
    srand(time(0));
    vector<Vartotojas> vartotojai;
    vector<Transakcija> transakcijos;
    vector<Blokas> blokai;
    char pasirinkimas;

    cout << "Ar norite generuoti naujus vartotojus, transakcijas? (t/n): ";
    cin >> pasirinkimas;

    if (pasirinkimas == 't') {
        ofstream fail("Vartotojai.txt");
        ofstream failas("Transakcijos.txt");
        ofstream failiukas("Blokai.txt");

        cout << "Generuojami vartotojai..." << endl;
        generuotiVartotojus(vartotojai, fail);
        cout << "Generuojamos transakcijos..." << endl;
        generuotiTransakcijas(transakcijos, vartotojai, failas);
        cout << "Bloku generavimas prasideda..." << endl;
        cout << "Galima is kart paspausti keleta 't', kad iskasti keleta bloku." << endl;
        generuotiBlokus(blokai, transakcijos, failiukas);
        cout << "Atnaujinami balansai..." << endl;
        atnaujintiBalansus(vartotojai, blokai);
        ekranas(blokai, transakcijos);
        cout << "Imituojamas 'decentralizuotas' blokų kasimas..." << endl;
        kasiame(transakcijos);

        fail.close();
        failas.close();
        failiukas.close();
    } 
    return 0;
}