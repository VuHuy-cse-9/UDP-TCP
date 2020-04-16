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
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr <<"Can't creat socket a socket";
        return -1;
    }
// Bind the ip address and port to a socket    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        cerr << "Can't bind the hind to IP/Port";
        return -2;
    }
    if (listen(listening, SOMAXCONN) == -1) {
        cerr << "Can't listen!";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1) {
        cerr << "Problem with client connecting";
        return -4;
    }

    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0,  NI_MAXSERV);

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

    char buf[4096];
    //Enter a loop
    while (true) {
        memset(buf, 0, 4096);
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1) {
            cerr << "There were connnection issue";
            break;
        }
        if (bytesRecv == 0) {
            cerr << "The client disconnected" << endl;
            break;
        }
        //Wait message
        //Display message and client info
        cout << "Received: " << string(buf, 0, bytesRecv) << endl;

        send(clientSocket, buf, bytesRecv + 1, 0);
    }

    //close socket
    close(clientSocket);
}