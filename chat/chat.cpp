#include <iostream>
#include "SDL_net.h"

#include "settings.h"
#include "server.h"
#include "client.h"

int main(int argc, char* argv[]) {
    if (argc == 2 && argv[1][0] == '0') {
        Server server;
        server.waitForClients();
    } else if (argc == 4 && argv[1][0] == '1') {
        Client client(argv[2], argv[3]);
        for (;;);
    } else {
        fprintf(stderr, "bad args\n");
        exit(1);
    } 

    return 0;
}
