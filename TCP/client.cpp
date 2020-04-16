#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return 1;
    }

    sockaddr_in  hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);

    int connectRes = connect(sock, (sockaddr*)& hint, sizeof(hint));
    if (connectRes == -1) {
        return -2;
    }

    char buf[4096];
    string userInput;
    do {
        cout << '>';
        getline(cin, userInput);
        send(sock, userInput.c_str(), sizeof(userInput) + 1, 0);

        memset(buf, 0, 4096);

        int byteRecv = recv(sock, buf, sizeof(buf) + 1, 0);
        if (byteRecv == -1) {
            cout << "Could not receive message, OOPS!";
        }
        cout << "SERVER>" << string(buf, byteRecv) << "\r\n";
    }
    while (true);

    close(sock);
}