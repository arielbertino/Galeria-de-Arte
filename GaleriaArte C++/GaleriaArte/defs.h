//Definiciones iniciales.

#ifndef DEFS_H
#define DEFS_H 

#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <set>

using namespace std;

#define sqr(t)  (t)*(t)

const double    PI=3.141592653589793238462643383279502884197169399375105820974944592308;
enum  Type      { DESCONOCIDO, INPUT, INSERT, START, END, MERGE, SPLIT, REGULAR_UP, REGULAR_DOWN};

class   Punto;
class   Lado;

template <class T, class ValorClave>      class    Arbin;
typedef map<unsigned int, Punto*>				PuntoMap;
typedef map<unsigned int, Lado*>				LadoMap;
typedef priority_queue<Punto>					colaPuntos;
typedef Arbin<Lado*, double>				arBinLado;
typedef list<unsigned int>						PoliMonotono;
typedef list<PoliMonotono>						PoliMonotonos; 
typedef vector<unsigned int>					Triangulo;
typedef list<Triangulo>                         ListaTriangulo;
typedef map<unsigned int, set<unsigned int> >   MapaAdyacencias;
typedef float *									entrada;
#endif

