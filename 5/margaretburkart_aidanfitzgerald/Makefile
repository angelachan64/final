all: server server_setup client client_setup

# Executables
server: server.o user.o dir.o network.o
	gcc server.o dir.o user.o network.o -o server

server_setup: server_setup.c
	gcc server_setup.c -o server_setup

client: client.o network.o
	gcc client.o network.o -o client

client_setup: client_setup.c
	gcc client_setup.c -o client_setup

# Object files
client.o: client.c lib.h
	gcc -c client.c

server.o: server.c lib.h
	gcc -c server.c

dir.o: dir.c lib.h
	gcc -c dir.c

network.o: network.c lib.h
	gcc -c network.c

user.o: user.c lib.h
	gcc -c user.c

# Creates server directory structure
mail.d: server_setup
	./server_setup

# Creates client directory structure
my_mail.d: client_setup
	./client_setup

# Clean out object files
clean:
	rm *.o
