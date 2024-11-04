# Supaprastintos blokų grandinės (blockchain) kūrimas - v0.2 versija
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

# Naudojimosi instrukcija
1. GitHub'e paspauskite žalią mygtuką "Code".
2. Paspauskite "Download ZIP".
3. Susinstaliavus failui, spauskite "Extract".
4. Gautą kodą galite paleisti per Visual Studio Code ar per bet kokią kitą programėlę.
5. Paleidus kodą, atsiras kalusimas "Ar norite generuoti naujus vartotojus, transakcijas? (t/n):". Paspaudus "t" bus generuojami vartotojai ir transakcijos.
6. Atsiras užrašai ant ekrano:
   * "Generuojami vartotojai..."
   * "Generuojamos transakcijos..."     
   * Sugeneruotus vartotojus galima peržiūrėti faile "Vartotojai.txt", transakcijas - "Transakcijos.txt".
8. Ant ekrano atsiras daug klaidų pranešimų - jos nurodo, kad yra netinkamų transakcijų, kur siuntėjų balansai yra nepakankami transakcijoms vykdyti.
9. Ant ekrano atsiras
    * "Bloku generavimas prasideda..."
    * "Galima is kart paspausti keleta 't', kad iskasti keleta bloku."
    * Kiek kartų paspausite "t", tiek blokų ir bus generuojama. Blokai gali būti generuojami tol, kol yra neįtrauktų transakcijų.
11. Kada programa įtrauks visas transakcijas arba tik tiek, kiek reikia, bus atnaujinami vartotojų balansai. Juos galima peržiūrėti faile "NaujiVartotojuBalansai.txt".
12. Informacija atsiras faile "Blokai.txt".
13. Galiausiai bus tokie pasirinkimai:
    
    Pasirinkite viena veiksma is nurodytu zemiau.
    1. Ieskoti transakcijos pagal ID.
    2. Ieskoti bloko pagal ID.
    Bet koks kitas sakicius baigia programa.

Tai leidžia išrinkti ar norite gauti informacijos apie tam tikras transakcijas ir blokus. Jei nenorite jokios informacijos, bet koks paspaustas skaičius užbaigs programos veikimą.

# Užduoties formuluotė
* Jei v0.1 versijoje nenaudojote Merkle medžio, v0.2 versijoje Merkle Root Hash turi būti realizuotas pagal binarinio Merkle Tree veikimą.
* Realizuokite transakcijų verifikavimą:
  - Balanso tikrinimas: Jei siuntėjo balansas yra mažesnis už siunčiamą sumą, transakcija turi būti atmesta.
  - Transakcijos maišos reikšmės tikrinimas: Patikrinkite, ar transakcijos informacijos maišos reikšmė sutampa su transakcijos ID.
* Patobulinkite blokų kasimo procesą:
  - Sudarykite 5 potencialius blokus (kandidatus) iš ~100 atsitiktinai pasirinktų transakcijų. Transakcijos blokuose gali kartotis.
  - Atsitiktinai pasirinkite vieną bloką-kandidatą ir bandykite jį "kasti" tam tikrą laiką (pvz., 5 sekundes) arba iki tam tikro bandymų skaičiaus (pvz., 100000). Jei per tą laiką blokas nebuvo "iškastas", pasirinkite kitą bloką-kandidatą.
  - Jei nė vienas iš 5 blokų nebuvo "iškastas", pailginkite kasimo laiką arba bandymų skaičių ir pakartokite procesą.
    
Šitaip imituojamas "decentralizuotas" blokų kasimas.

# Atliktos užduoties ataskaita
## 1. Merkle Root Hash
„Merkle“ šaknis yra rezultatas, gautas susiejant bloko transakcijų maišas, poruojant tas maišas ir jas vėl maišant, kol lieka tik viena maiša. Kai kurios blokų grandinės naudoja ją transakcijoms patikrinti, nes maišų poravimo ir „Merkle“ šaknų, sugeneruotų kitų mazgų, palyginimo procesas padeda išvengti papildomo maišavimo. Šis metodas sutrumpina laiką, reikalingą patikrinti transakcijoms, įtrauktoms į bloką. Nuskaityta iš https://www.investopedia.com/terms/m/merkle-root-cryptocurrency.asp

Mano kode Merkle Root Hash buvo įgyvendintas kodu:

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

Merkle Root Hash galima pamatyti bloko antraštėje:

![image](https://github.com/user-attachments/assets/269dc7ff-9ccd-41ac-8a68-2e4dca7f9a83)

## 2. Transakcijų verifikavimas

- Balanso tikrinimas: Jei siuntėjo balansas yra mažesnis už siunčiamą sumą, transakcija turi būti atmesta.
- Transakcijos maišos reikšmės tikrinimas: Patikrinkite, ar transakcijos informacijos maišos reikšmė sutampa su transakcijos ID.


## 3. "Decentralizuoto" blokų kasimo imitacija
  
## 4. Papildomos užduotys




# Papildomi reikalavimai

## 1. Transakcijų ir blokų atspausdinimas
Užduotis: sukurkite funkcijas, kurios leidžia atspausdinti bet kurią transakciją ir bloką. 
Tam tikslui sukuriau dvi funkcijas: rastiTransakcija() ir rastiBloka().
- rastiTransakcija() leidžia vartotojui rasti bet kokią norimą transakciją pagal transakcijos unikalųjį kodą. Net ir tas transakcijas, kurios nėra įtrauktos į blokus. Išrašoma visa transakcijos informacija.
- rastiBloka() leidžia vartotojui rasti bet kokią norimą bloką pagal bloko unikalųjį kodą. Išrašoma visa bloko informacija.

## 2. OOP praktikos
Tam, kad kodas būtų saugus, panaudojau enkapsuliavimą, konstrukrius ir RAII idiomą. Pavyzdžius galima pamatyti 1, 2 ir 3 užduočių aprašymuose. Ten nurodytos klasės, kuriose buvo panaudotos OOP praktikos.
