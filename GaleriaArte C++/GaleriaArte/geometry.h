#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "defs.h"
#include "arbin.h"


//Clase base Punto
class Punto
{
   public:
       //Constructores y destructores
       Punto() {}
       Punto(const Punto& pb);
       
       Punto(double xx, double yy)
                 :id(0), x(xx), y(yy), type(DESCONOCIDO) { }
       
       Punto(int idd, double xx, double yy)
	         :id(idd), x(xx), y(yy), type(DESCONOCIDO) { }
       
       Punto(double xx, double yy, Type ttype)
                 :id(0), x(xx), y(yy), type(ttype) { }
       
       Punto(int idd, double xx, double yy, Type ttype)
                 :id(idd),x(xx), y(yy), type(ttype) { }

      

       //Operadores
       friend  bool operator==(const Punto&, const Punto&); 
       friend  bool operator>(const Punto&, const Punto&);
       friend  bool operator<(const Punto&, const Punto&);
       friend  bool operator!=(const Punto&, const Punto&);
       friend  ostream &operator<<(ostream &os, const Punto& point);

       //Datos publicos (se pueden hacer privados y armar los getters y setters
       unsigned int    id;              //id del punto;
       double          x, y;            //coordenadas;
       Type            type;            //Tipo de punto (para la triangulación;
       bool            left;            //Si pertenece a la cadena izquierda o no (ver algoritmo);
	   unsigned int	   color=0;			//color del punto;
	   unsigned int	   guardia = 0;		//Si corresponde a un guardia, o no.

};


//Clase Lado segmento dirigido desde la cola hacia la cabeza.
class Lado
{
  public:
      //Constructores y destructor
      Lado();
      Lado(Punto* ep1, Punto* ep2, Type type);
      Lado(const Lado& line);
      ~Lado() {};

      unsigned int id() const { return _id; }
      
      //Extremos
      Punto*       extremoLado(int i) const { return _puntoExtremo[i]; }
      Type         tipo() const { return _type; }
      double       valorClave() const { return _clave; }
      void         setvalorClave(double y); 
      //Aumentamos el valor de la clave para evitar claves duplicadas en el árbol de búsqueda. 
      void         incrementarvalorClave(const double diff) { _clave+=diff; }
      //Cambiar la cola por la cabeza, y viceversa
      void         invertirSegmento(); 

      //Establecer y devolver un helper de un lado.
      void         setHelper(unsigned int i) { _helper=i; }
      unsigned int helper() { return _helper; }

      //operador             
      friend ostream &operator<<(ostream &os, const Lado& line);

  protected:
     unsigned int _id;					//id del lado;
     Punto*   _puntoExtremo[2];			//extremos del lado;
     
     Type         _type;         //Tipo de lado, input/insert (entrada o a insertar)
     double       _clave;          //clave de lado, se lo utiliza en el arbol de busqueda
     unsigned int _helper;       //helper de un lado
};



//Clase de archivo de contorno (con esto se crea el poligono original)
class BDMFile
{
public:

	BDMFile(string nombreArchivo, bool parse=true);
        ~BDMFile();
  
	PuntoMap&     puntos()    { return _puntos; }
	LadoMap&      lados()     { return _lados; }
 
private:
        bool   leerlinea(ifstream& nombreArchivo, string& result);
        int    contarCampos(const string& source, char delim); 
        string encontrarCampo(string& source);
        bool   existe(double, double); 
        void   leerBDM(const string& nombreArchivo);
		void   leerBDM(const string& nombreArchivo, bool parse);
		void   guardarBDM(float * in, int N);
 
protected:
    unsigned int            _ncontornos;   //numero de contornos
    vector<unsigned int>    _nVertices;   //
	PuntoMap				_puntos;      //todos vertices
	LadoMap                 _lados;       //todos edges 
	double                  _xmin,_xmax, _ymin,_ymax; //caja que contiene al poligono
	string                  _prefix;     //prefix of associated polygon bdm file; 
};

class Polygon: public BDMFile  
{
   public:
	//constructor y destructor   
	Polygon(string nombreArchivo, bool parse=true);
        ~Polygon();

		// Funciones principales para la triangulacion de poligonos.

	void         particion2Monotono();
	void         buscarMonotonos();
	void         triangulacion();
	bool vecinos(Triangulo t1, Triangulo t2);
	void		 colorear();

	int encontrarColorMenosRepetido();

	void colocarGuardias(int color);

	void colorearTriangulo(Triangulo &t);

	Punto centro(Triangulo t);

	//Devolver los triangulos
        ListaTriangulo    triangulos() { return _triangulos; }

	//Archivo de salida;
	void         saveAsShowme();
		
   //Funciones privadas
   private:
		void         inicializar();

        //prev or next point/edge id for a given ith point/edge; 	
	unsigned int anterior(unsigned int i);
	unsigned int siguiente(unsigned int i);

	//Manejar situación respecto al tipo de vértice
	void         manejarStartVertex(unsigned int);
	void         manejarEndVertex(unsigned int);
	void         manejarSplitVertex(unsigned int);
	void         manejarMergeVertex(unsigned int);
	void         manejarRegularVertexUp(unsigned int);
	void         manejarRegularVertexDown(unsigned int);
	
	//Agregar una diagonal entre dos vertices;
	void         agregarDiagonal(unsigned int i, unsigned int j);


	//Angulo para tres puntos dados, se lo utiliza en la búsqueda de polígonos monótonos.
	double       cosenoAngulo(double *A, double *B, double *C);
	//Encontrar el lado siguiente, se lo utiliza en la búsqueda de polígonos
	unsigned int seleccionarSiguienteLado(Lado* edge);
	
	//Triangular un poligono monotono
    void         triangularMonotono(PoliMonotono& mpoly);
		
	
        //Miembros privados
        colaPuntos			_colaPuntos;                          //Cola de prioridad para los vértices
        arBinLado			_arBinLados;                             //Árbol binario de búsqueda de lados
        PoliMonotonos		_mpolys;                              //Todos los polígonos monótonos
		ListaTriangulo		_triangulos;                          //Todos los triángulos.

		
		//Estructuras adicionales para descomponer los polígonos monótonos
        MapaAdyacencias		_startMapaAdyacencias;               //Todos los lados que comienzan de un punto dado
        LadoMap				_diagonales;                         //Diagonales agregadas para partir en polígonos monótono
	//						 
	//bool        _debug;                              //debug option;
	//fstream     _logfile;                            //log file for debug purpose;
};

#endif