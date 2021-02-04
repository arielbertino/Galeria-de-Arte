#include <stack>
#include "geometry.h"
#include "Grafo2.h"
#include <array>
#include <sstream>

long   int    l_id=0,p_id=0;
double orient2d(double* pa, double* pb, double* pc)
{
	double acx, bcx, acy, bcy;

	acx = pa[0] - pc[0];
	bcx = pb[0] - pc[0];
	acy = pa[1] - pc[1];
	bcy = pb[1] - pc[1];
	return acx * bcy - acy * bcx;
}

//----------------------------------------------------------------------------
//Cuadrado de la distancia entre dos puntos
//----------------------------------------------------------------------------
double dist_sqr(const Punto& sp, const Punto& ep)
{
	return sqr(sp.x-ep.x)+sqr(sp.y-ep.y);
}


double dist_sqr(double *pa, double *pb)
{
    	return sqr(pa[0]-pb[0])+sqr(pa[1]-pb[1]);
}

void UpdateKey(Nodo<Lado*,double>* node, double y)
{
     node->data()->setvalorClave(y);
}


//----------------------------------------------------------------------------
//Constructor por copia
//----------------------------------------------------------------------------
Punto::Punto(const Punto& pb)
{
    this->id=pb.id; 
	this->x=pb.x;
	this->y=pb.y;
	this->type=pb.type;
	this->left=pb.left;
	this->color = pb.color;
}

//----------------------------------------------------------------------------
//Operadores ( ==, >, < and != )
//----------------------------------------------------------------------------
bool operator==(const Punto& pa, const Punto& pb)
{
   	return (pa.x==pb.x && pa.y==pb.y);
}

//----------------------------------------------------------------------------
bool operator>(const Punto& pa, const Punto& pb)
{
   	return( (pa.y > pb.y) || ( (pa.y==pb.y) && (pa.x < pb.x)) );
}

//----------------------------------------------------------------------------
bool operator<(const Punto& pa, const Punto& pb)
{
	return( (pa.y < pb.y) || ( (pa.y==pb.y) && (pa.x > pb.x)) );
}

//----------------------------------------------------------------------------
bool operator!=(const Punto& pa, const Punto& pb)
{
   	return !(pa.x==pb.x && pa.y==pb.y);
}

//----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const  Punto& point)
{
    	os<<point.id<<" "<<setw(35)<<point.x<<setw(35)<<point.y<<'\n';
    	return os;
}


//----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Lado &line)
{
    	os<<"Lado:("<<line._id<<")"<<'\n';
	os<<*(line._puntoExtremo[0])<<*(line._puntoExtremo[1]);
	os<<" Type="<<line._type<<" key value:"<<line.valorClave()<<'\n';
    	return os;
}

//----------------------------------------------------------------------------
//Lado constructor
//----------------------------------------------------------------------------
Lado::Lado():_type(DESCONOCIDO)
{
    	for(int i=0; i<2; i++) _puntoExtremo[i]=0;
    	_id=0; 
}

//-----------------------------------------------------------------------------
//Lado constructor
//-----------------------------------------------------------------------------
Lado::Lado(Punto* sp, Punto* ep, Type type):_type(type)
{
    	_puntoExtremo[0]=sp;
    	_puntoExtremo[1]=ep;
    	_id=++l_id;
}

//----------------------------------------------------------------------------
//Constructor por copia
//----------------------------------------------------------------------------
Lado::Lado(const Lado& line)
{
    	this->_id=line._id;
    	this->_puntoExtremo[0]=line._puntoExtremo[0];
    	this->_puntoExtremo[1]=line._puntoExtremo[1];
    	this->_clave=line._clave;
    	this->_helper=line._helper;
}


//----------------------------------------------------------------------------
//Dar vuelta un lado
//----------------------------------------------------------------------------
void Lado::invertirSegmento() 
{ 
	assert(_type==INSERT); 
	Punto* tmp=_puntoExtremo[0]; 
	_puntoExtremo[0]=_puntoExtremo[1];
	_puntoExtremo[1]=tmp;
}

void Lado::setvalorClave(double y)
{
        if( _puntoExtremo[1]->y==_puntoExtremo[0]->y )  
		_clave=_puntoExtremo[0]->x < _puntoExtremo[1]->x ? _puntoExtremo[0]->x:_puntoExtremo[1]->x;
	else    _clave=( y - _puntoExtremo[0]->y ) * ( _puntoExtremo[1]->x - _puntoExtremo[0]->x ) / (_puntoExtremo[1]->y - _puntoExtremo[0]->y ) + _puntoExtremo[0]->x; 
}

BDMFile::BDMFile(string nombreArchivo, bool parse)
{ 
	
	if(parse) leerBDM(nombreArchivo);
	else leerBDM(nombreArchivo, false);
	_prefix=nombreArchivo;
        int pos=_prefix.find_last_of('.');
        if(pos!=-1) _prefix.erase(pos);
}
	



