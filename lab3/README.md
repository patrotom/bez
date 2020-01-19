# Obsah cvičení #

Účelem dnešního cvičení je seznámit se s použitím blokových šifer a jejich operačních módů. Přitom si také ukážeme šifrování delšího souboru. Za splnění celého zadání dostanete **5 bodů**.

# Zadání #

* Pomocí šablony kódu z minulé laboratoře si připravte program, který bude šifrovat zadaný řetězec pomocí šifry DES s operačním módem ECB.
* Volbu šifry a operačního módu zjistěte z dokumentace `EVP_EncryptInit` (man `EVP_EncryptInit`).
* Vlastnosti operačního módu demonstrujte pomocí zašifrování obrazových dat (datové části obrázku ve formátu BMP)
    * Stáhněte si např. tento obrázek ve formátu BMP (rozbalte zip): [obrazek.zip](https://edux.fit.cvut.cz/courses/BI-BEZ/_media/labs/03/obrazek.zip). ([homer-simpson.zip](https://edux.fit.cvut.cz/courses/BI-BEZ/_media/labs/03/homer-simpson.zip))
    * Napište program, který zkopíruje hlavičku a **zašifruje** část souboru s obrazovými daty pomocí DES v módu ECB. Výstupní soubor se bude jmenovat (původní_jméno)_ecb.bmp
    * Napište program, který **dešifruje** obrázek zašifrovaný prvním programem. Výstupní soubor se bude jmenovat (původní_jméno)_dec.bmp. Porovnejte dešifrovaný soubor s původním.
    * Porovnejte původní obrázek a jeho zašifrovanou podobu a vysvětlete svá zjištění
    * Změňte pro šifrování i dešifrování použitý operační mód na **CBC** a vytvořte (původní_jméno)_cbc.bmp a (původní_jméno)_cbc_dec.bmp(upřesní cvičící)
    * Porovnejte původní obrázek a jeho zašifrovanou podobu a vysvětlete svá zjištění
* **Na první řádek zdrojáku dejte komentář se jménem autora!**

# Formát obrázků BMP (zjednodušeno)# 

Jednotky: bajty (slabiky)

| **Pozice** | **Délka**            | **Význam**                            |
| ------ | ---------------- | --------------------------------- |
| 0      | 2                | Magické číslo (ASCII znaky BM)    |
| 2      | 4                | Délka souboru (unsigned int)      |
| 6      | 4                | 2x 2B rezervováno                 |
| 10     | 4                | Pozice začátku obrazových dat **zac** |
| 14     | …                | Informace o formátu …             |
| …      | …                | Barevná paleta (nepovinné) …      |
| **zac**    | do konce souboru | **Obrazová data**                     |

# Postup šifrování delšího souboru, pole, atd. #

* Inicializace knihovny, alokace kontextu (viz minulé cvičení)
* `EVP_EncryptInit_ex(ctx, type, NULL, key, iv);`
* Cyklus: dokud mám data
    1. Přečti obrazová data do pole ot (např. fread, …), délka přečtených dat je otLength.
    2. `EVP_EncryptUpdate(ctx, st, &stLength, ot, otLength);`
    3. Zapiš do výstupního souboru obsah pole st délky stLength (může se lišit od otLength).
* `EVP_EncryptFinal_ex(ctx, st, &stLength);` – dokončení šifrování posledního bloku
* Zapiš do výstupního souboru obsah pole st délky `stLength`
* U blokových šifer je ve výchozím nastavení zapnuto zarovnání (padding). Proto může být šifrovaný soubor větší než původní.
