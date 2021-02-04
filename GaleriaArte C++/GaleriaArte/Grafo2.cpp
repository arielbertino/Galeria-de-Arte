#include "Grafo2.h"

Graph::Graph(int V) {
	this->V = V;
	adj = new list<int>[V];
	this->visited = new bool[this->V];
	for (int i = 0; i<this->V; i++) this->visited[i] = false;
}
void Graph::addEdge(int v, int w) {
	adj[v].push_back(w); //agrega w a la posicion v
						 //adj[w].push_back(v); si fuera no dirigido
}
void Graph::DFS(int v, vector<int> &V, int &i) {
	this->visited[v] = true; //marca el nodo actual como visitado e imprimelo
	//cout << v << " ";
	V[i] = v;
	i++;

	list<int>::iterator it;
	for (it = adj[v].begin(); it != adj[v].end(); ++it) { //nodos adyacentes
		if (!this->visited[*it]) {
			DFS(*it, V ,i);
		}
	}
}