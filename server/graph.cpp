//
// Este codigo fue tomado de: https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
//

// C / C++ program for Dijkstra's shortest path algorithm for adjacency 
// list representation of graph 

#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <string.h>
#include <string>
#include "graph.h"

using namespace std;

struct AdjListNode 
{ 
	int dest; ///< ID del nodo destino para la lista de adyacencia
	int weight;  ///< Peso del nodo destino para la lista de adyacencia
	struct AdjListNode* next; ///< Puntero hacia el siguiente nodo en la lista de adyacencia
}; 

struct AdjList 
{ 
	struct AdjListNode *head; ///< Puntero hacia el inicio de la lista de adyacencia
}; 

// A structure to represent a graph. A graph is an array of adjacency lists. 
// Size of array will be V (number of vertices in graph) 
struct Graph 
{ 
	int V; ///< Cantidad de nodos en el grafo
	struct AdjList* array; ///< Puntero hacia la lista de adyancencia del grafo
}; 

/**
 * Este metodo agrega un nuevo nodo en la lista de adyacencia del grafo
 * @param dest Destino del nuevo nodo
 * @param weight Peso asociado con el nuevo nodo
 * @return El nodo
 */
struct AdjListNode* newAdjListNode(int dest, int weight) 
{ 
	struct AdjListNode* newNode = 
			(struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
	newNode->dest = dest; 
	newNode->weight = weight; 
	newNode->next = NULL; 
	return newNode; 
} 

/**
 * Permite crear un grafo del tamaño indicado
 * @param V Tamaño indicado
 * @return 
 */
struct Graph* createGraph(int V) 
{ 
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph)); 
	graph->V = V; 

	graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList)); 

	for (int i = 0; i < V; ++i) 
		graph->array[i].head = NULL; 

	return graph; 
} 

/**
 * Agrega una arista al grafo (no dirigido)
 * @param graph
 * @param src
 * @param dest
 * @param weight
 */
void addEdge(struct Graph* graph, int src, int dest, int weight) 
{ 
	struct AdjListNode* newNode = newAdjListNode(dest, weight); 
	newNode->next = graph->array[src].head; 
	graph->array[src].head = newNode; 

	newNode = newAdjListNode(src, weight); 
	newNode->next = graph->array[dest].head; 
	graph->array[dest].head = newNode; 
} 

struct MinHeapNode 
{ 
	int v; 
	int dist; 
}; 

struct MinHeap 
{ 
	int size;	 
	int capacity;  
	int *pos;	 
	struct MinHeapNode **array; 
}; 

/**
 * Funcion que permite crear un nodo en el heap que contiene los nodos a los que
 * aun no se les ha encontrado camino
 * @param v
 * @param dist
 * @return 
 */
struct MinHeapNode* newMinHeapNode(int v, int dist) 
{ 
	struct MinHeapNode* minHeapNode = 
		(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode)); 
	minHeapNode->v = v; 
	minHeapNode->dist = dist; 
	return minHeapNode; 
} 

/**
 * Permite crear un heap para almacenar nodos a los que aun no se les ha
 * encontrado camino
 * @param capacity
 * @return 
 */
struct MinHeap* createMinHeap(int capacity) 
{ 
	struct MinHeap* minHeap = 
		(struct MinHeap*) malloc(sizeof(struct MinHeap)); 
	minHeap->pos = (int *)malloc(capacity * sizeof(int)); 
	minHeap->size = 0; 
	minHeap->capacity = capacity; 
	minHeap->array = 
		(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*)); 
	return minHeap; 
} 

/**
 * Permite hacer un swap entre dos nodos del heap
 * @param a
 * @param b
 */
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) 
{ 
	struct MinHeapNode* t = *a; 
	*a = *b; 
	*b = t; 
} 
 
/**
 * Permite mantener ordenado el heap de nodos que aun no se ha encontrado
 * el camino mas corto
 * @param minHeap
 * @param idx
 */
