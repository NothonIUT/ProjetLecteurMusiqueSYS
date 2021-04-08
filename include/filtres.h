// filtres.[ch]
// Déclaration des fonctions d'application des filtres 

/* Modifie le sample_rate afin d'accélerer ou ralentir la lecture de la musique.
*  La fonction renvoie -1 si le sample_rate est invalide (i.e s'il est négatif ou nul)
*  La fonction renvoie -2 si le facteur d'accélération est invalide (i.e si factor est inférieur à 0.3)
*  Cette limite a été choisie afin de ne pas trop allonger la durée de la musique jouée. 
*  La fonction renvoie le produit de sample_rate et factor sinon.
*/
int modify_song_speed(int factor, int sample_rate);

int stereo_to_mono(int play, int channels, unsigned short *bytes_lus, int file_descriptor, int sample_size);

int mono_to_stereo(int channels, unsigned short *bytes_lus, int file_descriptor, int sample_size);

int add_echo(char* bytes_lus, int audio_descriptor, int sample_rate, int sample_size, int channels);

int turn_up_volume(float factor, int sample_size, unsigned short *bytes_lu);