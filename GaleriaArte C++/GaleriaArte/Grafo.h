#pragma once
#ifndef GRAFO_H_
#define GRAFO_H_

#include <list>
#include <map>

using namespace std;

template <typename C> class Grafo
{
public:
	class Arco
	{
	public:
		Arco();
		Arco(int adyacente, const C & costo);
		int devolverAdyacente() const;
		const C & devolverCosto() const;
	private:
		int vertice;
		C costo;
	}; // class Arco

public:
	// NOTA: Dependiendo de la implementación puede ser necesario incluir otras funciones constructuras
	Grafo();
	Grafo(const Grafo & otroGrafo);

	~Grafo();

	Grafo & operator = (const Grafo & otroGrafo);

	// Devuelve true si la cantidad de vértices es cero
	bool estaVacio() const;
	// Indica la cantidad de vértices del grafo
	int devolverLongitud() const;

	bool existeVertice(int vertice) const;

	bool existeArco(int origen, int destino) const;

	// PRE CONDICION: existeArco(origen, destino)
	const C & costoArco(int origen, int destino) const;

	void devolverVertices(list<int> & vertices) const;

	void devolverAdyacentes(int origen, list<Arco> & adyacentes) const;

	void agregarVertice(int vertice);

	// POST CONDICION: Para todo vértice v != vertice: !existeArco(v, vertice) && !existeArco(vertice, v)
	void eliminarVertice(int vertice);

	// PRE CONDICION: existeArco(origen, destino)
	void modificarCostoArco(int origen, int destino, const C & costo);

	// PRE CONDICION: existeVertice(origen) && existeVertice(destino)
	// POST CONDICION: existeArco(origen, destino)
	void agregarArco(int origen, int destino, const C & costo);

	// POST CONDICION: !existeArco(origen, destino)
	void eliminarArco(int origen, int destino);

	void vaciar();

	void Grafo::DFS(int paramatroInutil);



private:
	/*
	* Definir la estructura interna
	*/
	map<int, map<int,C>> grafo;

	void Grafo<C>::DFSfun(int v, bool visitado[], int tiempo[], int t); //Este método debería ser privado
}; // class Grafo


   /*
   * Arco
   */

template <typename C> Grafo<C>::Arco::Arco()
{
}

template <typename C> Grafo<C>::Arco::Arco(int adyacente, const C & costo)
{
	this->vertice = adyacente;
	this->costo = costo;
}

template <typename C> int Grafo<C>::Arco::devolverAdyacente() const
{
	return vertice;
}

template <typename C> const C & Grafo<C>::Arco::devolverCosto() const
{
	return costo;
}


/*
* Grafo
*/

template <typename C> Grafo<C>::Grafo()
{
}

template <typename C> Grafo<C>::Grafo(const Grafo & otroGrafo)
{
	this->operator=(otroGrafo);
}

template <typename C> Grafo<C>::~Grafo()
{
}

template <typename C> Grafo<C> & Grafo<C>::operator = (const Grafo & otroGrafo)
{
	grafo.clear();
	typename map<int, map<int, C> >::const_iterator itOtro = otroGrafo.grafo.begin();
	while (itOtro != otroGrafo.grafo.end()) {
		grafo[itOtro->first] = itOtro->second;
		itOtro++;
	}

	return *this;
}

template <typename C> bool Grafo<C>::estaVacio() const
{
	return (grafo.empty());
}

template <typename C> int Grafo<C>::devolverLongitud() const
{
	return (grafo.size());
}

template <typename C> bool Grafo<C>::existeVertice(int vertice) const
{
	return (grafo.find(vertice) != grafo.end());
}

template <typename C> bool Grafo<C>::existeArco(int origen, int destino) const
{
	bool result = false;
	typename map<int, map<int, C> >::const_iterator itV = grafo.find(origen);
	if (itV != grafo.end()) {
		typename map<int, C>::const_iterator itA = itV->second.find(destino);
		if (itA != itV->second.end())
			result = true;
	}
	return result;
}

