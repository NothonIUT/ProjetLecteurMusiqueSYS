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
#include <arpa/inet.h>
#include "../include/audio.h"
#include "../include/serveurclient.h"
#include "../include/filtres.h"


int main(int argc, char** args) {
	// args[1] = nom du fichier, args[2] = ip du serveur, args[3] = nom du filtre
	printf("Création du socket...\n");
	struct sockaddr_in addr;
	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(client_socket < 0) {
		printf("Erreur lors de la création du socket client !\n");
		return -1;
	}

	printf("Assignation du port...\n");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr(args[2]); //148.60.3.90
	/*if(bind(client_socket, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Erreur lors de l'assignation du port !\n");
		return -1;
	}*/  /* d'après le cours, il n'y en aurait pas besoin ici. */ 

	printf("Envoi du datagramme...\n");

	if(sendto(client_socket, args[1], strlen(args[1]) + 1, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Erreur lors de l'envoi du datagramme !\n");
		return -1;
	}

	// Seul le filtre stereo_to_mono est appliqué côté client, c'est donc le seul nom de filtre à ne pas avoir besoin d'être envoyé
	char nomFiltre[32];
	strcpy(nomFiltre, args[3]);
	char arguments[64];
	strcpy(nomFiltre, args[1]);
	if (strcmp(nomFiltre, "stereo_to_mono") == 1){
		strcat(args[1], "|");
		strcat(arguments, nomFiltre);
	}

	if(sendto(client_socket, arguments, strlen(arguments) + 1, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Erreur lors de l'envoi du datagramme !\n");
		return -1;
	}
	////////// Réception des paquets du serveur //////////
	int sample_rate, sample_size, channels = 0;
	struct sockaddr_in from;

	printf("En attente de la réception des informations de la musique...\n");

	int infos = reception_infos_musique(&from, client_socket, &sample_rate, &sample_size, &channels);
	if( infos < 0) {
		printf("Erreur lors de la réception des informations de la musique !\n");
	}

	printf("Réglage du lecteur...\n");
	if(strcmp(nomFiltre, "stereo_to_mono")){
		channels = 1;
	}

	int audio_descriptor = aud_writeinit(sample_rate, sample_size, channels);
	if (audio_descriptor < 0) {
		printf("Erreur du audio_descriptor\n");
		return -1;
		}

	socklen_t len, flen;
	unsigned short bytes_lus[sample_size]; // Tableau dans lequel les octets lus seront stockés pour être écrits dans le lecteur

	printf("Lecture de la musique...\n");
	int play = 1;
	do {
		len = recvfrom(client_socket, bytes_lus, sizeof(bytes_lus), 0, (struct sockaddr *) &from, &flen);
		if(len < 0) {
			printf("Erreur lors de la réception d'une partie de la musique !\n");
			return -1;
		}
		play = stereo_to_mono(play, bytes_lus, sample_size);
		write(audio_descriptor, bytes_lus, sample_size);
			// Nettoyage du tableau bytes_lus
			bzero(bytes_lus, sample_size);
	} while(strcmp((char*) bytes_lus, "fin") == 1);

	printf("Lecture terminée !\n");
	close(client_socket);
}