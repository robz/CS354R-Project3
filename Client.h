#include <SDL_net.h>

class Client {
protected:
    UDPsocket sd;
    UDPpacket *p;

public:
    Client(int port);
    ~Client();
    
    // returns success
    bool recMsg(char* data);
};

Client::Client(int port) {
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Open a socket */
    if (!(sd = SDLNet_UDP_Open(port)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Make space for the packet */
    if (!(p = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

// returns success
bool Client::recMsg(char* data) {
    if (SDLNet_UDP_Recv(sd, p))
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
