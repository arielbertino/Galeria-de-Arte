#ifndef _ARBIN_H_
#define _ARBIN_H_

#include <iostream>  
#include <iomanip>
#include <assert.h>

template <class T, class ValorClave>
class Arbin;

template <class T, class ValorClave>
class Nodo
{
  public:
      friend class Arbin<T, ValorClave>;
      Nodo( ) : _left( NULL ), _right( NULL ), _visited(false) { }
      Nodo( const T & data, Nodo *lt, Nodo *rt )
        : _data(data),_left( lt ), _right( rt ), _visited(false) { }
     
      T& data()                     { return _data; }
      Nodo* Left()             { return _left; }
      Nodo* Right()            { return _right; }   
      void SetVisited(bool visited) { _visited=visited; }
      bool GetVisited()             { return _visited; }
      ValorClave valorClave()            { return _data->valorClave(); }

  private:
      T          _data;
      Nodo *_left;
      Nodo *_right;
      bool      _visited;

};


template <class T, class ValorClave>
class Arbin
{
  public:
      explicit Arbin( ):root(NULL),size(0) { }
      Arbin( const Arbin & rhs );
      ~Arbin( );

    
      bool IsEmpty( ) const;
      long int Size() { return size; }
      Nodo<T, ValorClave>* Root() { return root; }     

  

      void FindMaxSmallerThan( const ValorClave& keys, Nodo<T, ValorClave>* &res);
      
      void Insert( const T & x );
      void Delete( ValorClave keys);
      void Delete( ValorClave keys, Nodo<T, ValorClave>* &res);
      void InOrder( void(*Visit)(Nodo<T,ValorClave> *u) ) 
                { InOrder(Visit, root); }

      void InOrder( void(*Visit)(Nodo<T,ValorClave>*u, ostream&), ostream& os)
                { InOrder(Visit, root, os); }
      
      void InOrder( void(*Visit)(Nodo<T,ValorClave>*u, double y), double y)
	                      { InOrder(Visit, root, y); }

      Nodo<T, ValorClave>* Left(Nodo<T, ValorClave> *node) { return node->_left; }
      Nodo<T, ValorClave>* Right(Nodo<T, ValorClave> *node) { return node->_right; }     

  private:
      Nodo<T, ValorClave> *root;
      long int              size;          


      void PreOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t);
      void InOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t);
      void PostOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t);
      void InOrder( void(*Visit)(Nodo<T, ValorClave>*, ostream&), 
		Nodo<T, ValorClave> *t, ostream& os);
      
      void InOrder( void(*Visit)(Nodo<T, ValorClave>*, double y),
		                      Nodo<T, ValorClave> *t, double y);
      


      // Manipulación del árbol
      void rotarAIzquierda( Nodo<T, ValorClave> * & k2 ) const;
      void rotarADerecha( Nodo<T, ValorClave> * & k1 ) const;
      void arbin( ValorClave keys, Nodo<T, ValorClave> * & t ) const;
};

//----------------------------------------------------------------------
//Constructor;
//----------------------------------------------------------------------
template <class T, class ValorClave>
Arbin<T, ValorClave>::Arbin( const Arbin<T, ValorClave> & rhs )
{
      *this = rhs;
}

//-----------------------------------------------------------------------
//Destructor.
//-----------------------------------------------------------------------
template <class T, class ValorClave>
Arbin<T, ValorClave>::~Arbin( )
{
   //   MakeEmpty( );
}

//------------------------------------------------------------------------
//Insertar en el árbol
//------------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::Insert( const T & x )
{

     Nodo<T, ValorClave> *newNode= new Nodo<T, ValorClave>;
     newNode->_data=x;
    
     if( root == NULL )
      {
          newNode->_left = newNode->_right = NULL;
          root = newNode; size++; 
      } 
     else
      {
         ValorClave keys=x->valorClave();
         arbin( keys, root );
         ValorClave rootk=root->valorClave();
         if( keys < rootk )
           {
              newNode->_left = root->_left;
              newNode->_right = root;
              root->_left = NULL;
              root = newNode; 
              size++;
           }
        else if( keys > rootk )
           {
              
              newNode->_right = root->_right;
              newNode->_left = root;
              root->_right = NULL;
              root = newNode;
              size++;
           }
        else 
           {
             //Para evitar claves duplicadas.		   
             x->incrementarvalorClave(1.0e-10);
             Insert(x);
	     
           }         
      }
}

//---------------------------------------------------------------------
//Sacamos el nodo con la clave indicada
//---------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::Delete( ValorClave keys, Nodo<T, ValorClave>* &res)
{
      Nodo<T, ValorClave> *newTree;

      arbin( keys, root );
      if( root->valorClave() != keys ) { res=NULL; return; } // Item not found; do nothing
 
      res = root;

      if( root->_left == NULL )
           newTree = root->_right;
      else
         {
            // Find the maximum in the _left subtree
            // arbin it to the root; and then attach _right child
            newTree = root->_left;
            arbin( keys, newTree );
            newTree->_right = root->_right;
         }

      root = newTree;
      size--; 
}

