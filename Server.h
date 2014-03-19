#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h" 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

#include <SDL_net.h>

class Server {
protected:
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket *p;

public:
    Server(char* clientAddress, int clientPort);
    ~Server();
    void sendMsg(char* data, int len);
};

Server::Server(char* clientAddress, int clientPort) {
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Open a socket on random port */
    if (!(sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

 
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&srvadd, clientAddress, clientPort) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", 
            clientAddress, 
            clientPort, 
            SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Allocate memory for the packet */
    if (!(p = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}

void Server::sendMsg(char *data, int len) {
    p->address.host = srvadd.host;  /* Set the destination host */
    p->address.port = srvadd.port;  /* And destination port */

    p->len = len;
    memcpy(p->data, data, len);

    SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
}
