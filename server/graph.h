

#ifndef GRAPH_H
#define GRAPH_H

struct AdjListNode;
struct AdjList;
struct Graph;
AdjListNode* newADjListNode(int dest, int weight);
Graph* createGraph(int V);
void addEdge(Graph* graph, int src, int dest, int weight);
struct MinHeapNode;
struct MinHeap;
MinHeapNode* newMinHeapNode(int v, int dist);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
void minHeapify(MinHeap* minHeap, int idx);
int isEmpty(MinHeap* minHeap);
MinHeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int v, int dist);
bool isInMinHeap(MinHeap* minHeap, int v);
std::string printArr(int dist[], int n, int src);
std::string dijkstra(Graph* graph, int src);


#endif /* GRAPH_H */