//----------------------------------------------------------------------------
//destructor of wrapped BDM class
////----------------------------------------------------------------------------
BDMFile::~BDMFile()
{
	
}

//----------------------------------------------------------------------------
//Leer una linea de un archivo
//----------------------------------------------------------------------------
bool BDMFile::leerlinea(ifstream& bdmfile, string& result)
{

bool endofile=false;

  	//Leer algo que sea un número
  	for(;;)
  	{
      		getline(bdmfile, result);
      		if( endofile=bdmfile.eof() ) { return endofile; }
      		if( contarCampos(result,0x20)==0 ) continue;
      		int i=0;
      		while(result[i]==0x20) i++;
      		if( result[i]>'9' || (result[i]<'0' && result[i]!='-') ) continue;
      		else  { return endofile=false; }
   	}
}

//----------------------------------------------------------------------------
//Cantidad de campos
//----------------------------------------------------------------------------
int  BDMFile::contarCampos(const string& source, char delim)
{
  int count=0;
  int size=source.size();
  
  for(int i=0; i<size; i++) 
    {
      if(source[i]==delim) continue;
      else {
             count++;
             do{
                  i++; 
                  if( i>=size ) break;
               }  
             while( source[i]!=delim );
           }
    }

  return count; 
 
}

//----------------------------------------------------------------------------
//Procesar linea
//----------------------------------------------------------------------------
string BDMFile::encontrarCampo(string& source)
{
  string denstr;
  int i=0;

  //Saltear espacios en blanco
  while ( (source[i] == '\0') || ( source[i] == ' ')
          || (source[i] == '\t')) i++;

  //Saltear el campo actual. Parar en un espacio en blanco
  while ((source[i] != '\0') && (source[i] != '#')
         && (source[i] != ' ') && (source[i] != '\t')) i++;

  //Saltamos el espacio en blanco y cualquier cosa que sea un número, comentario o fin de línea
  while ((source[i] != 0x20) && (source[i] != '#')
         && (source[i] != '.') && (source[i] != '+') && (source[i] != '-')
         && ((source[i] < '0') || (source[i] > '9'))) i++;
  
  //Fijarse si es un comentario
  if (source[i] == '#') source[i] = '\0';

  denstr=source.substr(i);
  return denstr;

}

//---------------------------------------------------------------------------
//Ver si existe duplicado
//---------------------------------------------------------------------------
bool BDMFile::existe(double x, double y)
{
   bool result=false;

   PuntoMap::iterator it=_puntos.begin();
   for(; it!=_puntos.end(); it++)
    {
      if( it->second->x==x && it->second->y==y )  { result=true; break; }
    }

   return result;
}

//----------------------------------------------------------------------------
//Leer archivo
//----------------------------------------------------------------------------
void BDMFile::leerBDM(const string& nombreArchivo)
{

 ifstream bdmfile(nombreArchivo.c_str(), ios::in);
 
 if(!bdmfile) { cout<<"No existe el archivo!\n"; exit(1); }

 unsigned int i = 1, first, last;
 double x,y;
 Type type;

 _ncontornos=0; 


 for(;;) 
    {
      string result="\0";
      if( leerlinea(bdmfile,result) )  break;
      _nVertices.push_back( atoi(result.c_str()) );

      if(_nVertices[_ncontornos]==0) break; 
      first = i;
      last = first + _nVertices[_ncontornos] - 1;
      for (unsigned int j = 0; j < _nVertices[_ncontornos]; j++, i++)
        {
           if( leerlinea(bdmfile, result) && j!=_nVertices[_ncontornos] )
              {
                cout<<_nVertices[_ncontornos]-j<<"Faltan vertices en el contorno";
                cout<<_ncontornos+1<<". Programa falla."<<'\n';
                bdmfile.close();
                exit(1);
               }
         
           x = (double) atof( result.c_str() );
           int nfields=contarCampos(result,0x20);
           if( nfields<2 )
              {
                 cout<<"No hay datos suficientes para el vértice:"<<j+1<<"("<<x<<", ?)";
                 cout<<" en el contorno:"<<_ncontornos+1<<"\nEl programa se detiene."<<'\n';
                 bdmfile.close();
                 exit(1);
              }

           string denstr=encontrarCampo(result);
           y = (double) atof( denstr.c_str() );
           type=INPUT;
 
           if( existe(x, y) )
              {
                 cout<<"Error! Punto duplicado. El punto es:";
                 cout<<i<<"("<<x<<","<<y<<").\n";
                 exit(1);
              }
           else
              {
                 Punto* point=new Punto(i,x,y,type);
		 if(x > _xmax ) _xmax=x; 
		 if(x < _xmin ) _xmin=x;
		 if(y > _ymax ) _ymax=y;
		 if(y < _ymin ) _ymin=y;
		 _puntos[i]=point;
              }
         }

         _ncontornos++;
     } 
   
   int sid,eid;
   int num=0;

   for(unsigned int j=0; j<_ncontornos; j++)
    {
     for(i=1; i<=_nVertices[j]; i++)
       {
          sid=num+i;
          eid=(i==_nVertices[j])?num+1:num+i+1;
          type = INPUT;
          Lado* line=new Lado(_puntos[sid], _puntos[eid], type);
          _lados[l_id]=line;
        }
      num+=_nVertices[j];
    }

   int sum=0;
   for(unsigned int i=0; i<_ncontornos; i++)
   {
	   sum+= _nVertices[i];
           _nVertices[i]=sum;
   }
	   
   p_id=num;
   bdmfile.close();
}
//----------------------------------------------------------------------------
//Crear el archivo que viene de afuera
//----------------------------------------------------------------------------
void BDMFile::guardarBDM(float * in, int N) {
	
	ofstream bdmfile("entrada.bdm");
	std::ostringstream b;
	std::string bb;
	b << in[0];
	bdmfile << b.str();
	bdmfile << "\n";
	for (int i = 1; i < N; i++) {
		b << in[(2 * i) - 1];
		bdmfile << b.str();
		bdmfile << " ";
		b << (in[2 * i]);
		bdmfile << b.str();
		bdmfile << "\n";
	}
}

