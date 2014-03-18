class Client {
protected:
    IPaddress serverIP; 
    SDLNet_SocketSet socketset;
    TCPsocket servsock;
    TCPsocket tcpsock;
    UDPsocket udpsock;

public: 
    Client(char* server, char* name);
    void sendMsgToServer(char* msg);
    ~Client();

protected:
    void sendHello(char* name);
    void cleanup(int exitcode);
};


Client::Client(char* server, char* name) :
socketset(NULL), servsock(NULL), tcpsock(NULL), udpsock(NULL)
{
    std::cout << "client!" << std::endl;
/*
    // Initialize SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        exit(1);
    }
*/
    /* Initialize the network */
    if ( SDLNet_Init() < 0 ) {
        fprintf(stderr, "Couldn't initialize net: %s\n", SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    /* Allocate a vector of packets for client messages */
    UDPpacket** packets = SDLNet_AllocPacketV(4, CHAT_PACKETSIZE);
    if ( packets == NULL ) {
        fprintf(stderr, "Couldn't allocate packets: Out of memory\n");
        cleanup(2);
    }

    /* Connect to remote host and create UDP endpoint */
    printf("Connecting to %s ... ", server);
    SDLNet_ResolveHost(&serverIP, server, CHAT_PORT);

    if ( serverIP.host == INADDR_NONE ) {
        printf("Couldn't resolve hostname\n");
    } else {
        /* If we fail, it's okay, the GUI shows the problem */
        tcpsock = SDLNet_TCP_Open(&serverIP);
        
        if ( tcpsock == NULL ) {
            printf("Connect failed\n");
        } else {
            printf("Connected\n");
        }
    }

    /* Try ports in the range {CHAT_PORT - CHAT_PORT+10} */
    for (int i=0; (udpsock == NULL) && i<10; ++i )
        udpsock = SDLNet_UDP_Open(CHAT_PORT+i);

    if ( udpsock == NULL ) {
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Couldn't create UDP endpoint\n");
    }

    /* Allocate the socket set for polling the network */
    socketset = SDLNet_AllocSocketSet(2);
    if ( socketset == NULL ) {
        fprintf(stderr, "Couldn't create socket set: %s\n",
        SDLNet_GetError());
        cleanup(2);
    }

    SDLNet_TCP_AddSocket(socketset, tcpsock);
    SDLNet_UDP_AddSocket(socketset, udpsock);

    printf("client success!\n");
    /* Run the GUI, handling network data */
    sendHello(name);
}

void Client::sendHello(char* name) {
    char data[512] = {0};

    data[0] = CHAT_HELLO;
    memcpy(&data[CHAT_HELLO_PORT], &serverIP.port, 2);
    memcpy(&data[CHAT_HELLO_NAME], name, strlen(name));

    SDLNet_TCP_Send(tcpsock, data, 512);
}

void Client::sendMsgToServer(char* msg) {
    SDLNet_TCP_Send(tcpsock, msg, strlen(msg));
}

void Client::cleanup(int exitcode) {
    std::cout << "client cleanup!" << std::endl;
   
    if (servsock != NULL) {
        SDLNet_TCP_Close(servsock);
        servsock = NULL;
    }
 
    if (tcpsock != NULL) {
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
    }
 
    if (udpsock != NULL) {
        SDLNet_UDP_Close(udpsock);
        udpsock = NULL;
    }
 
    if (socketset != NULL) {
        SDLNet_FreeSocketSet(socketset);
        socketset = NULL;
    }

    SDLNet_Quit();
    SDL_Quit();

    exit(exitcode);
}

Client::~Client() {
    cleanup(0);
}