void minHeapify(struct MinHeap* minHeap, int idx) 
{ 
	int smallest, left, right; 
	smallest = idx; 
	left = 2 * idx + 1; 
	right = 2 * idx + 2; 

	if (left < minHeap->size && 
		minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
	smallest = left; 

	if (right < minHeap->size && 
		minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
	smallest = right; 

	if (smallest != idx) 
	{ 
		// The nodes to be swapped in min heap 
		MinHeapNode *smallestNode = minHeap->array[smallest]; 
		MinHeapNode *idxNode = minHeap->array[idx]; 

		// Swap positions 
		minHeap->pos[smallestNode->v] = idx; 
		minHeap->pos[idxNode->v] = smallest; 

		// Swap nodes 
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 

		minHeapify(minHeap, smallest); 
	} 
} 

/**
 * Permite verificar si el heap de nodos esta vacio
 * @param minHeap
 * @return 
 */
int isEmpty(struct MinHeap* minHeap) 
{ 
	return minHeap->size == 0; 
} 

/**
 * Permite extraer el menor nodo del heap
 * @param minHeap
 * @return 
 */
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
	if (isEmpty(minHeap)) 
		return NULL; 

	// Store the root node 
	struct MinHeapNode* root = minHeap->array[0]; 

	// Replace root node with last node 
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
	minHeap->array[0] = lastNode; 

	// Update position of last node 
	minHeap->pos[root->v] = minHeap->size-1; 
	minHeap->pos[lastNode->v] = 0; 

	// Reduce heap size and heapify root 
	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return root; 
} 

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, int v, int dist) 
{ 
	// Get the index of v in heap array 
	int i = minHeap->pos[v]; 

	// Get the node and update its dist value 
	minHeap->array[i]->dist = dist; 

	// Travel up while the complete tree is not hepified. 
	// This is a O(Logn) loop 
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
	{ 
		// Swap this node with its parent 
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]); 

		// move to parent index 
		i = (i - 1) / 2; 
	} 
} 

/**
 * Permite verificar si el vertice v esta en el heap o no
 * @param minHeap
 * @param v
 * @return 
 */
bool isInMinHeap(struct MinHeap *minHeap, int v) 
{ 
if (minHeap->pos[v] < minHeap->size) 
	return true; 
return false; 
} 

/**
 * Retorna un string con los resultados del algoritmo de dijkstra a partir
 * del nodo indicado
 * @param dist
 * @param n
 * @param src
 * @return 
 */ 
string printArr(int dist[], int n, int src) 
{ 
        string info;
        info = "Vertex Distance from Source: " + std::to_string(src) + "\n";
	for (int i = 0; i < n; ++i){
            info += std::to_string(i) + "\t\t" + std::to_string(dist[i]) + "\n";
        }
        return info;
            
} 


/**
 * Permite aplicar el algoritmo de dijkstra al nodo indicado
 * @param graph
 * @param src
 * @return 
 */
string dijkstra(struct Graph* graph, int src) 
{ 
	int V = graph->V;// Get the number of vertices in graph 
	int dist[V];	 // dist values used to pick minimum weight edge in cut 

	// minHeap represents set E 
	struct MinHeap* minHeap = createMinHeap(V); 

	// Initialize min heap with all vertices. dist value of all vertices 
	for (int v = 0; v < V; ++v) 
	{ 
		dist[v] = INT_MAX; 
		minHeap->array[v] = newMinHeapNode(v, dist[v]); 
		minHeap->pos[v] = v; 
	} 

	// Make dist value of src vertex as 0 so that it is extracted first 
	minHeap->array[src] = newMinHeapNode(src, dist[src]); 
	minHeap->pos[src] = src; 
	dist[src] = 0; 
	decreaseKey(minHeap, src, dist[src]); 

	// Initially size of min heap is equal to V 
	minHeap->size = V; 

	// In the followin loop, min heap contains all nodes 
	// whose shortest distance is not yet finalized. 
	while (!isEmpty(minHeap)) 
	{ 
		// Extract the vertex with minimum distance value 
		struct MinHeapNode* minHeapNode = extractMin(minHeap); 
		int u = minHeapNode->v; // Store the extracted vertex number 

		// Traverse through all adjacent vertices of u (the extracted 
		// vertex) and update their distance values 
		struct AdjListNode* pCrawl = graph->array[u].head; 
		while (pCrawl != NULL) 
		{ 
			int v = pCrawl->dest; 

			// If shortest distance to v is not finalized yet, and distance to v 
			// through u is less than its previously calculated distance 
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && 
										pCrawl->weight + dist[u] < dist[v]) 
			{ 
				dist[v] = dist[u] + pCrawl->weight; 

				// update distance value in min heap also 
				decreaseKey(minHeap, v, dist[v]); 
			} 
			pCrawl = pCrawl->next; 
		} 
	} 

	// print the calculated shortest distances 
	string result = printArr(dist, V, src);
        return result;
        
} 