//----------------------------------------------------------------------------
//Leer archivo sin procesar la entrada
//----------------------------------------------------------------------------
void BDMFile::leerBDM(const string& nombreArchivo, bool parse)
{

 ifstream bdmfile(nombreArchivo.c_str(), ios::in);
 if(!bdmfile) { cout<<"No existe el archivo!\n"; exit(1); }
 
 unsigned int i = 1, first, last, num;
 double x,y;
 Type type;

 _ncontornos=0; 

 while(!bdmfile.eof()) 
    {
      num=0;	     
      bdmfile>>num;
      if( !bdmfile.eof() && num==0)
      {
	      cout<<"Archivo incorrecto. No usar la opcion -p si el archivo tiene comentarios.\n";
              cout<<"Error.\n"; exit(1); 
      }
      
      if( bdmfile.eof() ) break;
      _nVertices.push_back( num );

      first = i;
      last = first + _nVertices[_ncontornos] - 1;
      for (unsigned int j = 0; j < _nVertices[_ncontornos]; j++, i++)
        {
           bdmfile>>x>>y;
	   if( bdmfile.eof() ) { cout<<"EoF inesperado! Saliendo\n"; exit(1); }
           type=INPUT;
 
           Punto* point=new Punto(i,x,y,type);
	   if(x > _xmax ) _xmax=x; 
	   if(x < _xmin ) _xmin=x;
	   if(y > _ymax ) _ymax=y;
	   if(y < _ymin ) _ymin=y;
	   //point->rotate(PI/2.0);
           _puntos[i]=point;
         }
         _ncontornos++;
     } 
   
   int sid,eid;
   num=0;

   for(unsigned int j=0; j<_ncontornos; j++)
    {
     for(i=1; i<=_nVertices[j]; i++)
       {
          sid=num+i;
          eid=(i==_nVertices[j])?num+1:num+i+1;
          type = INPUT;
          Lado* line=new Lado(_puntos[sid], _puntos[eid], type);
          _lados[l_id]=line;
        }
      num+=_nVertices[j];
    }

   int sum=0;
   for(unsigned int i=0; i<_ncontornos; i++)
   {
	   sum+= _nVertices[i];
           _nVertices[i]=sum;
   }
	   
   p_id=num;
   bdmfile.close();
}




//----------------------------------------------------------------------------
//Constructor de poligono
//----------------------------------------------------------------------------
Polygon::Polygon(string nombreArchivo, bool parse):BDMFile(nombreArchivo,parse)
{
   inicializar();
  
}

//----------------------------------------------------------------------------
//Destructor de poligono
//----------------------------------------------------------------------------
Polygon::~Polygon()
{
   /*if(_debug && _logfile.is_open() ) 
   {
           string logfile=_prefix+".log";
	   cout<<"Debug info. "<<logfile<<" was saved\n";
	   _logfile.close();
   }*/
   //Borrar toda la memoria utilizada
   PuntoMap::iterator itp=_puntos.begin();
   for(; itp!=_puntos.end(); itp++)
   {
	   delete itp->second;
   }

   LadoMap::iterator itl=_lados.begin();
   for(; itl!=_lados.end(); itl++)
   {
          delete itl->second;
   }
      
}

//----------------------------------------------------------------------------
//Devolver el id de un punto anterior, o un lado anterior dado un punto o lado.
//----------------------------------------------------------------------------
unsigned int Polygon::anterior(unsigned int i)
{
   unsigned int j(0),prevLoop(0),currentLoop(0);
  
   while ( i > _nVertices[currentLoop] ) 
   {
     prevLoop=currentLoop;
     currentLoop++;     
   }
 
   if( i==1 || (i==_nVertices[prevLoop]+1) ) j=_nVertices[currentLoop];
   else if( i <= _nVertices[currentLoop] ) j=i-1;

   return j;
}

