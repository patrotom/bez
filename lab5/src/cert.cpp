#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>

#define IP_ADDRESS "147.32.232.248"
#define BUFFER_SIZE 1024

using namespace std;

int main (int argc, char * argv[]) {

    // Validate the number of arguments
    if (argc != 3 ) {
        cout << "Error: invalid number of arguments!" << endl;
        return -1;
    }

    char buffer[BUFFER_SIZE];
    int sockfd;

    // Open socket communication
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero (&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr (IP_ADDRESS);
    servaddr.sin_port = htons (443);

    if (connect(sockfd, (struct sockaddr *) & servaddr, sizeof (servaddr)) != 0) {
        cout << "Error: can not connect to the server" << endl;
        return -1;
    }

    // Initialize SSL library 
    SSL_library_init();
    SSL_CTX * ctx = SSL_CTX_new (SSLv23_client_method());
    if (!ctx) {
        cout << "Error: can not process SSL context" << endl;
        return -1;
    }

    SSL_CTX_set_options (ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);

    SSL * ssl = SSL_new (ctx);
    if (!ssl) {
        cout << "Error: can not process SSL_new" << endl;
        return -1;
    }
    if (!SSL_set_fd (ssl, sockfd)) {
        cout << "Error: can not process SSL_set_fd" << endl;
        return -1;
    }
    if (SSL_connect (ssl) <= 0) {
        cout << "Error: can not process SSL_connect" << endl;
        return -1;
    }

    // Get certificate from the website
    X509 * cert = SSL_get_peer_certificate (ssl);
    if (!cert) {
        cout << "Error: can not get certificate" << endl;
        return -1;
    }

    // Open a file where the website will be written
    FILE * file_pem = fopen (argv[2], "w"); 
    if (!PEM_write_X509(file_pem, cert)) {
        cout << "Error: can not write certificate to file" << endl;
        return -1;
    }
    fclose (file_pem);

    snprintf (buffer, BUFFER_SIZE, "GET /student/odkazy HTTP/1.1\r\nConnection: close\r\nHost: fit.cvut.cz\r\n\r\n");

    // Write the certificate to the file
    if (SSL_write (ssl, buffer, strlen (buffer)) <= 0) {
        cout << "Error: can not process SSL_write" << endl;
        return -1;
    }

    // Write the website to the file
    int tmp;
    int dataLength = 0;
    FILE * file_output = fopen (argv[1], "w");
    while ((tmp = SSL_read (ssl, buffer, BUFFER_SIZE)) > 0) {
        fwrite (buffer, sizeof (char), tmp, file_output);
        dataLength += tmp;
    }

    // Output the information about the operations
    cout << "===============================" << endl;
    cout << "Certificate written to the file: " << argv[2] << endl;
    cout << "Site downloaded to the file: " << argv[1] << " [" << dataLength << " bytes]" << endl;
    cout << "===============================" << endl;

    fclose (file_output);
    SSL_shutdown (ssl);
    close (sockfd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);

    return 0;
}