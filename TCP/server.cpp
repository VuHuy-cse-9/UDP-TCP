#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>
using namespace std; //Save from typing std;

int main() {
    // Create a socket
    //AF_INET = IPv4: Internet protocol
    //SOCK_STREAM: TCP
    //0: Protocol apply for socket (when using with type family protocol)
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr <<"Can't creat socket a socket";
        return -1;
    }

    // Bind the ip address and port to a socket    
    //sockaddr_in: Socket address include IP address, hostname and port
    //inet_pton: Convert IP address form text to binary (For transmit in internet)
    sockaddr_in hint; // serverSocket
    hint.sin_family = AF_INET;//IPV4
    hint.sin_port = htons(54000); //serverPort
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    //We start listening from the beginning
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        cerr << "Can't bind the hind to IP/Port";
        return -2;
    }
    if (listen(listening, SOMAXCONN) == -1) {
        cerr << "Can't listen!";
        return -3;
    }

    // Wait for a connection
    sockaddr_in client; //connectionSocket
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    //accept: When connection, open a new socket.Have to specify ADDR_LEN(clientsSize)
    //and accept return a socket description or -1 if failed
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1) {
        cerr << "Problem with client connecting";
        return -4;
    }

    // Close listening socket
    close(listening);
    //memset: Fill first n byte of memmory area that is pointed by void* ptr(host, svc)
    //and replace them with const c (NI_MAXHOST,   NI_MAXSERV)
    memset(host, 0, NI_MAXHOST);//sentenceFromHost
    memset(svc, 0,  NI_MAXSERV);//sentenceFromServer

    int result = getnameinfo((sockaddr*)& client, 
                            sizeof(client), 
                            host, 
                            NI_MAXHOST, 
                            svc, 
                            NI_MAXSERV, 
                            0);
    if (result)
    {
        cout << host << "connected on " << svc << endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host,  NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }
    // While loop: accept and echo message back to client
    char buf[4096];

    while (true) {
        memset(buf, 0, 4096);

        // Wait for client to send data
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1) {
            cerr << "There were connnection issue";
            break;
        }

        if (bytesRecv == 0) {
            cerr << "The client disconnected" << endl;
            break;
        }

        cout << "Received: " << string(buf, 0, bytesRecv) << endl;
        //Display message and client info
        send(clientSocket, buf, bytesRecv + 1, 0);//socket send
    }

    //close socket
    close(clientSocket);

    return 0;
}