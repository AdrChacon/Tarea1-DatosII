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
#include "graph.h"

using namespace std;

/**
 * Corresponde al main del servidor, inicialmente se crea un grafo de 6 nodos.
 * Despues de aceptar una conexion de un socket cliente, espera una respuesta
 * del cliente, que corresponde a el nodo inicial con el que se busca probar
 * el algoritmo de dijkstra.
 * @return 
 */
int main()
{
    int V = 6;
    Graph* graph = createGraph(V);
    addEdge(graph, 0, 3, 3); 
    addEdge(graph, 1, 0, 2); 
    addEdge(graph, 2, 5, 3); 
    addEdge(graph, 3, 1, 4); 
    addEdge(graph, 3, 2, 7); 
    addEdge(graph, 4, 1, 5); 
    addEdge(graph, 4, 0, 1);
    addEdge(graph, 5, 4, 2);
    
 
    
    
    int listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listening == -1){
        cerr << "Can't create socket";
        return -1;
    }
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "Can't bind to IP/Port";
        return -2;
    }
    
    if (listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen";
        return -3;
    }
    
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    
    if (clientSocket == -1){
        cerr << "Problem with client connecting";
        return -4;
    }
    int counter = 1;
    send(clientSocket, "Welcome! Please enter start node & end node (Format: start,end)\n", 65, 0);
    
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

        
        string userIn(buf);
        int Node = stoi(userIn);
        
        string result = dijkstra(graph, Node);
        int resultLength = result.length();
        
        char resultChar[resultLength + 1];
        
        strcpy(resultChar, result.c_str());
        send(clientSocket, resultChar, sizeof(resultChar), 0);
        
     
        
        cout << "Received: " << string(buf, 0, bytesRecv) << endl;
      
        
    }
    
    close(clientSocket);
            
    return 0;
}
