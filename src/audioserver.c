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

    // Variables
    int socket_descriptor, err_bind; // Descripteur du socket et indicateur du succès (ou non) de bind
    struct sockaddr_in address, from; // Structures contenant l'addresse du socket et l'addresse de la source des messages
    char msg[64]; // Buffer pour le message
    socklen_t rlen, flen; // Variables pour le nombre d'octets reçus et pour la longueur de l'addresse source des messages


    // Créatoion du Socket UDP sur le port 1234
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0); 
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

    // Recevoir un message du client afin de récupérer son IP
    flen = sizeof(struct sockaddr_in);
    rlen = recvfrom(socket, msg, sizeof(msg), 0, (struct sockaddr*) &from, &flen); // On attend de recevoir un message

    // Traitement des erreurs
    if(rlen < 0){
        // A compléter
    }

    // TODO : Communication avec le client

}