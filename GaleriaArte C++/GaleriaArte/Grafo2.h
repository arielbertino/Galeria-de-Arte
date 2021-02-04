#pragma once

#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Graph {
	int V; //Numero de vertices
	list<int> *adj; //arreglo de listas de adyacencia
	bool *visited; //matriz de visitados.
public:
	Graph(int V); //Constructor
	void addEdge(int v, int w); //GRAFO Dirigido de v -> w
	void DFS(int v, vector<int> &V, int &i); //DFS iniciando de v
};
