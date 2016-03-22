/*
 * tokenizador.h
 *
 *  Created on: 11 de feb. de 2016
 *      Author: jaquer
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <list>
#include <map>
using namespace std;
#ifndef TOKENIZADOR_H_
#define TOKENIZADOR_H_

class Tokenizador {
	friend ostream& operator<<(ostream&, const Tokenizador&);
	// cout << “DELIMITADORES: “ << delimiters << “ TRATA CASOS ESPECIALES:
	//“ << casosEspeciales << “ PASAR A MINUSCULAS Y SIN ACENTOS: “ <<
	//pasarAminuscSinAcentos;
	// Aunque se modifique el almacenamiento de los delimitadores por temas
	//de eficiencia, el campo delimiters se imprimirá con el string con el que
	//se inicializó el tokenizador
	public:
	Tokenizador (const string& delimitadoresPalabra, const bool&
	kcasosEspeciales, const bool& minuscSinAcentos);
	// Inicializa delimiters a delimitadoresPalabra; casosEspeciales a
	//kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
	Tokenizador ();
	Tokenizador(const Tokenizador& t);
	~Tokenizador();
	// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
	//casosEspeciales a true; pasarAminuscSinAcentos a false
	void CasosEspeciales (const bool& nuevoCasosEspeciales){this->casosEspeciales = nuevoCasosEspeciales; };
	// Cambia la variable privada “casosEspeciales”
	bool CasosEspeciales () const{ return this->casosEspeciales; }
	// Devuelve el contenido de la variable privada “casosEspeciales”
	void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){this->pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;}
	// Cambia la variable privada “pasarAminuscSinAcentos”. Atención al
	//formato de codificación del corpus (comando “file” de Linux). Para la
	//corrección de la práctica se utilizará el formato actual (ISO-8859).
	bool PasarAminuscSinAcentos ()const  {return this->pasarAminuscSinAcentos;}
	// Devuelve el contenido de la variable privada “pasarAminuscSinAcentos”

	void Tokenizar (const string& str, list<string>& tokens) const;
	// Tokeniza str devolviendo el resultado en tokens. La lista tokens se vaciará antes de almacenar el resultado de la tokenización.
	// El código de esta función tal y como se ha visto en clase se muestra en este enunciado a continuación del prototipo de la clase TokenizadorClase
	bool Tokenizar (const string& i, const string& f) const;
	// Tokeniza el fichero i guardando la salida en el fichero f (una palabra en cada línea del fichero). Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
	bool Tokenizar (const string & i) const;
	// Tokeniza el fichero i guardando la salida en un fichero de nombre i añadiéndole extensión .tk (sin eliminar previamente la extensión de i por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk), y que contendrá una palabra en cada línea del fichero. Devolverá true si se realiza la tokenización de forma correcta enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
	bool TokenizarListaFicheros (const string& i) const;
	// Tokeniza el fichero i que contiene un nombre de fichero por línea guardando la salida en un fichero cuyo nombre será el de entrada añadiéndole extensión .tk, y que contendrá una palabra en cada línea del fichero. Devolverá true si se realiza la tokenización de forma correcta de todos los archivos que contiene i; devolverá false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i, o bien enviando a “cerr” los archivos de i que no existan)
	bool TokenizarDirectorio (const string& i) const;

	void DelimitadoresPalabra(const string& nuevoDelimiters);
	// Cambia �delimiters” por “nuevoDelimiters”
	void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);

	string DelimitadoresPalabra() const{return this->delimiters;};

	private:
	string delimiters;
	// Delimitadores de términos. Aunque se
	//modifique la forma de almacenamiento interna para mejorar la eficiencia, este
	//campo debe permanecer para indicar el orden en que se introdujeron los
	//delimitadores
	bool casosEspeciales;
	// Si true detectará palabras compuestas y casos especiales
	bool pasarAminuscSinAcentos;
	// Si true pasará el token a minúsculas y quitará acentos, antes de
	//realizar la tokenización
	void tokenizarCasosEspeciales (const string& str, list<string>& tokens)const ;
	string toMinusNoAccent(const string&)const;
	map<unsigned int,char> tokens;
	void estadoNada(const string& str,list<string>& tokens,int& index)const;
	void estadoCortar(const string& str, list<string>& tokens,int& index, int& pos,const string& palabra)const;
	void estadoPosPalabra(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosUrl(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosEmail(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosNumero(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoNumeroEspecial(const string& str,list<string>& tokens,int& index,int pos,string& actual)const;
	void estadoCortar(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosGuion(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosAcronimo1(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;
	void estadoPosAcronimo2(const string& str,list<string>& tokens,int& index,int& pos,string& actual)const;

};

#endif /* TOKENIZADOR_H_ */
