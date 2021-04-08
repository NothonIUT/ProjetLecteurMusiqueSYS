CC=gcc
CFLAGS= -Wall -g -I./include -c #include pour GCC, car option de compilation
EXEC=bin/lecteur bin/audioserver bin/audioclient


# $@: cible, $<: première dépendance , $^: toutes les dépendances

####################BINAIRES###############

bin/lecteur: obj/lecteur.o obj/audio.o
	$(CC) -o $@ $^
	
bin/audioserver: obj/audioserver.o obj/audio.o obj/serveurclient.o
	$(CC) -o $@ $^
	
bin/audioclient: obj/audioclient.o obj/audio.o obj/serveurclient.o
	$(CC) -o $@ $^

####################OBJETS###############
	
obj/lecteur.o: src/lecteur.c include/audio.h 
	$(CC) -o $@ $(CFLAGS) $<
	
obj/audioserver.o: src/audioserver.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

obj/audioclient.o: src/audioclient.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

obj/audio.o: src/audio.c include/audio.h
	$(CC) -o $@ $(CFLAGS) $<
	
obj/serveurclient.o: src/serveurclient.c include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<
	
# Remove files

clean :
	rm -rf obj/*.o
	
mrproper: clean
	rm -rf $(EXEC)
	
	
	
	#gcc -o audioclient -c audioclient.c bonne commande .c to binary
	# erreurs dans le serveurclient.c
