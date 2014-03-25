#include <SDL_net.h>

class Client {
protected:
    IPaddress ip; /* Server address */
    TCPsocket TCPsd;

public:
    UDPNetEnt* ent;
    Client(char* ipAddr, int port);
    ~Client();
};

Client::Client(char* ipAddr, int port) {
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
        //assert(false);
    }

    /* Resolve the host we are connecting to */
    if (SDLNet_ResolveHost(&ip, ipAddr, port) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
        //assert(false);
    }
 
    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(TCPsd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
        //assert(false);
    }

    ent = new UDPNetEnt(ipAddr, 32100, 49153);
}

Client::~Client() {
    SDLNet_Quit();
    delete ent;
}
