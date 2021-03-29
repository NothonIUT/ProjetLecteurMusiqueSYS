#include <stdint.h>
#include <stdio.h>
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