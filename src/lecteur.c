#include <stdint.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <string.h>
#include "../include/audio.h"

#define DEFAULT_PATH "/home/nathan/Documents/SYS/Projet/data/" // A changer selon l'emplacement du projet sur votre machine
#define MAX_FILENAME_LENGTH 512 // Longueur maximale du chemin jusqu'au fichier

int main() {
    
    /* On demande le nom du fichier à l'utilisateur. Pour plus de simplicité, on impose le dossier 
    * dans lequel la musique doit se trouver pour la lecture de fichiers locaux
    * Il faut donc changer DEFAULT_PATH en fonction de l'emplacement du projet sur votre machine
    */
    
    printf("Entrez le nom du fichier.\n(Il doit être présent dans le dossier data et terminer par .wav :\n");
    char input[MAX_FILENAME_LENGTH];
    scanf("%s", input); // Lecture de l'entrée utilisateur
    char filename[MAX_FILENAME_LENGTH]; // Déclaration de la variable qui contiendra le chemin complet jusqu'au fichier
    strcpy(filename, DEFAULT_PATH);
    strcat(filename, input);

    int sample_rate = 0;
    int sample_size = 0;
    int channels = 0;

    // Lecture des caractéristiques du fichier et récupération du descripteur du fichier
    int file_descriptor = aud_readinit(filename, &sample_rate, &sample_size, &channels);

    if (file_descriptor == -1) {
       printf("Erreur du file_descriptor\n");
       return -1;
    }

    // Initialisation du descripteur du lecteur audio
    int audio_descriptor = aud_writeinit(sample_rate, sample_size, channels);
    
    if (audio_descriptor == -1) {
       printf("Erreur du audio_descriptor\n");
       return -1;
    }
    
    char bytes_lus[sample_size]; // Tableau dans lequel les octets lus seront stockés pour être écrits dans le lecteur
    ssize_t nbr_bytes_lu = sample_size, nbr_bytes_ecrits=sample_size; // Variables contenant le nombre de bytes écrits/lus

    /* Pour lire la musique, il faut d'abord lire les octets dans le fichier .wav
    *  puis les écrires dans le lecteur audio.
    *  Le tableau bytes_lus est vidé à chaque tour de boucle par sécurité.
    *  Lorsque le fichier arrive à son terme, le nombre de bytes lus ne correspondra plus 
    *  à la taille des échantillons et on sort de la boucle
    */
    while(nbr_bytes_lu == sample_size && nbr_bytes_ecrits == sample_size) {
        // Lecture des octets dans le fichier.wav
        nbr_bytes_lu = read(file_descriptor, bytes_lus , sample_size); 
        // Ecriture de ces octets dans le lecteur audio
        nbr_bytes_ecrits = write(audio_descriptor, bytes_lus, sample_size);
        // Nettoyage du tableau bytes_lus
        bzero(bytes_lus, sample_size);
    }

    printf("Lecture terminée");
}
