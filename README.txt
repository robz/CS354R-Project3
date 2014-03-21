Team 7
Thomas Custer
Robby Nevels
Steven Wilken

To Run
1. execute ./makeit and then run ./assignment3
2. If starting the server: enter the IP address and port of the client, as well as the port to listen to data incoming from the client, then hit the “Server” button
3. Likewise, if starting the client: enter the IP address and port of the server, as well as the port to listen to data incoming from the server, then hit the “Client” button

Code architecture
We have stuck with maintaining the same general game engine architecture as we had with Assignment 2. Rather than added a separate client and server class, we have created a generalized “UDPNetEnt” (UDP Network Entity) class to facilitate communication between the server and client with UDP. 

Changes for Multiplayer
At the moment, we have not implemented the ability for multiple players to play the same game. Instead, we focused on getting a robust network game connection between a client and server. We expect to make the same changes as we suggested our initial the design doc: adding a paddle and ball for each player that connects to the server.

How are you handling the client-server issues?
When we wrote the design doc, we were planning to use a central server to maintain physics, and then have each client be a separate program that corresponds to a separate player. The game state is centrally maintained by the server, and each client connects to the server to interact with the game physics. 

However, we have decided to instead allow the server correspond to a player as well. This is due to how our physics and graphics are so closely tied within the GameObject class. 

To go into more detail on our networking solution, here are the steps to will be completed for a 2-player game:
1. Server starts up, waits for the client to connect
2. Client connects to server using server’s address and port
3. TCP handshaking ensues, resulting in the server obtaining the client’s IP and port
4. The server begins the game

At the moment, we haven’t finished the TCP handshaking code, so the client IP and port are required for the server to run.

Game upgrades beyond the basic networking
We haven’t begun on extra upgrades besides the GUI elements for starting the client/server, but we still plan on adding them.

Division of Labor
Robby and Steven: Server/client issues
Thomas: Sound and GUI

