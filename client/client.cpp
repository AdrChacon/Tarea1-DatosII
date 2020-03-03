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
#include<errno.h>


using namespace std;

/**
 * Corresponde al main del cliente, le permite al cliente probar el algoritmo
 * de dijkstra utilizando un grafo preexistente.
 * @return 
 */
int main(){
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        return 1;
    }
    
    int port = 54000;
    string ipAddress = "127.0.0.1";
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(),&hint.sin_addr);
    
    
    int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectResult == -1){
        
        return 2;
    }
    
    char hail[65];
    int serverHail = recv(sock, hail, 65, 0);
    
    cout << "Server response: " << string(hail, 65) << endl;
    
    char buf[4096];
    string userInput;
    
    
    while(true){
        
        cout << "> ";
        getline(cin, userInput);
        
        
        int sendRes = send(sock, userInput.c_str(), userInput.size()+1,0);
        if (sendRes == -1){
            cout << "Could not connect to server. \r\n";
            continue;
        }
        
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        
        
    };
    
    close(sock);
    
    return 0;
}