//---------------------------------------------------------------------
//Sacamos los nodos con las claves
//---------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::Delete( ValorClave keys)
{
      Nodo<T, ValorClave> *newTree;

      arbin( keys, root );
      ValorClave rootk=root->valorClave();
      if( rootk != keys ) { return; } // Item not found; do nothing

      if( root->_left == NULL ) newTree = root->_right;
      else
         {
            // Find the maximum in the _left subtree
            // arbin it to the root; and then attach _right child
            newTree = root->_left;
            arbin( keys, newTree );
            newTree->_right = root->_right;
         }

      delete root;
      root = newTree;
      size--;
}


//--------------------------------------------------------------------
//Encuentra el nodo más grande con clave más pequeña que la indicada
//Se usa en triangulación.
//--------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::FindMaxSmallerThan( const ValorClave& keys, Nodo<T, ValorClave>* &res)
{
      if( IsEmpty( ) ) { res=NULL; return; }
      arbin( keys, root );
      
      if( root->data()->valorClave() < keys) res=root; 
      else if(root->_left) 
      {       
	      res=root->_left;
	      while(res->_right) res=res->_right;  
      }
      else 
      {
	      assert(false);
      }
}


//---------------------------------------------------------------------
//Ver si está vacío.
//---------------------------------------------------------------------
template <class T, class ValorClave>
bool Arbin<T, ValorClave>::IsEmpty( ) const
{
      return root == NULL;
}



//-----------------------------------------------------------------------
//Método interno Top-down
//-----------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::arbin( ValorClave keys, Nodo<T, ValorClave> * & t ) const
{
      Nodo<T, ValorClave> *_leftTreeMax, *_rightTreeMin;
      static Nodo<T, ValorClave> header;

      header._left = header._right = NULL;
      _leftTreeMax = _rightTreeMin = &header;

      for( ; ; )
        {
          ValorClave rkey=t->valorClave();
          if( keys < rkey )
            {
               if(t->_left == NULL) break;
               if( keys < t->_left->valorClave() ) rotarAIzquierda( t );
                if( t->_left == NULL ) break;
               
               // Ir a derecha
               _rightTreeMin->_left = t;
               _rightTreeMin = t;
               t = t->_left;
            }
          else if( keys > rkey )
            {
              if( t->_right == NULL ) break;
              if( keys > t->_right->valorClave() ) rotarADerecha( t );
              if( t->_right == NULL ) break;    
 
              // Ir a la izquierda
              _leftTreeMax->_right = t;
              _leftTreeMax = t;
              t = t->_right;
             }
         else  break;
        }

      _leftTreeMax->_right = t->_left;
      _rightTreeMin->_left = t->_right;
      t->_left = header._right;
      t->_right = header._left;
 
}

//--------------------------------------------------------------------
//Rotar el árbol binario
//--------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::rotarAIzquierda( Nodo<T, ValorClave> * & k2 ) const
{
      Nodo<T, ValorClave> *k1 = k2->_left;
      k2->_left = k1->_right;
      k1->_right = k2;
      k2 = k1;
}

//---------------------------------------------------------------------
//Rotar el árbol a la derecha
//---------------------------------------------------------------------
template <class T, class ValorClave>
void Arbin<T, ValorClave>::rotarADerecha( Nodo<T, ValorClave> * & k1 ) const
{
      Nodo<T, ValorClave> *k2 = k1->_right;
      k1->_right = k2->_left;
      k2->_left = k1;
      k1 = k2;
}


//-----------------------------------------------------------------------
//Preorder
//-----------------------------------------------------------------------
template<class T, class ValorClave>
void Arbin<T, ValorClave>::PreOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t)
{
   if(t!=NULL)
    {
       Visit(t);
       PreOrder(Visit,t->_left);
       PreOrder(Visit,t->_right);
    } 
     
}

//-----------------------------------------------------------------------
//In order.
//-----------------------------------------------------------------------
template<class T, class ValorClave>
void Arbin<T, ValorClave>::InOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left);
        Visit(t);
        InOrder(Visit,t->_right);
      }
}


//-----------------------------------------------------------------------
//In order
//-----------------------------------------------------------------------
template<class T, class ValorClave>
void Arbin<T, ValorClave>::InOrder( void(*Visit)(Nodo<T, ValorClave>*u, ostream&)
, Nodo<T, ValorClave> *t, ostream& os)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left, os);
        Visit(t, os);
        InOrder(Visit,t->_right, os);
      }
}

//-----------------------------------------------------------------------
//In order
//-----------------------------------------------------------------------
template<class T, class ValorClave>
void Arbin<T, ValorClave>::InOrder( void(*Visit)(Nodo<T, ValorClave>*u, double y)
, Nodo<T, ValorClave> *t, double y)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left, y);
        Visit(t, y);
        InOrder(Visit,t->_right, y);
      }
}



//-----------------------------------------------------------------------
//Pos order
//-----------------------------------------------------------------------
template<class T, class ValorClave>
void Arbin<T, ValorClave>::PostOrder( void(*Visit)(Nodo<T, ValorClave> *u), Nodo<T, ValorClave> *t)
{
   if(t!=NULL)
     {
        PostOrder(Visit,t->_left);
        PostOrder(Visit,t->_right);
        Visit(t);
     }
}



#endif

