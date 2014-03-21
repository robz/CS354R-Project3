#include <SDL_net.h>

class Client {
protected:
    IPaddress ip; /* Server address */
    UDPsocket UDPsd; /* Socket descriptor */
    TCPsocket TCPsd;
    UDPpacket *p;

public:
    Client(char* ipAddr, int port);
    ~Client();
    
    // returns success
    bool recMsg(char* data);
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

    /* Open a socket on random port? */
    if (!(UDPsd = SDLNet_UDP_Open(49153)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
        //assert(false);
    }
 
    /* Make space for the packet */
    if (!(p = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
        //assert(false);
    }
}

// returns success
bool Client::recMsg(char* data) {
    if (SDLNet_UDP_Recv(UDPsd, p))
    {
        /*
        printf("UDP Packet incoming\n");
        printf("\tChan:    %d\n", p->channel);
        printf("\tData:    %s\n", (char *)p->data);
        printf("\tLen:     %d\n", p->len);
        printf("\tMaxlen:  %d\n", p->maxlen);
        printf("\tStatus:  %d\n", p->status);
        printf("\tAddress: %x %x\n", p->address.host, p->address.port);
        */
        memcpy(data, (char*)p->data, p->len);
        return true;
    } else {
        return false;
    }
}

Client::~Client() {
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}