//----------------------------------------------------------------------------
//Devolver el id de un punto siguiente, o un lado siguiente dado un punto o lado.
//----------------------------------------------------------------------------
unsigned int Polygon::siguiente(unsigned int i)
{
   unsigned int j(0),prevLoop(0),currentLoop(0);
  
   while ( i > _nVertices[currentLoop] ) 
   {
     prevLoop=currentLoop;	   
     currentLoop++;  
   }
 
   if( i < _nVertices[currentLoop] ) j=i+1;
   else if ( i==_nVertices[currentLoop] ) 
   {
	   if( currentLoop==0) j=1;
	   else j=_nVertices[prevLoop]+1;
   }
   
   return j;
}


//----------------------------------------------------------------------------
//Inicialización del poligono
//Para encontrar los tipos de todos los vértices del polígono
//creamos una cola de prioridad para todos los vértices
//construimos un set de lados para cada vértices 
//(el conjunto contiene todos los lados que comienzan con el determinado vértice, se lo utiliza en las búsquedas). 
//----------------------------------------------------------------------------
void Polygon::inicializar()
{
     PuntoMap::iterator it=_puntos.begin();
     for(; it!=_puntos.end(); it++)
	{
	    int id=it->first; 
	    int idp=anterior(id);
	    int idn=siguiente(id);
	    Punto p=*_puntos[id], pnext=*_puntos[idn], pprev=*_puntos[idp];
	   
	    if( p > pnext && pprev > p )
		   _puntos[id]->type=REGULAR_DOWN;
	    else if (p > pprev && pnext > p)
		   _puntos[id]->type=REGULAR_UP;
	    else
	    {
               double pa[2], pb[2], pc[2];
	       
               pa[0]=_puntos[idp]->x;
	       pa[1]=_puntos[idp]->y;

	       pb[0]=_puntos[id]->x;
	       pb[1]=_puntos[id]->y;

	       pc[0]=_puntos[idn]->x;
	       pc[1]=_puntos[idn]->y;
 
	       double area=orient2d(pa,pb,pc);

	       if( pprev > p && pnext > p ) _puntos[id]->type=(area >0) ? END: MERGE ;
	       if( pprev < p && pnext < p ) _puntos[id]->type=(area >0) ? START : SPLIT;
	       
	     }
	    
	    _colaPuntos.push(*(it->second));

	    _startMapaAdyacencias[id].insert(id);//no se usa
	    
	}
}

//----------------------------------------------------------------------------
//Agregar una diagonal del punto i al punto j
//----------------------------------------------------------------------------  
void Polygon::agregarDiagonal(unsigned int i, unsigned int j)
{
    Type type=INSERT;
    Lado* diag=new Lado(_puntos[i], _puntos[j], type);
    _lados[diag->id()]=diag;
	
    _startMapaAdyacencias[i].insert(diag->id());
    _startMapaAdyacencias[j].insert(diag->id());

    _diagonales[diag->id()]=diag;     
}

//----------------------------------------------------------------------------
//manejar start vertex
//----------------------------------------------------------------------------
void Polygon::manejarStartVertex(unsigned int i)
{
     double y=_puntos[i]->y;	
     _arBinLados.InOrder(UpdateKey, y);
     
     _lados[i]->setHelper(i);
     _lados[i]->setvalorClave(y);      
     _arBinLados.Insert(_lados[i]);

    
}

//----------------------------------------------------------------------------
//manejar end vertex
//----------------------------------------------------------------------------
void Polygon::manejarEndVertex(unsigned int i)
{
     double y=_puntos[i]->y;
     _arBinLados.InOrder(UpdateKey, y);
     
     unsigned int previ=anterior(i);
     Lado* edge=_lados[previ];
     unsigned int helper=_lados[previ]->helper();
    
     
     if(_puntos[helper]->type==MERGE) agregarDiagonal(i, helper);
     _arBinLados.Delete(edge->valorClave());

  
}

//----------------------------------------------------------------------------
//manejar split vertex
//----------------------------------------------------------------------------
void Polygon::manejarSplitVertex(unsigned int i)
{
     double x=_puntos[i]->x, y=_puntos[i]->y;
     _arBinLados.InOrder(UpdateKey, y);
     
     Nodo<Lado*, double>*  leftnode;
     _arBinLados.FindMaxSmallerThan(x, leftnode);
     Lado* leftedge=leftnode->data();
     
     unsigned int helper=leftedge->helper();
     agregarDiagonal(i, helper);

   
    
     leftedge->setHelper(i);
     _lados[i]->setHelper(i);
     _lados[i]->setvalorClave(y);
     _arBinLados.Insert(_lados[i]);  
}


//----------------------------------------------------------------------------
//manejar merge vertex
//----------------------------------------------------------------------------
void Polygon::manejarMergeVertex(unsigned int i)
{
     double x=_puntos[i]->x, y=_puntos[i]->y;
     _arBinLados.InOrder(UpdateKey, y);
    
     unsigned int previ=anterior(i);
     unsigned int helper=_lados[previ]->helper();
     if (_puntos[helper]->type==MERGE) agregarDiagonal(i, helper);
     _arBinLados.Delete(_lados[previ]->valorClave());
   

     Nodo<Lado*, double>*  leftnode;
     _arBinLados.FindMaxSmallerThan(x, leftnode);
     Lado* leftedge=leftnode->data();
            
     helper=leftedge->helper();
     if(_puntos[helper]->type==MERGE) agregarDiagonal(i, helper);
     
     leftedge->setHelper(i);

}