template <typename C> const C & Grafo<C>::costoArco(int origen, int destino) const
{
	typename map<int, map<int, C> >::const_iterator itV = grafo.find(origen);
	typename map<int, C>::const_iterator itA = itV->second.find(destino);
	return itA->second;
}

template <typename C> void Grafo<C>::devolverVertices(list<int> & vertices) const
{
	typename map<int, map<int, C> >::const_iterator itV = grafo.begin();
	while (itV != grafo.end()) {
		vertices.push_back(itV->first);
		itV++;
	}
}

template <typename C> void Grafo<C>::devolverAdyacentes(int origen, list<Arco> & adyacentes) const
{
	typename map<int, map<int, C> >::const_iterator itV = grafo.find(origen);
	if (itV != grafo.end()) {
		typename map<int, C>::const_iterator itA = itV->second.begin();
		while (itA != itV->second.end()) {
			adyacentes.push_back(Arco(itA->first, itA->second));
			itA++;
		}
	}
}

template <typename C> void Grafo<C>::agregarVertice(int vertice)
{
	if (grafo.find(vertice) == grafo.end()) {
		map<int, C> adyacentes;
		grafo[vertice] = adyacentes;
	}
}

template <typename C> void Grafo<C>::eliminarVertice(int vertice)
{
	grafo.erase(vertice);
	typename map<int, map<int, C> >::iterator itV = grafo.begin();
	while (itV != grafo.end()) {
		typename map<int, C>::iterator itA = itV->second.find(vertice);
		if (itA != itV->second.end()) {
			itV->second.erase(itA);
		}
		itV++;
	}
}

template <typename C> void Grafo<C>::modificarCostoArco(int origen, int destino, const C & costo)
{
	typename map<int, map<int, C> >::iterator itV = grafo.find(origen);
	if (itV != grafo.end()) {
		typename map<int, C>::iterator itA = itV->second.find(destino);
		if (itA != itV->second.end())
			itA->second = costo;
	}
}

template <typename C> void Grafo<C>::agregarArco(int origen, int destino, const C & costo)
{
	typename map<int, map<int, C> >::iterator itV = grafo.find(origen);
	if (itV != grafo.end()) {
		itV->second[destino] = costo;
	}
}

template <typename C> void Grafo<C>::eliminarArco(int origen, int destino)
{
	typename map<int, map<int, C> >::iterator itV = grafo.find(origen);
	if (itV != grafo.end()) {
		itV->second.erase(destino);
	}
}

template <typename C> void Grafo<C>::vaciar()
{
	grafo.clear();
}

template <typename C> void Grafo<C>::DFSfun(int v, bool visitado[], int tiempo[], int t)
{
	// Marcamos el vertice actual como visitado
	
	//Podemos mostrarlo
	//cout << v << " ";

	// Resolvemos recursivamente para todos los vertices adyacentes.
	// A este vértice
	visitado[v] = true;
	t = t + 1;
	tiempo[v] = t;
	//int verticeActual = this[v]->second();											//Acá estoy en el vértice actual.
	//typename map<int, C> ::iterator itA = verticeActual.begin();
	//typename map<int, C> ::iterator itA = grafo[v]->second.begin();
	list<Arco> l;
	this->devolverAdyacentes(v, l);
	//typename list<Arco> ::iterator itA = l.begin();
	list<typename Grafo<int>::Arco>::iterator itA = l.begin();
	int i;
	//for (; itA != verticeActual.end(); itA++)					//Acá me muevo en todos los adyacentes (arcos?)
		//i = itA->first();
	for (; itA != l.end(); itA++)
		i = itA->devolverAdyacente();
		if (!visitado[i])
		{
			DFSfun(i, visitado, tiempo, t);
			i++;
		} 
}

// DFS 
// Usa DFSUtil()
template <typename C> void Grafo<C>::DFS(int parametroInutil)
{
	// Marcamos los vértices como no visitados
	int t;
	t = 0;
	bool *visitado = new bool[grafo.size() ];
	int *tiempo = new int[grafo.size()];
	for (int i = 0; i < grafo.size(); i++)
	{
		visitado[i] = false;
		tiempo[i] = 0;
	}
	DFSfun(0, visitado, tiempo, t);
}



#endif /* GRAFO_H_ */
