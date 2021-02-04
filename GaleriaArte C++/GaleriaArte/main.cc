#include "geometry.h"
#include "parse.h"

int main(int argc, char **argv)
{
	Options options;
        parse(argc, argv, options);

        string nombreArchivo=argv[options.fileindex]; //input bdm file name; 	
        Polygon poly(nombreArchivo,options.parsebdm);
	poly.setDebugOption(options.debug);      //set debug flag;
       	
	poly.triangulacion();                    //main triangulacion function
	
	poly.colorear();						// El poligono debe arrancar con un punto en el (0,0)
	                                        // En lo posible, debe ser punto de tipo end.
	                                          
	if(options.showme) poly.saveAsShowme();
	return 1;
}





