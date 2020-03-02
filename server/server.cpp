// 
// Este codigo fue obtenido de https://www.youtube.com/channel/UC4LMPKWdhfFlJrJ1BHmRhMQ
//


#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
    // Create socket
    int listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listening == -1){
        cerr << "Can't create socket";
        return -1;
    }
    
    // Bind the socket to an IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    //hint.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "Can't bind to IP/Port";
        return -2;
    }
    
    // Mark the socket for listening in
    if (listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen";
        return -3;
    }
    
    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    
    if (clientSocket == -1){
        cerr << "Problem with client connecting";
        return -4;
    }
    
    // Close listening socket
    close(listening);
    
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    
    int result = getnameinfo((sockaddr*)&client, sizeof(client),host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result){
        cout << host << " connected on " << svc << endl;
    } else {
        inet_ntop(AF_INET,&client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }
    
    
    
    // While receiving: display message, echo message
    char buf[4096];
    while(true){
        memset(buf, 0, 4096);
        
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1){
            cerr << "There was a connection issue" << endl;
            break;
        }
        
        if (bytesRecv == 0){
            cout << "Client disconnected" << endl;
            break;
        }
        // Display message received
        cout << "Test: " << string(buf,0,bytesRecv) << endl;
        
        
        
        cout << "Received: " << string(buf, 0, bytesRecv) << endl;
        
        // Send message
        send(clientSocket, text.c_str(), sizeof(text), 0);
        
    }
    
    // Close socket
    close(clientSocket);
            
    return 0;
}
