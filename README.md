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

Sukūriau dvi funkcijas: randomVardas() ir generuotiVartotojus().
- randomVardas() generuoja atsitiktinius **vardus** iš sąrašo.

       vector<string> var = {"Tomas", "Andrius", "Algirdas", "Renata", "Justina", "Kristina"};
       vector<string> pav = {"Kazlauskas", "Petrauskas", "Jankauskas", "Zukauskas", "Simutis"};
             
       string vard = var[rand() % var.size()];
       string pavard = pav[rand() % pav.size()];
       return vard + " " + pavard;
    
- generuotiVartotojus() generuoja 1000 atsitiktinius **viešuosius raktus**, **balansus** ir išveda informaciją apie vartotojus į failą "Vartotojai.txt".

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

 Transakcijoms sukūrti buvo padaryta viena funkcija: generuotiTransakcijas(). Ji generuoja 10.000 transakcijų pagal tokį nuoseklumą:
 * Transakcijai randamas vartotojas, kurio balansas yra didesnis už 1. Jis užvadinamas **siuntėju** ir užrašomas jo viešasis raktas.
 * Transakcijai randamas vartotojas, kuris nėra priskirtas kaip siuntėjas - tada jis gali būti **gavėju** ir užrašomas jo viešasis raktas.
 * Generuojama **suma**, kuri bus pervedama transakcijoje.
 * Generuojamas **unikalusis transakcijos kodas** (transakcijos_id). Transakcijos id priklauso nuo siuntėjo viešojo rakto, gavėjo viešojo rakto ir sumos.
 * Patikrinama ar siuntėjo balanso pakanka transakcijai vykdyti. Jeigu ne - terminale surašomi klaidų pranešimai ir nurodoma kokios transakcijos nebus vykdomos.
 * Informacija apie transakcijas išvedama į failą "Transakcijos.txt".

