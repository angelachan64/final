
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "map.h"

#define PORT 5000
#define FTOK_FILE "Makefile"
#define MAP_MEMORY_ID 0
#define PLAYERS_MEMORY_ID 1
#define UID_LENGTH 1

/**
 *  Removes the shared memory segments when the server is killed.
 */
static void clean_up_memory(int signo) {
  int e;
  int map_memory_segment, map_memory_key;
  int players_memory_segment, players_memory_key;

  map_memory_key = ftok(FTOK_FILE, MAP_MEMORY_ID);
  map_memory_segment = shmget(map_memory_key, MAP_MEMORY_SIZE, 0644);
  players_memory_key = ftok(FTOK_FILE, PLAYERS_MEMORY_ID);
  players_memory_segment = shmget(players_memory_key, 64, 0644);

  e = shmctl(map_memory_segment, IPC_RMID, NULL);
  printf("Map memory segment removed: %d\n", e);
  e = shmctl(players_memory_segment, IPC_RMID, NULL);
  printf("Players memory segment removed: %d\n", e);
  exit(0);
}

int main() {

  // Variable initialization.
  // Character buffer for storing information from the clients.
  char buffer[256];
  // Socket information for data transfer to clients.
  int socket_id, socket_client;
  // Shared memory to maintain the state of the server between the server forks.
  int map_memory_segment, map_memory_key;
  int players_memory_segment, players_memory_key;
  int* players;
  char* map;

  // Set up signal handler to clean up memory when we kill the server.
  signal(SIGINT, clean_up_memory);

  // Seed the random number generator.
  srand(time(NULL));

  // Create shared memory segments.
  map_memory_key = ftok(FTOK_FILE, MAP_MEMORY_ID);
  map_memory_segment = shmget(map_memory_key, MAP_MEMORY_SIZE, 0644 | IPC_CREAT);
  players_memory_key = ftok(FTOK_FILE, PLAYERS_MEMORY_ID);
  players_memory_segment = shmget(players_memory_key, 64, 0644 | IPC_CREAT);
  printf("Map memory segment created: %d\n", map_memory_segment);
  printf("Players memory segment created: %d\n", players_memory_segment);

  // Initialize the map
  map = shmat(map_memory_segment, 0, 0);
  players = shmat(players_memory_segment, 0, 0);
  (*players) = 1;
  init(map);

  // Create the socket that will allow client connections.
  socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // Bind to a specific port/address.
  struct sockaddr_in listener;
  listener.sin_family = AF_INET; // socket type IPv4
  listener.sin_port = htons(PORT); // port #
  listener.sin_addr.s_addr = INADDR_ANY; // bind to any incoming address
  bind(socket_id, (struct sockaddr *)&listener, sizeof(listener));

  // Begin listening on the specified port.
  listen(socket_id, 1);
  printf("Listening on port %d\n", PORT);

  // We will fork here to create a subprocess dedicated to listening for
  // connecting clients and a subprocess that will continually update the
  // internal state of the game.
  switch (fork()) {
  case -1:
    printf("Failed to fork!\n");
    return 0;

  case 0:
    // Child process which receive connecting clients and create a subserver
    // for each connection client.
    while (1) {
      // Wait for a client to join.
      socket_client = accept(socket_id, NULL, NULL);
      printf("Client connected: %d\n", socket_client);
      players = shmat(players_memory_segment, 0, 0);
      
      // Assign and send the client their player name / uid.
      sprintf(buffer, "%c", (char) (97 + *players));
      (*players)++;
      send(socket_client, buffer, sizeof(buffer), 0);

      // Fork off a subprocess to handle the client's input.
      switch (fork()) {
      case -1:
        printf("Failed to fork!\n");
        return 0;

      case 0:
        // Child process - subserver dedicated to this client's input.
        map = shmat(map_memory_segment, 0, 0);

        while (1) {
          recv(socket_client, buffer, sizeof(buffer), 0);
          strtok(buffer, "\n");
          send(socket_client, buffer, sizeof(buffer), 0);
        }
        break;

      default:
        // Parent process - close the socket client and wait for another client.
        close(socket_client);
      }
    }
    break;

  default:
    // Parent process - dedicated to continually updating the state of the game
    // at 60Hz.
    while (1) {
      update(map);
      usleep(16000);
    }
  }
  return 0;
}
