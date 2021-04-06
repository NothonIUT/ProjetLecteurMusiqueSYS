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

int stereo_to_mono(int channels){
    if (channels != 2){
        return -1;
    }

    return 1;
}

int add_echo(char* bytes_lus, int audio_descriptor, int sample_rate, int sample_size, int channels){
    char buffer1[sample_size];
    for (int i = 0; i < sample_size; i++)
    {
        buffer1[i] = bytes_lus[i];
    }

    write(aud_readinit, buffer1, sample_size);
    
}

int turn_up_volume(float factor, int sample_size, unsigned short *bytes_lu){
    float sample_float = (float) *bytes_lu;
    sample_float *= factor;
    *bytes_lu = (unsigned short) sample_float;
    return 0;
}