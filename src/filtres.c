#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <string.h>
#include "../include/audio.h"
#include "../include/filtres.h"

int modify_song_speed(int factor, int sample_rate){
    
    // Vérification de la valeur de sample_rate
    if(sample_rate < 0){
        return -1;
    }
    
    // Vérification de la valeur de factor
    if(factor < 0.3){
        return -2;
    }

    return sample_rate*factor;
}

int stereo_to_mono(int play, int channels, unsigned short *bytes_lus, int file_descriptor, int sample_size){
    int must_be_played = play;
    if (channels != 1)
        channels = 1;

    if (must_be_played > 1 || must_be_played < 0)
        return -1;

    // Si l'extrait ne doit pas être joue, on lit le suivant et on passe must_be_played à 1
    if (must_be_played == 0){
        read(file_descriptor, bytes_lus , sample_size);
        return 1;
    }

    // Si l'extrait doit etre joue, on se contente de passer must_be_played à 0
    if(must_be_played == 1)
        return 0;

    return -1;
}

int mono_to_stereo(int channels, unsigned short *bytes_lus, int file_descriptor, int sample_size){
    // TODO
}


int add_echo(char* bytes_lus, int audio_descriptor, int sample_rate, int sample_size, int channels){
    // TODO
}

int turn_up_volume(float factor, int sample_size, unsigned short *bytes_lu){
    float sample_float = (float) *bytes_lu;
    sample_float *= factor;
    *bytes_lu = (unsigned short) sample_float;
    return 0;
}