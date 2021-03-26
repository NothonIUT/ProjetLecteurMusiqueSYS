CC=gcc
CFLAGS=-Wall -g -I./include -c #include pour GCC, car option de compilation
DEP=include/audio.h include/serveurclient.h
OBJ=obj/lecteur.o obj/audio.o
EXEC=bin/lecteur

# Objects of PROJECT
# $@: cible, $<: première dépendance , $^: toutes les dépendances

$(EXEC): $(OBJ)
	$(CC) -o $@ $^
	
obj/lecteur.o: src/lecteur.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

obj/audio.o: src/audio.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

obj/audioserveur.o: src/audioserveur.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

obj/audioclient.o: src/audioclient.c include/audio.h include/serveurclient.h
	$(CC) -o $@ $(CFLAGS) $<

# Remove files

clean :
	rm -rf obj/*.o
	
mrproper: clean
	rm -rf $(EXEC)
