struct Person {
    int active;
    char name[256];
    TCPsocket sock;
    IPaddress peer;
};

class Server {
protected:
    Person people[CHAT_MAXPEOPLE];
    SDLNet_SocketSet socketset;
    TCPsocket servsock;
    IPaddress serverIP; 

public:
    Server();
    void waitForClients(void);
    ~Server();

protected:
    void handleClient(int i);
    void handleServer(void);
    void SendNew(int about, int to);
    void cleanup(int exitcode);
};


Server::Server() :
socketset(NULL), servsock(NULL)
{
    std::cout << "server..." << std::endl;
    
    if (SDL_Init(0) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        exit(1);
    }

    // Initialize the network
    if (SDLNet_Init() < 0) {
        fprintf(stderr, "Couldn't initialize net: %s\n", SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    // Initialize the channels
    for (int i = 0; i < CHAT_MAXPEOPLE; ++i) {
        people[i].active = 0;
        people[i].sock = NULL;
    }

    // Allocate the socket set
    socketset = SDLNet_AllocSocketSet(CHAT_MAXPEOPLE + 1);
    if (socketset == NULL) {
        fprintf(stderr, "Couldn't create socket set: %s\n", SDLNet_GetError());
        cleanup(2);
    }
   
    /* Create the server socket */
    SDLNet_ResolveHost(&serverIP, NULL, CHAT_PORT);
    printf("Server IP: %x, %d\n", serverIP.host, serverIP.port);
    servsock = SDLNet_TCP_Open(&serverIP);
    
    if (servsock == NULL) {
        fprintf(stderr, "Couldn't create server socket: %s\n", SDLNet_GetError());
        cleanup(2);
    }

    SDLNet_TCP_AddSocket(socketset, servsock);
}

void Server::handleServer(void) {
    TCPsocket newsock;
    int which;
    unsigned char data;
    
    newsock = SDLNet_TCP_Accept(servsock);
    
    if ( newsock == NULL ) { return; }
    
    /* Look for unconnected person slot */
    for ( which=0; which<CHAT_MAXPEOPLE; ++which ) {
        if ( ! people[which].sock ) { break; }
    }
    
    if ( which == CHAT_MAXPEOPLE ) {
        /* Look for inactive person slot */
        for ( which=0; which<CHAT_MAXPEOPLE; ++which ) {
            if ( people[which].sock && ! people[which].active ) {
                /* Kick them out.. */
                data = CHAT_BYE;
                SDLNet_TCP_Send(people[which].sock, &data, 1);
                SDLNet_TCP_DelSocket(socketset, people[which].sock);
                SDLNet_TCP_Close(people[which].sock);
                break;
            }
        }       
    }
    
    if ( which == CHAT_MAXPEOPLE ) {
        /* No more room... */
        data = CHAT_BYE;
        SDLNet_TCP_Send(newsock, &data, 1);
        SDLNet_TCP_Close(newsock);
    } else {
        /* Add socket as an inactive person */
        people[which].sock = newsock;
        people[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
        SDLNet_TCP_AddSocket(socketset, people[which].sock);
    }
}

/* Send a "new client" notiﬁcation */
void Server::SendNew(int about, int to) {
    char data[512];
    int n;
    n = strlen((char *)people[about].name)+1;
    data[0] = CHAT_ADD;
    data[CHAT_ADD_SLOT] = about;
    memcpy(&data[CHAT_ADD_HOST], &people[about].peer.host, 4);
    memcpy(&data[CHAT_ADD_PORT], &people[about].peer.port, 2);
    data[CHAT_ADD_NLEN] = n;
    memcpy(&data[CHAT_ADD_NAME], people[about].name, n);
    SDLNet_TCP_Send(people[to].sock, data, CHAT_ADD_NAME+n);
}

void Server::handleClient(int which) {
    std::cout << "handling new client..." << std::endl;
    
    char data[512];
    int i;
    
    /* Has the connection been closed? */
    if ( SDLNet_TCP_Recv(people[which].sock, data, 512) <= 0 ) {
        /* Notify all active clients */
        if ( people[which].active ) {
            people[which].active = 0;
            data[0] = CHAT_DEL;
            data[CHAT_DEL_SLOT] = which;
            for ( i=0; i<CHAT_MAXPEOPLE; ++i )
                if ( people[i].active )
                    SDLNet_TCP_Send(people[i].sock,data,CHAT_DEL_LEN);
        }
        
        SDLNet_TCP_DelSocket(socketset, people[which].sock);
        SDLNet_TCP_Close(people[which].sock);
        people[which].sock = NULL;
    } else {
        switch (data[0]) {
            case CHAT_HELLO: {
                /* Yay! An active connection */
                memcpy(&people[which].peer.port, &data[CHAT_HELLO_PORT], 2);
                memcpy(people[which].name, &data[CHAT_HELLO_NAME], 256);
                people[which].name[256] = 0;
                /* Notify all active clients */
                for ( i=0; i<CHAT_MAXPEOPLE; ++i )
                    if ( people[i].active ) { SendNew(which, i); }
                /* Notify about all active clients */
                people[which].active = 1;
                for ( i=0; i<CHAT_MAXPEOPLE; ++i )
                    if ( people[i].active ) { SendNew(i, which); }
            }
            break;
            default: { /* Unknown packet type?? */; }
            break;
        }
    }
}

void Server::waitForClients(void) {
    /* Loop, waiting for network events */
    for (;;) {
        /* Wait for events */
        SDLNet_CheckSockets(socketset, ~0);

        /* Check for new connections */
        if (SDLNet_SocketReady(servsock)) {
            handleServer();
        }

        /* Check for events on existing clients */
        for (int i = 0; i < CHAT_MAXPEOPLE; ++i) {
            if (SDLNet_SocketReady(people[i].sock)) {
                handleClient(i);
            }
        }
    }
}

void Server::cleanup(int exitcode) {
    std::cout << "server cleanup!" << std::endl;
   
    if (servsock != NULL) {
        SDLNet_TCP_Close(servsock);
        servsock = NULL;
    }
 
    if (socketset != NULL) {
        SDLNet_FreeSocketSet(socketset);
        socketset = NULL;
    }

    SDLNet_Quit();
    SDL_Quit();

    exit(exitcode);
}

Server::~Server() {
    cleanup(0);
}