## 3. Blokų generavimas
Trečiai užduočiai atlikti sukūriau blokų klasę.

       class Blokas {
       private:
       string bloko_id;
       vector<Transakcija> transakcijos;
       string prev_block_hash;     //Ankstesnio bloko maisos reiksme 
       time_t timestamp;           //Laiko zyma 
       int version = 1;            //Blokų grandines duomenu strukturos versija
                                   // Merkel Root Hash
       int nonce;                  //Atsitiktinis skaicius, naudojamas tinkamo sudetingumo bloko maisos reiksmei gauti 
       int difficulty_target = 2;  // Bloko maisos reiksmes sudetingumas 
   
       public:
       // Konstruktorius
       Blokas() = default;
       Blokas(const string& id, const vector<Transakcija>& trans = {}, int nonce = 0,
              const string& prev_hash = "")
           : bloko_id(id), transakcijos(trans), nonce(nonce), timestamp(time(nullptr)),
           prev_block_hash(prev_hash) {}
   
       // Destruktorius
       ~Blokas() {}
   
       // Getteriai ir setteriai
       string getBlokoId() const { return bloko_id; }
       const vector<Transakcija>& getTransakcijos() const { return transakcijos; }
       string getPreviousBlockHash() const { return prev_block_hash; }
       const time_t& getTimestamp() const { return timestamp; }
       int getVersion() const { return version; }
       int getNonce() const { return nonce; }
       void setNonce(int newNonce) { nonce = newNonce; }
       int getDifficultyTarget() const { return difficulty_target; }
   
       void setBlokoId(const string& id) { bloko_id = id; }
       void setPreviousBlockHash(const string& prevHash) { prev_block_hash = prevHash; }
       void setTimestamp(time_t ts) { timestamp = ts; }
  

Sukūriau funkciją generuotiBlokus(). Ji generuoja blokus po 100 transakcijų pagal tokį nuoseklumą:
* Skaičiuoja kiek liko neįtrauktų transakcijų.
* Klausiama ar reikia iškasti naują bloką su 100 transakcijomis.
* Tikrinama ar ta pati transakcija nėra įtraukta kelis kartus.
* Sujungiami visi 100 transakcijų id ir generuojamas naujas bloko hash.
* Visa bloko ir irašytų į ją transakcijų informacija įrašoma į "Blokai.txt".
  
## 4. Blokų kasimo procesas
Šiai užduočiai atlikti reikia realizuoti naujų blokų kasimo procesą, naudojant Proof-of-Work (PoW) algoritmą. PoW yra metodas, kuriame tinklo dalyviai sprendžia sudėtingus skaičiavimus, kad patvirtintų naujus blokus blokų grandinėje, taip užtikrindami tinklo saugumą ir patikimumą. 

Prie to, kas buvo 3 užduotyje pridėjau, kad būtų skaičiuojama ir išrašoma dar tokia bloko informacija:
  - Praeito bloko hash
  - Timestamp
  - Version
  - Nonce
  - Difficulty Target

Tada yra iškviečiama funkcija pridetiNonce(). Tokiu būdu randama naujo bloko maišos reikšmė, kuri atitinka Difficulty Target. Aš pažymėjau, kad Difficulty Target = 2, todėl gaunu blokus su 2 nuliais maišos reikšmės pradžioje. Pavyzdžiui, 

      00ad411327b17eadd78100d4dcc9126dd946c6fbba91003f450e121f683d767b, 
      00edb785ba3c865ddc8eb365d8ce90efd84706fb274cf8e7550e1a1f6c3d747b,
      00ed4f42c7ccb43ddac2bcdb22e190efd847f2a45abcca87550e1a1f6c3d747b.

Galiausiai, visa bloko ir irašytų į ją transakcijų informacija įrašoma į "Blokai.txt".

## 5. Transakcijų vykdymas ir duomenų atnaujinimas
Šiai užduočiai reikėjo atlikti tokius taškus:
* Ištrinti į bloką įtrauktas transakcijas iš transakcijų sąrašo; 
* Įvykdyti transakcijas, t.y. atnaujinti vartotojų balansus; 
* Pridėti naują bloką prie blokų grandinės.

Transakcijų ištrinimui iš transakcijų sąrašo panaudojau dvi funkcijas: filtruotiTransakcijas() ir atnaujintiTransakcijuFaila().
- filtruotiTransakcijas()
  Funkcija peržiūri visas transakcijas, išrenka 100 transakcijų (arba mažiau, jei tiek nėra) ir tą išrinktą transakciją ištrina iš vektoriaus naudojant erase.
- atnaujintiTransakcijuFaila()
  Funkcija perrašo "Transakcijos.txt" failą ir palieka tik tas transakcijas, kurios nebuvo įtrauktos į bloką.

Vartotojų balansų atnaujinimui panaudojau dvi funkcijas: atnaujintiBalansus() ir issaugotiBalansus().
- atnaujintiBalansus()
  Tikrina ar vartotojas tikrai turi pakankamą balansą transakcijai vykdyti ir koreguoja balanso kiekį.
- issaugotiBalansus()
  Įrašo naujus vartotojų balansus į failą "NaujiVartotojuBalansai.txt". Ji yra kviečiama praeitos funkcjios.

Naujas blokas yra pridėdamas prie blokų grandinės funkcijos generuotiBlokus() veikimo metu.

## 5. Visų transakcijų vykdymas
Paleidžiau programą. Kadangi į blokus yra įtraukiamos tik tos transakcijos, kur siuntėjų balansai turi pakankamą balansą, iš 10000 transakcijų liko tik 8501, kurias galima įtraukti į blokus.

![изображение](https://github.com/user-attachments/assets/4afe9eef-37f1-4263-b6f7-2b3d8e94bf17)

"Blogų" transakcijų klaidų pranešimai.

![изображение](https://github.com/user-attachments/assets/c790b8a5-71fb-4a60-bbe2-9e871e61d053)

Programos veikimas:

![изображение](https://github.com/user-attachments/assets/4b71d95a-a14d-4c1a-882e-2c23615b1c3e)

"Vartotojai.txt" balansai:

![изображение](https://github.com/user-attachments/assets/116558ec-c666-4b93-967a-3c853a8a8c21)

Nauji balansai "NaujiVartotojuBalansai.txt" po transakcijų:

![изображение](https://github.com/user-attachments/assets/49201e9b-4349-487d-8957-a8ba6c71747f)

"Transakcijos.txt" failas:

![изображение](https://github.com/user-attachments/assets/e7e22846-e28d-4b4c-a375-3b475532f835)

Iš viso gavosi 86 blokų (paskutiniame yra tik 1 transakcija):

![изображение](https://github.com/user-attachments/assets/ac9e13f4-c613-4d3f-a5be-d2110128c3ff)


Patikrinimas, ar balanso atnaujinimas yra vykdomas tik tuo metu, kada jos įtrauktos į bloką, ar ir net tada, kada nėra įtrauktos.
- Balanso stebėjimas kada transakcijos įtrauktos į blokus.

Pradinis balansas:

![изображение](https://github.com/user-attachments/assets/00bf370a-c501-4325-a8b0-427ef2643372)

Transakcijos:

![изображение](https://github.com/user-attachments/assets/412fe099-ad0a-46dc-a439-e021c9d796a1)

![изображение](https://github.com/user-attachments/assets/af6c99c9-ee9f-4e80-9b6a-4fd05ecd2635)

![изображение](https://github.com/user-attachments/assets/c25c4631-46f0-43cd-b1da-1542dca3aca0)

![изображение](https://github.com/user-attachments/assets/64cd3aa5-f384-4d83-897d-ee9a5f4bc780)

![изображение](https://github.com/user-attachments/assets/d37e8d77-27ce-4d4c-ac2e-e71a475eed6c)

![изображение](https://github.com/user-attachments/assets/02ab35de-6acb-419f-b07f-84282e203cad)

![изображение](https://github.com/user-attachments/assets/65926f2c-56dd-43d8-b6e9-64d5d704bd6c)

![изображение](https://github.com/user-attachments/assets/4c4eff5e-20a2-47d5-bce0-2114b5447da6)

![изображение](https://github.com/user-attachments/assets/72652aa9-119c-4f64-8268-5d8d58ac19b9)

![изображение](https://github.com/user-attachments/assets/52cd8de5-cbc6-4cc9-94a2-170524d0ac18)


Skaičiavimas: 6418 - 3736 + 8251 - 5658 + 2548 + 7614 + 3634 - 1396 + 3973 + 3020 - 4168 = 20500.
  
- Balanso stebėjimas kada ne visos transakcijos įtrauktos į blokus.

Į blokus neįtrauktos transakcijos:

![изображение](https://github.com/user-attachments/assets/eeae6ffb-600b-4e00-a2bf-4040bcff5345)

![изображение](https://github.com/user-attachments/assets/67e048f5-6bfe-46db-a444-b68bae062f6e)

![изображение](https://github.com/user-attachments/assets/6044a3da-9549-49ce-9422-2625d66a19c6)


Skaičiavimas: 6418 - 3736 + 8251 - 5658 + 2548 + 7614 + 3634 - 1396 + 3973 + 3020 - 4168 + 956 + 8135 + 9821 = 39412.

- Koks iš tikrųjų yra atnaujintas balansas:

![изображение](https://github.com/user-attachments/assets/5917e9e7-b2d9-42a6-a2cc-24d276fba06d)

Taigi matome, kad balansas yra atnaujinamas tik tada, kad transakcijos yra įtrauktos į blokus.