//----------------------------------------------------------------------------
//manejar regular down vertex
//----------------------------------------------------------------------------
void Polygon::manejarRegularVertexDown(unsigned int i)
{
     double y=_puntos[i]->y;
     _arBinLados.InOrder(UpdateKey, y);
     
     unsigned int previ=anterior(i);
     unsigned int helper=_lados[previ]->helper();
     if(_puntos[helper]->type==MERGE) agregarDiagonal(i, helper); 
	  
     _arBinLados.Delete(_lados[previ]->valorClave());
     _lados[i]->setHelper(i);
     _lados[i]->setvalorClave(y);
     _arBinLados.Insert(_lados[i]);

}


//----------------------------------------------------------------------------
////manejar regular up vertex
//----------------------------------------------------------------------------
void Polygon::manejarRegularVertexUp(unsigned int i)
{	
     double x=_puntos[i]->x, y=_puntos[i]->y;
     _arBinLados.InOrder(UpdateKey, y);
     
     Nodo<Lado*, double>*  leftnode;
     _arBinLados.FindMaxSmallerThan(x, leftnode);
     
     Lado* leftedge=leftnode->data();
		     
     unsigned int helper=leftedge->helper();
     if(_puntos[helper]->type==MERGE) agregarDiagonal(i, helper);
     leftedge->setHelper(i);


}

//----------------------------------------------------------------------------
//Partimos el polígono en partes monótonas
//----------------------------------------------------------------------------
void Polygon::particion2Monotono()
{
     if(_colaPuntos.top().type!=START) 
     { 
		cout<<"El punto inicial debe ser de tipo START, verificar orientaciones y puntos duplicados\n";
	        cout<<"El programa se detuvo.\n";
	        exit(2); 
     }
		
     while(!_colaPuntos.empty())
     {
          Punto vertex=_colaPuntos.top();
          _colaPuntos.pop();
	  unsigned int id=vertex.id;
	  


    	  switch(vertex.type)
	  {
      		case START:        manejarStartVertex(id);       break;
      		case END:          manejarEndVertex(id);         break;
      		case MERGE:        manejarMergeVertex(id);       break;
      		case SPLIT:        manejarSplitVertex(id);       break;
      		case REGULAR_UP:   manejarRegularVertexUp(id);   break;
		case REGULAR_DOWN: manejarRegularVertexDown(id); break;
      		default: 
				   cout<<"Hay puntos duplicados. Galeria se detuvo.\n";
				   exit(1); break;                         
  	   }
     }
}


//----------------------------------------------------------------------------
//Funciones auxiliares para encontrar porciones de polígonos monótonos
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//Calcular el ángulo a partir de tres puntos A,B,C el origen de los dos puntos es A y los extremos son B y C, forman dos segmentos a los 
//que puede calcularse el ángulo.
//----------------------------------------------------------------------------
double Polygon::cosenoAngulo(double *pa, double *pb, double *pc)
{
  double dxab = pa[0] - pb[0];
  double dyab = pa[1] - pb[1];

  double dxcb = pc[0] - pb[0];
  double dycb = pc[1] - pb[1];

  double dxab2 = dxab * dxab;
  double dyab2 = dyab * dyab;
  double dxcb2 = dxcb * dxcb;
  double dycb2 = dycb * dycb;
  double ab = dxab2 + dyab2;
  double cb = dxcb2 + dycb2;

  double cosb = dxab * dxcb + dyab * dycb;
  double denom = sqrt( ab * cb);
  
  cosb/=denom;
  
  return cosb;
}

//----------------------------------------------------------------------------
//Para cada lado, encontrar el lado siguiente que deberíamos elegir cuando buscamos una porción monótona.
//----------------------------------------------------------------------------
unsigned int Polygon::seleccionarSiguienteLado(Lado* edge)
{

    unsigned int eid= edge->extremoLado(1)->id;
    set<unsigned int> edges=_startMapaAdyacencias[eid];
    assert(!edges.empty());
     
    unsigned int nexte=0;
    if( edges.size() == 1 )  nexte=*(edges.begin());
    else if( edges.size() > 1 )
    {
	unsigned int nexte_ccw(0), nexte_cw(0);
	double max=-2.0,min=2.0;
	
    	
	set<unsigned int>::iterator it=edges.begin();
	for(; it!=edges.end(); it++)
	{
		if(*it==edge->id()) continue;     
		double A[2], B[2], C[2];
		A[0]=edge->extremoLado(0)->x;        A[1]=edge->extremoLado(0)->y;
		B[0]=edge->extremoLado(1)->x;        B[1]=edge->extremoLado(1)->y;
		
		if(edge->extremoLado(1)!=_lados[*it]->extremoLado(0)) _lados[*it]->invertirSegmento();
		C[0]=_lados[*it]->extremoLado(1)->x; C[1]=_lados[*it]->extremoLado(1)->y;
		
		double area=orient2d(A, B, C);
		double cosb=cosenoAngulo(A, B, C);

		if( area > 0 && max < cosb ) { nexte_ccw=*it; max=cosb; }
		else if( min > cosb ) { nexte_cw=*it; min=cosb; }
	}

	nexte = (nexte_ccw!=0) ? nexte_ccw : nexte_cw;
    } 

   return nexte; 
}

