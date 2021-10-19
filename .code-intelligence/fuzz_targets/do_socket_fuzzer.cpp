#include <iostream>
#include <flibc.h>
#include <string.h>
#include <stdint.h>
#include "server_funcs.h"
#include <signal.h>

int StartServer(int argc, char**argv) {
  signal(SIGPIPE, SIG_IGN);
  MonoServer();
  return 0;
}


FUZZ_SERVER(StartServer, 5001, int client_fd, const uint8_t* data,
            size_t size) {
  write(client_fd, data, size);
}
