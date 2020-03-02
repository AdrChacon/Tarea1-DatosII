#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include<errno.h>


using namespace std;

int main(){
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        return 1;
    }
    
    // Create a hint structure to connect to server
    int port = 54000;
    string ipAddress = "127.0.0.1";
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(),&hint.sin_addr);
    
    
    // Connect to the server on the socket
    int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectResult == -1){
        
        return 2;
    }
    
    // While loop
    
    char buf[4096];
    string userInput;
    
    
    do {
        
        //      Enter lines
        cout << "> ";
        getline(cin, userInput);
        
        
        //      Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size()+1,0);
        if (sendRes == -1){
            cout << "Could not connect to server. \r\n";
            continue;
        }
        
        //      Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        
        //      Display response
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        
        
    } while(true);
    
    // Close socket
    close(sock);
    
    return 0;
}