//----------------------------------------------------------------------------
//Buscamos todas las porciones monótonas
//----------------------------------------------------------------------------
void Polygon::buscarMonotonos()
{
    int loop=0;
    
    LadoMap edges=_lados;
    
    while( edges.size() > _diagonales.size() )
    {
	    loop++;
	    PoliMonotono poly;
	    LadoMap::iterator it=edges.begin();
	    Punto* startp=startp=it->second->extremoLado(0);
	    Punto* endp=0;
	    Lado*  next=it->second;
	    
	    poly.push_back(startp->id);

	    for(;;) 
	    {
		    endp=next->extremoLado(1);
		    if(next->tipo()!=INSERT) 
		    	{ 
				edges.erase(next->id());
				_startMapaAdyacencias[next->extremoLado(0)->id].erase(next->id());
			}
		    if(endp==startp) break; 
		    poly.push_back(endp->id);
		    
		 
		    
		    unsigned int nexte=seleccionarSiguienteLado(next);
		    
		    if(nexte==0) 
		    {
			 
			    exit(1);
		    }
		    //assert( nexte > 0);
		    next=edges[nexte];
		    if(next->extremoLado(0) !=endp ) next->invertirSegmento(); 
	    }
	   
	  

	    _mpolys.push_back(poly);
    }
}


//----------------------------------------------------------------------------
//Triangulamos un polígono monótono
//----------------------------------------------------------------------------
void  Polygon::triangularMonotono(PoliMonotono& mpoly)
{
       	
      colaPuntos qvertex; 	
      PoliMonotono::iterator it=mpoly.begin(), itnext;
      for(; itnext=it, it!=mpoly.end(); it++) 
      {
	      itnext++;  
	      if(itnext==mpoly.end()) itnext=mpoly.begin();
	      Punto point=*_puntos[*it], pointnext=*_puntos[*itnext];
	      point.left=(point > pointnext)? true:false;
	      qvertex.push(point);
      }
     
     stack<Punto> spoint;
     for(int i=0; i<2; i++) { spoint.push(qvertex.top()); qvertex.pop(); }
     
     while ( qvertex.size() > 1 )
     {
	Punto topQueuePoint=qvertex.top();
	Punto topStackPoint=spoint.top();
		
        if(topQueuePoint.left!=topStackPoint.left)
	{
            while ( spoint.size()  > 1 )
	    {
		  Punto p1=spoint.top();
		  spoint.pop(); 
		  Punto p2=spoint.top();
		  Triangulo v(3);
		  v[0]=topQueuePoint.id;
		  v[1]=p1.id;
		  v[2]=p2.id;
		  _triangulos.push_back(v);
		  
	    }
	    spoint.pop();
            spoint.push(topStackPoint);
            spoint.push(topQueuePoint); 	    
	}
	else
	{
	    while( spoint.size() > 1 )
	    {	    
            	Punto stack1Point=spoint.top();
	    	spoint.pop(); 
            	Punto stack2Point=spoint.top();
	    	spoint.push(stack1Point);
	    	double pa[2], pb[2], pc[2];
	    	pa[0]=topQueuePoint.x; pa[1]=topQueuePoint.y;
	    	pb[0]=stack2Point.x;   pb[1]=stack2Point.y;
	    	pc[0]=stack1Point.x;   pc[1]=stack1Point.y;
		
            	double area=orient2d(pa,pb,pc);
	    	bool   left=stack1Point.left;
            	if( (area > 0 && left) || (area < 0 && !left ) ) 
	    	{
                        Triangulo v(3);
                        v[0]=topQueuePoint.id;
                        v[1]=stack2Point.id;
                        v[2]=stack1Point.id;
                        _triangulos.push_back(v);
	                  	spoint.pop();
	    	} else break;
	    }
	    
	    spoint.push(topQueuePoint); 
	    
	}
	
	qvertex.pop();
	
     }
     
     Punto lastQueuePoint=qvertex.top();
     while( spoint.size() !=1 )
     {
	     Punto topPoint=spoint.top();
	     spoint.pop();
	     Punto top2Point=spoint.top();
	      
             Triangulo v(3);
             v[0]=lastQueuePoint.id;
             v[1]=topPoint.id;
             v[2]=top2Point.id;
             _triangulos.push_back(v);
	     
     }
}

