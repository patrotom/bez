# Co je OpenSSL #
[OpenSSL](http://www.openssl.org/) je knihovna pracující s SSL (Secure Socket Layer) a TLS (Transport Layer Security) protokoly. Tato knihovna je vyvíjena od roku 1998 a je založena na starší knihovně SSLeay. OpenSSL se skládá ze tří hlavních částí:

## Command line tool ##

OpenSSL obsahuje nástroj pro příkazový řádek, pomocí kterého lze rychle a jednoduše šifrovat text (DES, 3DES, RC4 …), vytvářet certifikáty (X.509), počítat hashe (SHA1, SHA256, MD5 …) a další funkce z Crypto knihovny. Přiklad na vypočítání SHA1 pro zadaný text v příkazovém řádku:

``` bash
echo -n "Hashovany text." | openssl sha1
```

## SSL library ##

SSL library je knihovna implementující vrstvy síťové komunikace Secure Sockets Layer (SSL v2/v3) a Transport Layer Security (TLS v1). Zastřešuje použití hashovacích a šifrovacích funkcí pro navázání zabezpečené relace a implementuje také několik typů struktur, se kterými ssl funkce pracují, jako například SSL_CTX, SSL_METHOD a další.

## Crypto library ##

Crypto knihovna poskytuje implementaci konkrétních kryptografických funkcí, datových struktur pro uložení klíčů a formátování zpráv a práci s certifikáty. Kromě nízkoúrovňové implementace jednotlivých algoritmů také poskytuje rozhraní vyšší úrovně, tzv. EVP (envelope) funkce.

# Ukázky kódu #

## Příklad na hashovací funkce ##

Tento kód zahashuje text hashem uloženým v proměnné hashFunction. Aktualizace 21.3.2018 JB: Upraveno na novejsi verzi API.

``` c
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
 
int main(int argc, char *argv[]){
 
  int i, res;
  char text[] = "Text pro hash.";
  char hashFunction[] = "sha1";  // zvolena hashovaci funkce ("sha1", "md5" ...)
 
  EVP_MD_CTX *ctx;  // struktura kontextu
  const EVP_MD *type; // typ pouzite hashovaci funkce
  unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
  int length;  // vysledna delka hashe
 
  /* Inicializace OpenSSL hash funkci */
  OpenSSL_add_all_digests();
  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
  type = EVP_get_digestbyname(hashFunction);
 
  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
  if(!type) {
    printf("Hash %s neexistuje.\n", hashFunction);
    exit(1);
  }
 
  ctx = EVP_MD_CTX_create(); // create context for hashing
  if(ctx == NULL) exit(2);
 
  /* Hash the text */
  res = EVP_DigestInit_ex(ctx, type, NULL); // context setup for our hash type
  if(res != 1) exit(3);
  res = EVP_DigestUpdate(ctx, text, strlen(text)); // feed the message in
  if(res != 1) exit(4);
  res = EVP_DigestFinal_ex(ctx, hash, (unsigned int *) &length); // get the hash
  if(res != 1) exit(5);
 
  EVP_MD_CTX_destroy(ctx); // destroy the context
 
  /* Vypsani vysledneho hashe */
  printf("Hash textu \"%s\" je: ", text);
  for(i = 0; i < length; i++){
    printf("%02x", hash[i]);
  }
  printf("\n");
 
  exit(0);
}
```

Zdrojový kód zkompilujete pomocí následujícího příkazu. Doporučjeme napsat si makefile.

``` bash
gcc -Wall nazevSouboru.c -lcrypto -o nazevVystupu
```

**Úkol 1 (1 bod):** Najděte libovolnou zprávu, jejíž hash (SHA-256) začíná zleva na posloupnost bajtů 0xAA, 0xBB. Nalezenou zprávu vypište v hexadecimální podobě.

## Příklad na proudovou šifru ##

Tento kód zašifruje pomocí proudové šifry rc4 otevřený text ot za použití klíče key a inicializačního vektoru iv. Aktualizace 21.3.2018 JB: Upraveno na novejsi verzi API.

``` c
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
 
int main(void) {
  int res;
  unsigned char ot[1024] = "Text pro rc4.";  // open text
  unsigned char st[1024];  // sifrovany text
  unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
  const char cipherName[] = "RC4";
  const EVP_CIPHER * cipher;
 
  OpenSSL_add_all_ciphers();
  /* sifry i hashe by se nahraly pomoci OpenSSL_add_all_algorithms() */
  cipher = EVP_get_cipherbyname(cipherName);
  if(!cipher) {
    printf("Sifra %s neexistuje.\n", cipherName);
    exit(1);
  }
 
  int otLength = strlen((const char*) ot);
  int stLength = 0;
  int tmpLength = 0;
 
  EVP_CIPHER_CTX *ctx; // context structure
  ctx = EVP_CIPHER_CTX_new();
  if (ctx == NULL) exit(2);
 
  printf("OT: %s\n", ot);
 
  /* Sifrovani */
  res = EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);  // context init - set cipher, key, init vector
  if(res != 1) exit(3);
  res = EVP_EncryptUpdate(ctx,  st, &tmpLength, ot, otLength);  // encryption of pt
  if(res != 1) exit(4);
  stLength += tmpLength;
  res = EVP_EncryptFinal_ex(ctx, st + stLength, &tmpLength);  // get the remaining ct
  if(res != 1) exit(5);
  stLength += tmpLength;
 
  printf ("Zasifrovano %d znaku.\n", stLength);
 
  /* Desifrovani */
  res = EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);  // nastaveni kontextu pro desifrovani
  if(res != 1) exit(6);
  res = EVP_DecryptUpdate(ctx, ot, &tmpLength,  st, stLength);  // desifrovani st
  if(res != 1) exit(7);
  otLength += tmpLength;
  res = EVP_DecryptFinal_ex(ctx, ot + otLength, &tmpLength);  // dokonceni (ziskani zbytku z kontextu)
  if(res != 1) exit(8);
  otLength += tmpLength;
 
   /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
 
  /* Vypsani zasifrovaneho a rozsifrovaneho textu. */
  printf("ST: %s\nDT: %s\n", st, ot);
 
  exit(0);
}
```

Zdrojový kód zkompilujete pomocí následujícího příkazu. Avšak doporučujeme napsat si makefile.

``` bash
gcc -Wall nazevSouboru.c -lcrypto -o nazevVystupu
```

**Úkol 2 (2 body):**

* Prozkoumejte vliv inicializačního vektoru na šifrový text. Zdůvodněte zjištění.
* Modifikujte program tak, aby zašifroval dvě různé zprávy. Zprávy zašifrujte na sobě nezávisle, vždy postupem EVP_EncryptInit(…), EVP_EncryptUpdate(…), EVP_EncryptFinal(…) obě dvě **stejným klíčem** (což je **fatální chyba**, jak si později ověříte):
    1. Zvolte si tajný klíč (např. 16 znakový řetězec)
    2. První zpráva bude tajná, 30 znaků dlouhá, její text neprozrazujte
    3. Druhá zpráva bude veřejně známá (prozrazená nebo vynucená) „abcdefghijklmnopqrstuvwxyz0123“
    4. **Šifrové texty obou zpráv (v hexadecimální podobě) pošlete svému kolegovi** elektronickou poštou, Váš kolega by měl **poslat svoji dvojici zpráv Vám**. Nezapomeňte ŠT označit, aby bylo zřejmé, který odpovídá veřejně známé zprávě.
    5. Víte následující skutečnosti: Váš kolega byl neopatrný a zašifroval dvě různé zprávy stejným klíčem. Navíc u jedné ze zpráv znáte otevřený text. Vaším úkolem je vyluštit i druhou z obou zpráv. (Postup je triviální.)
* Váš program musí přijímat hexadecimální podobu šifrových textů obou zpráv na svém standardním vstupu. Vyluštěnou zprávu vypíše v textové podobě.

Příklad šifrových textů dvou zpráv v hexadecimální podobě, které byly zašifrovány stejným klíčem, z nichž jedna je zašifrovaná známá zpráva „abcdefghijklmnopqrstuvwxyz0123“, a druhou máte vyluštit:

`06fb7405eba8d9e94fb1f28f0dd21fdec55fd54750ee84d95ecccf2b1b48`
`33f6630eaea4dba152baf38d019c04cbc759c94544fb9a815dc68d7b5f1a`

# Zdroje #

* Oficiální stránky OpenSSL: http://www.openssl.org/
* Secure programming with the OpenSSL API: http://ibm.co/ZYsHLO
* John Viega, Matt Messier, Pravir Chandra: Network Security with OpenSSL, https://books.google.cz/books?id=IIqwAy4qEl0C
* OpenSSL Wiki http://wiki.openssl.org/index.php/Main_Page

(C) 2013-2015 Jiří Buček, Jan Duchač
