/*
 * TokenizadorClase.cpp
 *
 *  Created on: 29/01/2016
 *      Author: irf12
 */

#include "tokenizadorClase.h"
static const string base=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
TokenizadorClase::TokenizadorClase() {

	// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@"
	this->delimiters= base;

}

TokenizadorClase::TokenizadorClase(const TokenizadorClase& base) {

	this->delimiters=base.delimiters;

}

TokenizadorClase::TokenizadorClase(const string& delimitadoresPalabra) {
	this->delimiters=delimitadoresPalabra;
}

TokenizadorClase::~TokenizadorClase() {

	this->delimiters.clear();
}

TokenizadorClase& TokenizadorClase::operator =(const TokenizadorClase& base) {
	(*this).~TokenizadorClase();
	this->delimiters = base.delimiters;
	return *this;
}

void TokenizadorClase::Tokenizar(const string& str,list<string>& tokens) const {
	// Versión del tokenizador vista en clase

	string::size_type lastPos = str.find_first_not_of(delimiters,0);
	string::size_type pos = str.find_first_of(delimiters,lastPos);
	while(string::npos != pos || string::npos != lastPos)
	{
	tokens.push_back(str.substr(lastPos, pos - lastPos));
	lastPos = str.find_first_not_of(delimiters, pos);
	pos = str.find_first_of(delimiters, lastPos);
	}
}

bool TokenizadorClase::Tokenizar(const string& NomFichEntr, const string& NomFichSal) const {
	ifstream i;
	ofstream f;
	string cadena;
	list<string> tokens;
	i.open(NomFichEntr.c_str());
	if (!i) {
		cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
		return false;
	} else {
		while (!i.eof()) {
			cadena = "";
			getline(i, cadena);
			if (cadena.length() != 0) {
				Tokenizar(cadena, tokens);
			}
		}
	}
	i.close();
	f.open(NomFichSal.c_str());
	list<string>::iterator itS;
	for (itS = tokens.begin(); itS != tokens.end(); itS++) {
		f << (*itS) << endl;
	}
	f.close();
	return true;
}





bool TokenizadorClase::Tokenizar(const string& i) const {
	return this->Tokenizar(i,i+".tk");

}

bool TokenizadorClase::TokenizarListaFicheros(const string& nomDir) const {

string cadena; //TODO  posible optimizacion
	ifstream fe(nomDir.c_str());
	if(!fe){
if(nomDir == ".lista_fich" ){
	getline(fe,cadena);
}
	   while(!fe.eof()) {
		   cadena.clear();
		   getline(fe,cadena);
		   if(cadena.length() != 0){

			    struct stat dir;
			    string dirAIndexar = cadena;
			   	// Compruebo la existencia del directorio
			   	int err=stat(dirAIndexar.c_str(), &dir);
			   	if(err==-1 || !S_ISDIR(dir.st_mode)){
			   	 if(!this->Tokenizar(cadena)){
			   		 cerr << "Error de fichero"<<endl;
			   		 return false;
			   	 }
			   	}
			   	else {
			   	// Hago una lista en un fichero con find>fich
			   	string cmd="find "+dirAIndexar+" -follow |grep -v -f .lista_fich |sort > .lista_fich";
			   	system(cmd.c_str());
			   	}
		   }


	   }
	}
	else{
		cerr<<"error de apertura de fichero"<<endl;
		return false;
	}


	   fe.close();

return true;
}

bool TokenizadorClase::TokenizarDirectorio(const string& dirAIndexar) const {
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(dirAIndexar.c_str(), &dir);
	if(err==-1 || !S_ISDIR(dir.st_mode))
	return false;
	else {
	// Hago una lista en un fichero con find>fich
	string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich";
	system(cmd.c_str());
	return TokenizarListaFicheros(".lista_fich");//TODO mirrar error
	}
}

void TokenizadorClase::DelimitadoresPalabra(const string& nuevoDelimiters) {
	this->delimiters=nuevoDelimiters;
}

void TokenizadorClase::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters) {

	//string a;

	this->delimiters += nuevoDelimiters;
	std::sort(this->delimiters.begin(), this->delimiters.end());
	this->delimiters.erase(std::unique(this->delimiters.begin(),this->delimiters.end()), this->delimiters.end());



}

ostream& operator<<(ostream& salida, const TokenizadorClase& base){

salida << "DELIMITADORES: " + base.DelimitadoresPalabra();
return salida;
}