//----------------------------------------------------------------------------
//Función principal de triangulación
////----------------------------------------------------------------------------
void Polygon::triangulacion()
{			  
     particion2Monotono();
     buscarMonotonos();
     PoliMonotonos::iterator it=_mpolys.begin();
     for(; it!=_mpolys.end(); it++)
	triangularMonotono(*it);

     cout<<"Numero de triangulos:"<<_triangulos.size()<<'\n';
		
}



//-----------------------------------------------------------------------------
//Guardamos los resultados .ele para los elementos y .node para los puntos
//los podemos ver en MATLAB
//-----------------------------------------------------------------------------
void Polygon::saveAsShowme()
{
    string ele=_prefix+".ele";
    string node=_prefix+".node";
	string color = _prefix + ".color";
	string guardias = _prefix + ".guar";

    fstream fele(ele.c_str(), ios::out);
    fstream fnode(node.c_str(), ios::out);
	fstream fcolor(color.c_str(), ios::out);
	fstream fguar(guardias.c_str(), ios::out);

    fnode.precision(20);
    fnode.setf(ios::fixed, ios::floatfield);

   
    fnode<<(int)_puntos.size()<<'\n';
    fele<<(int)_triangulos.size()<<'\n';

    PuntoMap::iterator it=_puntos.begin();
    for(;it!=_puntos.end(); it++)
    {
	Punto tmp=*(it->second);
	fnode<<tmp;
	int colorcito = it->second->color;
	int guardita = it->second->guardia;
	fcolor << colorcito<<'\n';
	fguar << guardita<< '\n';
    }

    long int num=0; 
    ListaTriangulo::iterator itt=_triangulos.begin();
    for(; itt!=_triangulos.end(); itt++)
          fele<<++num<<" "<<(*itt)[0]<<"  "<<(*itt)[1]<<"  "<<(*itt)[2]<<'\n';
    
    fnode.close();
    fele.close();
	fcolor.close();
	fguar.close();

    cout<<node<<" guarda los nodos.\n";
    cout<<ele<<" guarda los triangulos.\n";
	cout << color << " guarda los colores. \n";
	cout << guardias << " guarda los guardias. \n" ;
}




Punto Polygon::centro(Triangulo t)
{
	//Esta función calcula el centro del triangulo, este va a ser el representante del grafo.

	Punto* p0 = _puntos[t[0]];
	Punto* p1 = _puntos[t[1]];
	Punto* p2 = _puntos[t[2]];

	return Punto((p0->x + p1->x + p2->x) / 3, (p0->y + p1->y + p2->y) / 3);
}

struct NodoGrafo {
	//El grafo siempre va a tener, a lo sumo,3 hijos y no más. Este es el llamado grafo dual
	Triangulo t;
	Punto c;
	int vecino1;
	int vecino2;
	int vecino3;
};

bool Polygon::vecinos(Triangulo t1, Triangulo t2) {
	//No hace falta mirar los puntos, con mirar los índices (que son únicos) basta.
	bool c1 = false; 
	bool c2 = false;
	int i;
	int j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if ((t1[i] == t2[j]) & (c1 == false)) {
				c1 = true;
				break; //Si encontramos uno, pasamos al siguiente.
			}
			else
			{
				//cout << t1[i];
				//cout << " ";
				//cout << t2[j] << endl;
				if (t1[i] == t2[j]) {
					c2 = true;
					//cout << "Encontre un vecino" << endl;
					break;
				}
			}
			}
		}
	//cout << endl;
	return c2;
}

