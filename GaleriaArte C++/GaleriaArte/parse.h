#ifndef  _PARSE_H
#define  _PARSE_H

#include <string>
#include <iostream>

class Options
{
   public:
     Options():parsebdm(true),showme(false), fileindex(1) 
     { 
     }
     
	 bool parsebdm, showme;
     int  fileindex;
};

	void help()
	{
		cout<<"Algoritmo de triangulacion, coloreo y colocacion de guardias.\n";
		cout<<"Uso: funcion [-sh] <nombreArchivo>.\n";
		cout<<"Opciones:\n";
		cout<<"     -s: Devuelve en formato showme (que se puede ver en MATLAB).\n";
		cout<<"     -h: Este archivo de ayuda.\n\n";
	}

	void parse(int argc, char **argv, Options& options)
	{
	  unsigned int num=5;
	  unsigned int files=0, fileindex=1;
  
	  string  option, start, end;
  
	  if( argc<2 ) { help(); exit(1); }
	  for (int k = 1; k < argc; k++)
	  {
		 option=argv[k];
		 if(option[0]!='-') { fileindex=k; files++; continue; } 
		 if(option.size() >= num+1 || option[0]!='-' || option.size() <= 1 ) { help(); exit(1); }
			 for(unsigned int i=1; i < option.size(); i++) 
		 {
			options.showme = true;    break;
			 switch (option[i])
			 {
				 case 'S': options.showme=true;    break;
				 case 's': options.showme=true;    break;
				 case 'h': help(); exit(1);        break;
				 case 'H': help(); exit(1);        break;
				 default : help(); exit(1);        break;
			 }
		 }
	   }

	  options.fileindex=fileindex;
  
	  if( files > 1 ) { cout<<"Solo un archivo, por favor!\n"; exit(1); }
	  else if (files <=0 ) { help(); exit(1); }

	}

#endif