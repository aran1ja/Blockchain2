# Supaprastintos blokų grandinės (blockchain) kūrimas
Šios užduoties tikslas – sukurti supaprastintą blokų grandinę (angl. blockchain), kurios duomenų sluoksnio struktūra pavaizduota žemiau:
 ![изображение](https://github.com/user-attachments/assets/09277d27-b027-4176-a6b7-57964d0cdba5)

Svarbūs blokų grandinės aspektai:
* Blokų grandinė (blockchain) yra nuoseklus blokų sąrašas, kur kiekvienas blokas susietas su prieš tai einančio bloko maišos funkcijos reikšme (angl. hash). Maišos funkcija taikoma 6 pagrindiniams bloko antraštės (angl. block header) elementams.
* Kiekvienas blokas sudarytas iš dviejų dalių: antraštės (angl. header) ir turinio (angl. Body of the Block (Transactions)).
  
Antraštę sudaro:
* Ankstesnio bloko maišos reikšmė ( Prev Block Hash )
* Laiko žyma ( Timestamp )
* Blokų grandinės duomenų struktūros versija ( Version )
* Visų bloko transakcijų maišos reikšmė, gauta naudojant Merkle medį ( Merkel Root Hash )
* Atsitiktinis skaičius, naudojamas tinkamo sudėtingumo (nustatomo pagal nulių skaičių maišos reikšmės pradžioje) bloko maišos reikšmei gauti ( Nonce )
* Bloko maišos reikšmės sudėtingumas ( Difficulty Target )
  
Turinį sudaro:
* Visos bloko transakcijos.

# Užduoties formuluotė
Sukurkite "centralizuotą" blokų grandinę (blockchain) ir imituokite jos veikimą kuo realistiškiau. Preliminari veiksmų seka:
1. Sugeneruokite ~1000 tinklo vartotojų, turinčių šiuos atributus:
* vardą,
* viešąjį raktą ( public_key ),
* atsitiktinį valiutos balansą (pvz., nuo 100 iki 1000000 valiutos vienetų).
2. Sugeneruokite ~10000 naujų transakcijų, kurios dar nėra įtrauktos į jokį bloką. Transakcijos struktūra turi turėti šiuos atributus:
* transakcijos ID (kitų transakcijos laukų maišos reikšmė),
* siuntėjas (jo viešasis raktas),
* gavėjas (jo viešasis raktas),
* suma.
Transakcijų struktūrą galite tobulinti, pvz., vietoj sąskaitos modelio (angl. account model) galite naudoti UTXO modelį.
3. Atsitiktinai pasirinkite 100 transakcijų iš transakcijų sąrašo ir pabandykite jas įtraukti į naują bloką.
4. Realizuokite naujų blokų kasimo (angl. mining) procesą, naudojant Proof-of-Work (PoW) algoritmą. Tikslas – rasti naujo bloko maišos reikšmę, atitinkančią Difficulty Target reikalavimą (nulių skaičių maišos reikšmės pradžioje). Nulių skaičius priklauso nuo jūsų sukurtos maišos funkcijos.
5. Suradus tinkamą naujo bloko maišos reikšmę:
* ištrinkite į bloką įtrauktas transakcijas iš transakcijų sąrašo;
* "įvykdykite" transakcijas, t.y. atnaujinkite vartotojų balansus;
* pridėkite naują bloką prie blokų grandinės.
6. Kartokite 3-5 žingsnius, kol yra neįtrauktų transakcijų. Taip pat galite kartoti 1-5 žingsnius, generuodami naujus vartotojus ir transakcijas.

# Atliktos užduoties formuluotė
## 1. Vartotojų generavimas
Pirmai užduočiai atlikti sukūriau vartotojų klasę.

       class Vartotojas {
        private:
        string vardas;
        string viesasis_raktas;
        int balansas;
    
        public:
        // Konstruktorius
        Vartotojas() = default;
        Vartotojas(const string& vardas, const string& viesasis_raktas, int balansas) 
            : vardas(vardas), viesasis_raktas(viesasis_raktas), balansas(balansas) {}
    
        // Destruktorius
        ~Vartotojas() {}
    
        // Getteriai ir setteriai
        string getVardas() const { return vardas; }
        string getViesasisRaktas() const { return viesasis_raktas; }
        int getBalansas() const { return balansas; }
        void setBalansas(int newBalansas) { balansas = newBalansas; }
    };

Taip pat sukūriau dvi funkcijas: randomVardas() ir generuotiVartotojus().
- randomVardas() generuoja atsitiktinius vadrus iš sąrašo.

        vector<string> var = {"Tomas", "Andrius", "Algirdas", "Renata", "Justina", "Kristina"};
        vector<string> pav = {"Kazlauskas", "Petrauskas", "Jankauskas", "Zukauskas", "Simutis"};
        
        string vard = var[rand() % var.size()];
        string pavard = pav[rand() % pav.size()];
        return vard + " " + pavard;
    
- generuotiVartotojus() generuoja 1000 atsitiktinius viešuosius raktus, balansus ir išveda informaciją apie vartotojus į failą "Vartotojai.txt".

## 2. Transakcijų generavimas
Antrai užduočiai atlikti sukūriau transakcijų klasę.

         class Transakcija {
          private:
          string transakcijos_id;
          string siuntejo_viesasis_raktas;
          string gavejo_viesasis_raktas;
          int suma;
      
          public:
          // Konstruktorius
          Transakcija() = default;
          Transakcija(const string& transakcijos_id, const string& siuntejo_viesasis_raktas, const string& gavejo_viesasis_raktas, int suma) 
              : transakcijos_id(transakcijos_id), siuntejo_viesasis_raktas(siuntejo_viesasis_raktas), gavejo_viesasis_raktas(gavejo_viesasis_raktas), suma(suma) {}
          
          // Destruktorius
          ~Transakcija() {}
      
          // Getteriai ir setteriai
          string getTransakcijosId() const { return transakcijos_id; }
          string getSiuntejoViesasisRaktas() const { return siuntejo_viesasis_raktas; }
          string getGavejoViesasisRaktas() const { return gavejo_viesasis_raktas; }
          int getSuma() const { return suma; }
          };

   