void Polygon::colorear()
{
	
	ListaTriangulo t = this->triangulos();
	ListaTriangulo::iterator it;
	int n;
	int cantTriangulos = t.size();
	NodoGrafo * arreglo;
	NodoGrafo aux;

	//El arreglo contiene la información, el grafo contiene la estructura donde nos vamos a mover.
	arreglo = new NodoGrafo[cantTriangulos];
	int i = 0;
	for (it = t.begin(); it != t.end(); it++) 
		{
			aux.t = *(it);
			aux.c = centro(aux.t);
			aux.vecino1 = -1; //-1: no tiene vecino.
			aux.vecino2 = -1;
			aux.vecino3 = -1;
			arreglo[i] = aux;
			i++;
		}
	n = i;
	//Una vez que tenemos todos los elementos, los agregamos al grafo (que es de int, lleva referencia al arreglo)
	Graph G(n); //Graph es de tipo int y se construye con la variable que contaba la cantidad de vertices.

	//Una vez que colocamos los triangulos en el arreglo, buscamos los vecinos correspondientes.
	for (i=0;i<n;i++)
	{
		for (int j = 0; j < n; j++) {
			if (j != i)
			{
				if (vecinos(arreglo[i].t,arreglo[j].t))
				{
					if (arreglo[i].vecino1 == -1)
						arreglo[i].vecino1 = j;
					else {
						if (arreglo[i].vecino2 == -1) {
							arreglo[i].vecino2 = j;
						}
						else {
							arreglo[i].vecino3 = j;
						}
					}
				}

			}
		}
		//cout << "Triangulo " << arreglo[i].t[0] << " " << arreglo[i].t[1] << " " << arreglo[i].t[2] << endl;
		//cout << "Vertice " << i << ": " << "Vecino 1: " << arreglo[i].vecino1 << " Vecino 2: " << arreglo[i].vecino2 << " Vecino 3: " << arreglo[i].vecino3 << endl;
		//cout << endl;
	}

	//Hay que construir un grafo que tenga los triangulos y sus centros de masa, a partir de ahí se puede crear la relación
	//a partir de que los triangulos comparten lados, una vez que se tiene el grafo, se puede aplicar el dfs
	//para colorear en costo O(n), con n el número de triangulos.
	
	//Creamos los lados del grafo
	for (i = 0; i < n; i++)
	{
		if (arreglo[i].vecino1 != -1)
		{
			G.addEdge(i, arreglo[i].vecino1); //Ponemos costo 1 para todos los nodos, así la búsqueda es netamente en profundidad.
		}
		if (arreglo[i].vecino2 != -1)
		{
			G.addEdge(i, arreglo[i].vecino2);
		}
		if (arreglo[i].vecino3 != -1) {
			G.addEdge(i, arreglo[i].vecino3);
		}
	}

	//Y ahora está listo para ejectuar el DFS
	vector<int> V(n,0);
	int inicio = 0;
	G.DFS(0,V,inicio); //DFS desde el nodo raíz
	cout << endl;
	//Coloreamos todos los triángulos.
	for (i = 0; i < n; i++) {
		colorearTriangulo(arreglo[V[i]].t);
		//for (int qj = 0; qj < 3; qj++) {
		//	cout << _puntos[arreglo[V[i]].t[qj]]->color << " ";
		//}
		//cout << endl;
	}
	//Buscamos el color que menos se repita
	int colorGuardia = encontrarColorMenosRepetido();
	//cout << colorGuardia << endl;
	//Colocamos los guardias en el color que menos se repite
	colocarGuardias(colorGuardia);

}

int Polygon::encontrarColorMenosRepetido()
{
	PuntoMap::iterator it;
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	int min1 = 0;
	int colormin = 0;
	for (it = _puntos.begin(); it != _puntos.end(); it++) {
		if (it->second->color == 1) {
			c1++;
		}
		else {
			if (it->second->color == 2)
				c2++;
			else {
				c3++;
			}
		}
	}
	if ((c1 < c2) || (c1 == c2)) {
		min1 = c1; //1
		colormin = 1;
	}
	else {
		min1 = c2; // 1 y 2 tienen la misma cantidad
		colormin = 2;
	}
	if (min1 < c3) {
		return colormin;
	}
	else {
		return 3;
	}
}

void Polygon::colocarGuardias(int color) {
	PuntoMap::iterator it;
	for (it = _puntos.begin(); it != _puntos.end(); it++) {
		if (it->second->color == color) {
			it->second->guardia = 1;
			//cout << it->second->id << " ";
		}
	}
}

void Polygon::colorearTriangulo(Triangulo &t) {
	//Observación, este algorito funciona únicamente para polígonos simplemente conexos (sin agujeros), si no, falla.
	if ((_puntos[t[0]]->color == 0) & (_puntos[t[1]]->color == 0) & (_puntos[t[2]]->color == 0)) {		//Si no está pintado ningún vértice del triángulo
		//Los pintamos con cualquier color
		_puntos[t[0]]->color = 1;
		_puntos[t[1]]->color = 2;
		_puntos[t[2]]->color = 3;
	}
	else {	//Si alguno está pintado, miramos cuáles están pintados (deberían ser siempre dos, y pintamos el restante con el que falta)
		int i, j, k;
		if (_puntos[t[0]]->color == 0) { //Si no está pintado 0, i y j representan los que ya están pintados, k es el que hay que pintar.
			i = 1;
			j = 2;
			k = 0;
		}
		else {
			if (_puntos[t[1]]->color == 0) { //Si no está pintado 1..
				i = 0;
				j = 2;
				k = 1;
			}
			else {	//Si no está pintado 2...
				if (_puntos[t[2]]->color == 0) {
					i = 0;
					j = 1;
					k = 2;
				}
				else
				{
					//Si están todos pintados, ya estuvimos en este triángulo, salimos.
					i = -1;
					j = -1;
					k = -1;
				}
			}
		}
		if (i>-1){
		int aux = _puntos[t[i]]->color + _puntos[t[j]]->color;	//Sumamos los colores.

		switch (aux) {
		case 3:	//Si la suma de los colores da 3 es porque los vecinos valen 1 y 2.
			_puntos[t[k]]->color = 3;
			break;
		case 4: //Si la suma es 4, es porque los vecinos valen 3 y 1
			_puntos[t[k]]->color = 2;
			break;
		case 5: //Si la suma es 5, es porque los vecinos valen 2 y 3
			_puntos[t[k]]->color = 1;
			break;
		}
		}
	}
}


