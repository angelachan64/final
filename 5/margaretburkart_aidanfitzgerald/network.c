#include "lib.h"

int sock_write_n(int sock, char *str, int n) {
  int status;

  // Write four bytes for size
  n = htonl(n);
  status = write(sock, &n, sizeof(n));

  if (status < sizeof(n)) {
    
    return -2;
  }

  // Write the string itself
  status = write(sock, str, n);
  return status;
}

int sock_write(int sock, char *str) {
  return sock_write_n(sock, str, strlen(str));
}

char *sock_read(int sock) {
  unsigned int size;
  if (read(sock, &size, sizeof(size)) < 4) return NULL;
  size = ntohl(size);

  char *buffer = malloc(size + 1);
  int actual_bytes_read = read(sock, buffer, size);
  buffer[actual_bytes_read] = 0;

  return buffer;
}
