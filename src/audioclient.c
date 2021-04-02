/* To make audio playback work on modern Linux systems:
   - Start your audio client with "padsp audioclient" instead of just "audioclient"
   - Or set $LD_PRELOAD to libpulsedsp.so
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <audio.h>
#include <netdb.h>
#include "../include/serveurclient.h"


int main(int argc, char** args) {

	struct sockaddr_in addr;
	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(client_socket < 0) {
		printf("Erreur lors de la création du socket client !\n");
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr(INADDR_ANY);

	if(bind(client_socket, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Erreur lors de l'assignation du port !\n");
		return -1;
	}

	if(sendto(client_socket, args[1], strlen(args[1]) + 1, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Erreur lors de l'envoi du datagramme !\n");
		return -1;
	}

	////////// Réception des paquets du serveur //////////
	int sample_rate, sample_size, channels = 0; 
	struct sockaddr_in from;

	int infos = reception_infos_musique(&from, client_socket, &sample_rate, &sample_size, &channels);
	if( infos < 0) {
		printf("Erreur lors de la réception des informations de la musique !\n");
	}

	int audio_descriptor = aud_writeinit(sample_rate, sample_size, channels);
	if (audio_descriptor < 0) {
		printf("Erreur du audio_descriptor\n");
		return -1;
		}

	socklen_t len, flen;
	char bytes_lus[sample_size]; // Tableau dans lequel les octets lus seront stockés pour être écrits dans le lecteur
	ssize_t nbr_bytes_lu = sample_size, nbr_bytes_ecrits = sample_size; // Variables contenant le nombre de bytes écrits/lus

	do {
		len = recvfrom(client_socket, bytes_lus, sizeof(bytes_lus), 0, (struct sockaddr *) &from, &flen);
		if(len < 0) {
			printf("Erreur lors de la réception d'une partie de la musique !\n");
			return -1;
		}

		nbr_bytes_ecrits = write(audio_descriptor, bytes_lus, sample_size);
			// Nettoyage du tableau bytes_lus
			bzero(bytes_lus, sample_size);
	} while(bytes_lus != "fin");

	printf("Lecture terminée \n");
	close(client_socket);
}