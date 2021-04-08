#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../include/audio.h"
#include "../include/serveurclient.h"


int envoi_infos_musique(int sample_rate, int sample_size, int channels, char* filename, int socket_descriptor, struct sockaddr_in from) {
    // Lecture des caractéristiques du fichier et récupération du descripteur du fichier
    int file_descriptor = aud_readinit(filename, &sample_rate, &sample_size, &channels);
    puts("balise1"); // Celle-ci passe
    char msg_send[64];
    // Envoie de sample_rate
        bzero(msg_send, 64);
        strcpy(msg_send, sample_rate);
        int err_send = sendto(socket_descriptor, msg_send, strlen(msg_send) + 1,
                            0, (struct sockaddr*) &from, sizeof(struct sockaddr_in *));
        if(err_send < 0){
            return -1;
        }   

        // Envoie de sample_size
        bzero(msg_send, 64);
        strcpy(msg_send, sample_size);
        err_send = sendto(socket_descriptor, msg_send, strlen(msg_send) + 1,
                            0, (struct sockaddr*) &from, sizeof(struct sockaddr_in *));
        if(err_send < 0){
            return -1;
        } 

        // Envoie de channels  
        bzero(msg_send, 64);
        strcpy(msg_send, channels);
        err_send = sendto(socket_descriptor, msg_send, strlen(msg_send) + 1,
                            0, (struct sockaddr*) &from, sizeof(struct sockaddr_in *));
        if(err_send < 0){
            return -1;
        }  

    return file_descriptor; 
}

int reception_infos_musique(struct sockaddr_in *from, int client_socket, int *sample_rate, int *sample_size, int *channels) {

	socklen_t len, flen;
	char message_recu[64];

	flen = sizeof(struct sockaddr_in);

    // Réception et écriture de sample_rate
	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet sample_rate !\n");
		return -1;
	}
	sample_rate = atoi(message_recu);
	bzero(message_recu, 64);

    // Réception et écriture de sample_size
	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet sample_size!\n");
		return -1;
	}
	sample_size = atoi(message_recu);
	bzero(message_recu, 64);

    // Réception et écriture de channels
	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet channels!\n");
		return -1;
	}
	channels = atoi(message_recu);
	bzero(message_recu, 64);

	return 0;	
}
