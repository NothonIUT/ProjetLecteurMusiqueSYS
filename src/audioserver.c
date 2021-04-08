#include <stdint.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../include/audio.h"
#include "../include/serveurclient.h"

#define DEFAULT_PATH "/private/student/e/ne/gcalonne/Bureau/TP_SYS/ProjetLecteurMusiqueSYS-master/data/"

int main() {

    // Variables
    int socket_descriptor, err_bind, err_send; // Descripteur du socket et indicateur du succès (ou non) de bind
    struct sockaddr_in adresse, from; // Structures contenant l'adresse du socket et l'adresse de la source des messages
    char msg_received[64], msg_send[64]; // Buffer pour le message
    socklen_t rlen, flen; // Variables pour le nombre d'octets reçus et pour la longueur de l'adresse source des messages


    // Création du Socket UDP sur le port 1234
    printf("Création du socket...\n");
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0); 
    // Traitement des erreurs
    if (socket_descriptor < 0) {
        printf("Erreur lors de la création du socket serveur !\n");
        return -1;
    }

    printf("Assignation du port...\n");
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(1234);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    err_bind = bind(socket_descriptor, (struct sockaddr *) &adresse, sizeof(struct sockaddr_in));
    // Traitement des erreurs de bind()
    if (err_bind < 0) {
        printf("Erreur lors de l'assignation du port !\n");
        return -1;
    } 

    // Recevoir un message du client afin de récupérer son IP et le nom du fichier à jouer
    printf("En attente d'un client...\n");
    flen = sizeof(struct sockaddr_in);
    rlen = recvfrom(socket_descriptor, msg_received, sizeof(msg_received), 0, 
                    (struct sockaddr*) &from, &flen);
    if(rlen < 0) {
        printf("Erreur de la réception du nom du fichier à jouer !\n");
        return -1;
    }else {
        char filename[512]; // Déclaration de la variable qui contiendra le chemin complet jusqu'au fichier
        strcpy(filename, DEFAULT_PATH);
        strcat(filename, msg_received);
        int sample_rate = 0;
        int sample_size = 0;
        int channels = 0;

        printf("Envoi des informations de la musique...\n");

        int file_descriptor = envoi_infos_musique(sample_rate, sample_size, channels, filename, socket_descriptor, from);
        
        if (file_descriptor == -1) {
            printf("Erreur lors de l'envoi des informations de la musique !\n");
            return -1;
        }
        
        // Tableau dans lequel les octets lus seront stockés pour être écrits dans le lecteur
        char bytes_lus[sample_size];
        ssize_t nbr_bytes_lu = sample_size;

        printf("Envoi de la musique... !\n");

            while(nbr_bytes_lu == sample_size) {
                // Lecture des octets dans le fichier.wav
                nbr_bytes_lu = read(file_descriptor, bytes_lus , sample_size); 
                // On envoie les bytes lu au client
                err_send = sendto(socket_descriptor, bytes_lus, strlen(bytes_lus) + 1,
                            0, (struct sockaddr*) &from, sizeof(struct sockaddr_in *));
                if(err_send < 0) {
                    return -1;
                }
                // Nettoyage du tableau bytes_lus
                bzero(bytes_lus, sample_size);
            }
    }
    // Message indiquant la fin de la transmission
    bzero(msg_send, 64);
    strcpy(msg_send, "fin");
    err_send = sendto(socket_descriptor, msg_send, strlen(msg_send) + 1,
                            0, (struct sockaddr*) &from, sizeof(struct sockaddr_in *));
    if(err_send < 0) {
        return -1;
    }
    close(socket_descriptor);
}
