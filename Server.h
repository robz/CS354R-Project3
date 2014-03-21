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
#include <iterator>

#include <SDL_net.h>

class Server {
protected:
    UDPsocket UDPsd;
    TCPsocket TCPsd, TCPcsd;
    IPaddress srvadd, *remoteIP;
    UDPpacket *p;
    std::string clientAddr;

public:
    Server(int serverPort);
    ~Server();
    void sendMsg(char* data, int len);
    void awaitConnections();
};

Server::Server(int serverPort) {
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

 
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&srvadd, NULL, serverPort) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", 
            NULL, 
            serverPort, 
            SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if(!(TCPsd = SDLNet_TCP_Open(&srvadd)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
   
    if (!(UDPsd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
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

void Server::awaitConnections(){
    bool connect = false;
    while(!connect){
        if(TCPcsd = SDLNet_TCP_Accept(TCPsd)){
            /*can now communicate with client using csd socket*/

            /* Get the remote address */
            if(remoteIP = SDLNet_TCP_GetPeerAddress(TCPcsd)){
                /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
                connect = true;
            }
            else
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
        }
    }
}

void Server::sendMsg(char *data, int len) {
    static bool resolved = false;
    static IPaddress ip;
    /* Resolve server name  */
    if(!resolved){
        /* hacky way to get IPaddress */
        Uint8* addr = (Uint8*) &remoteIP->host;
        char clientAddr[100];
        sprintf(clientAddr, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
        if (SDLNet_ResolveHost(&ip, clientAddr, 49153) == -1)
        {
            fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", 
                SDLNet_Read32(&remoteIP->host), 
                49153, 
                SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
        else
            resolved = true;
    }

    p->address.host = ip.host;  /* Set the destination host */
    p->address.port = ip.port;            /* And destination port */

    p->len = len;
    memcpy(p->data, data, len);

    //printf("Sending to: %x %d\n", SDLNet_Read32(&p->address.host), p->address.port);

    int numsent = SDLNet_UDP_Send(UDPsd, -1, p); /* This sets the p->channel */
    if(!numsent){
        printf("SDLNet_UDP_SendV: %s\n", SDLNet_GetError());
    }
}
