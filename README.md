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

# Atliktos užduoties formuluotė
## 1. Vartotojų generavimas

## 2. Transakcijų generavimas

## 3. Blokų generavimas
  
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

## 6. Visų transakcijų vykdymas
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

# Papildomi reikalavimai v0.1

## 1. Transakcijų ir blokų atspausdinimas
Užduotis: sukurkite funkcijas, kurios leidžia atspausdinti bet kurią transakciją ir bloką. 
Tam tikslui sukuriau dvi funkcijas: rastiTransakcija() ir rastiBloka().
- rastiTransakcija() leidžia vartotojui rasti bet kokią norimą transakciją pagal transakcijos unikalųjį kodą. Net ir tas transakcijas, kurios nėra įtrauktos į blokus. Išrašoma visa transakcijos informacija.
- rastiBloka() leidžia vartotojui rasti bet kokią norimą bloką pagal bloko unikalųjį kodą. Išrašoma visa bloko informacija.

## 2. OOP praktikos
Tam, kad kodas būtų saugus, panaudojau enkapsuliavimą, konstrukrius ir RAII idiomą. Pavyzdžius galima pamatyti 1, 2 ir 3 užduočių aprašymuose. Ten nurodytos klasės, kuriose buvo panaudotos OOP praktikos.
