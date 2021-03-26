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

/*
typedef struct packet {
	char message[64];
} packet;

void create_packet(struct packet* init, char* content) {
	memcpy(init->message, content, 64);
}

void clear_packet(struct packet* to_clear) {
	bzero(to_clear->message, 64);
} */

int reception_infos_musique(struct sockaddr_in *from, int client_socket, int *sample_rate, int *sample_size, int *channels) {

	socklen_t len, flen;
	char message_recu[64];

	flen = sizeof(struct sockaddr_in);

	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet sample_rate !\n");
		return -1;
	}
	sample_rate = atoi(message_recu);
	bzero(message_recu, 64);

	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet sample_size!\n");
		return -1;
	}
	sample_size = atoi(message_recu);
	bzero(message_recu, 64);

	len = recvfrom(client_socket, message_recu, sizeof(message_recu), 0, (struct sockaddr *) from, flen);
	if(len < 0) {
		printf("Erreur lors de la réception du paquet channels!\n");
		return -1;
	}
	channels = atoi(message_recu);
	bzero(message_recu, 64);

	return 0;	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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



/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Déclaration et initialisation du socket client
	struct sockaddr_in destination;
	int client_socket = init_socket(args[1], &destination);

	// Paramètres pour la transmission
	struct packet to_server;
	struct packet to_client;
	socklen_t destination_length = (socklen_t)sizeof(struct sockaddr);

	// Paramètres de lecture du lecteur audio
	int sample_rate, sample_size, channels;
	int write_audio, write_init_audio = 0;

	// Timeout
	fd_set watch_over;
	struct timeval timeout;
    int code;


	// Envoi du filename
	create_packet(&to_server, args[2]);
	if (sendto(client_socket, &to_server, sizeof(struct packet), 0, (struct sockaddr*)&destination, destination_length) == -1) {
		perror("Erreur: Filename_packet !");
		return 1;
	}


	// Echange avec audioserver
	do {

		// Réinitialisation de fd set
		FD_ZERO(&watch_over);
		FD_SET(client_socket, &watch_over);
		timeout.tv_sec = 0;
		timeout.tv_usec = 20000;  // 200ms
	    code = select(client_socket+1, &watch_over, NULL, NULL, &timeout);

		// Clear packets
		clear_packet(&to_server);
		clear_packet(&to_client);

		// Erreur durant la sélection
		if (code < 0) {
			perror("");
			return 1;
		}

		// Si timeout, on redemande le paquet
		if (code == 0) {
			to_server.type = 5; // redemander le même paquet
			if (sendto(client_socket, &to_server, sizeof(struct packet), 0, (struct sockaddr*)&destination, destination_length) == -1) {
				perror("Can't request same packet");
				return 1;
			}
		} */
