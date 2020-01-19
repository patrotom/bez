# Komunikace přes OpenSSL #

Za splnění celého zadání dostanete **3 body**.

Napište program v jazyce C, který:

* stáhne stránku předmětu https://fit.cvut.cz/student/odkazy do souboru;
* vypíše informace o certifikátu serveru fit.cvut.cz;
* uloží ho do souboru ve formátu `PEM`.
* Soubor PEM zobrazte v textové podobě pomocí utility `openssl x509`.

Dokumentace ke knihovně openssl naleznete na stránkách projektu: http://www.openssl.org/docs/

## Postup ##

* Vytvořte TCP spojení na server fit.cvut.cz, port 443
    * Znáte z předmětu Počítačové sítě, viz socket, connect
* Inicializujte knihovnu `OpenSSL (SSL_library_init)`
* Vytvořte nový kontext `(SSL_CTX_new, použijte metodu SSLv23_client_method)`
    * Zakažte zastaralé a děravé protokoly: `SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);`
* Vytvořte SSL strukturu `(SSL_new)`
* Přiřaďte otevřené spojení `(SSL_set_fd)`
* Zahajte SSL komunikaci `(SSL_connect)`

*Nyní je navázáno SSL spojení na HTTPS server, můžete poslat požadavek*

* Pošlete HTTP požadavek po zabezpečeném kanálu `(SSL_write)`
* Ve smyčce čtěte odpověď po částech, jak přicházejí po síti, a ukládejte výsledná data do souboru `(SSL_read)`

*Na závěr po sobě uklidíme*

* Ukončíme SSL session na otevřeném socketu `(SSL_shutdown)`
* Zavřeme socket
* Uvolníme strukturu SSL a kontext `(SSL_free, SSL_CTX_free)`

**Nezapomeňte přidat do linkování knihovnu ssl (-lssl).**

## Přečtení certifikátu ze serveru ##

* Získejte certifikát od serveru `(SSL_get_peer_certificate)`
* a zapište ho do souboru `(PEM_write_X509)`

## Příklad kódu pro vytvoření spojení ##

Známe-li IPv4 adresu serveru, stačí nám pro vytvoření TCP spojení jen několik příkazů (zdroj)

``` c++
   int sockfd;
   struct sockaddr_in servaddr;
 
   sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr("111.111.111.111"); //ip address
   servaddr.sin_port = htons(443); // port
   // Pozor, nektere platformy maji jeste pole sin_len.
 
   if (0 != connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) {error...}
```

Chcete-li umět překlad jmen (DNS) a zároveň IPv4 i IPv6, doporučujeme `man getaddrinfo`.