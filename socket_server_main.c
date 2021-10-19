#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server_funcs.h"

int main(int argc, char *argv[]) {
  MonoServer();
  return 0;
}
