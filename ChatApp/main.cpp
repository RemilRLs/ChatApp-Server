#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;


int main() {

    int resultWinSock;

	// Initialisation of winsocket.

    WSADATA WSAData;
    resultWinSock = WSAStartup(MAKEWORD(2, 0), &WSAData);

    if (resultWinSock != 0) {

        cerr << "[X] - WSAStartup failed to launch." << endl;
        cerr << "[X] - Error code for WSAStartup is : " << resultWinSock << endl;

        return 1;
    }

    // Creation of the socket.

    SOCKET sock;
    sockaddr_in sin;


    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        cerr << "[X] - Invalid Socket ! Please retry." << endl;
        return 1;
    }

    // Link of the IP Adress and port to the socket that we just create.

    sin.sin_family = AF_INET; // AF_INET is a property of the socket.
    sin.sin_port = htons(51000); // Host to network short. Listening port.
    sin.sin_addr.S_un.S_addr = INADDR_ANY; // We want any IP that come from there because that a server.
   
                                           
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));
            
    // We give the role of that socket (It will be a server) so he will have to listen.

    listen(sock, SOMAXCONN); // BACKLOG Inlimed connections.
    int value = 0;
    
    // Connection Queue. Creation of a client.

    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket;
    sockaddr_in sinClient;

    /*
     - The function accept is a function that we allow us to communicate with a server socket.
     - He will accept the socket of the client.    
    */

    clientSocket = accept(sock, (SOCKADDR*)&sinClient, &clientSize);
    
    if (clientSocket == INVALID_SOCKET) {
        cerr << "[X] - Invalid Socket ! Please retry." << endl;
        return 1;
    }

    char host[NI_MAXHOST]; // Client name.
    char service[NI_MAXSERV]; // Port where the user is connected.

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    
    if (getnameinfo((SOCKADDR*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {

        cout << host << " connected on port " << service << endl;

    } 
    else { // If we don't find any information about the client we do something else.

        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

        cout << host << " connected on port " << ntohs(client.sin_port) << endl; 

    }


    // Stop the listening of the socket.

    closesocket(sock);

    // Shutdown of winsocket.

    WSACleanup();


	return 0;
}