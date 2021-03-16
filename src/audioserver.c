#include <stdint.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../include/audio.h"

int main(){

    // Création du socket UDP sur le port 1234
    int socket_descriptor, err_bind;
    struct sockaddr_in address;

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0); // Création du Socket
    // Traitement des erreurs
    if (socket_descriptor < 0){
        // A completer
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(1234);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    err_bind = bind(socket_descriptor, (struct sockaddr *) &address, sizeof(struct sockaddr_in));
    // Traitement des erreurs de bind()
    if (err_bind < 0){
        // A completer
    }

    // TODO : Envoie des données de la musique (Choix de la musique, lecture du fichier, envoie des données)
}