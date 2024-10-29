#include "Blokai.h"
using namespace std;

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