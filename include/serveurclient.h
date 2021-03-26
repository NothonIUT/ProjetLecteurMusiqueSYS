/** Fonction servant à envoyer les informations sur la musique au client.
 * La fonction renvoie le descripteur du fichier si toutes les informations ont été récoltées et envoyées
 * et renvoie -1 sinon
 * @param sample_rate       le nombre d'échantillons par seconde
 * @param sample_size       l'amplitude de chaque échantillon
 * @param channels          nombre de channels (mono ou stéréo)
 * @param filename          nom du fichier à jouer
 * @param socket_descriptor descripteur de la socket
 * @param from              structure où l'on retrouvera l'adresse source du datagramme
*/
int envoi_infos_musique(int sample_rate, int sample_size, int channels, char* filename, int socket_descriptor, struct sockaddr_in from);

/** Fonction servant à réceptionner les informations sur la musique du serveur.
 * La fonction renvoie 0 si toutes les informations ont été correctement réceptionnées
 * et renvoie -1 sinon
 * @param from              structure où l'on retrouvera l'adresse source du datagramme
 * @param client_socket     descripteur de la socket du client
 * @param sample_rate       le nombre d'échantillons par seconde
 * @param sample_size       l'amplitude de chaque échantillon
 * @param channels          nombre de channels (mono ou stéréo)
*/
int reception_infos_musique(struct sockaddr_in *from, int client_socket, int *sample_rate, int *sample_size, int *channels);