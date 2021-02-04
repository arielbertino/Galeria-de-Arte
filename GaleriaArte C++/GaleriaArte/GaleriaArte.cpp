// GaleriaArte.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "geometry.h"
#include "parse.h"

int main(int argc, char **argv)
{
	Options options;
	parse(argc, argv, options);

	string nombreArchivo = argv[options.fileindex];  	
	Polygon poly(nombreArchivo, options.parsebdm);

	poly.triangulacion();                    //main triangulacion function

	poly.colorear();						// El poligono debe arrancar con un punto en el (0,0)
											// En lo posible, debe ser punto de tipo end.

	if (options.showme) poly.saveAsShowme();
	cout << "El programa termino con exito";
	system("PAUSE");
	return 1;
}

