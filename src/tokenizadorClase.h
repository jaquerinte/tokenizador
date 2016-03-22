/*
 * TokenizadorClase.h
 *
 *  Created on: 29/01/2016
 *      Author: irf12
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <list>
using namespace std;

#ifndef TOKENIZADORCLASE_H_
#define TOKENIZADORCLASE_H_

class TokenizadorClase {
friend ostream& operator<<(ostream&, const TokenizadorClase&);
// cout << “DELIMITADORES: “ << delimiters;
public:
TokenizadorClase (const TokenizadorClase&);
TokenizadorClase (const string& delimitadoresPalabra);
// Inicializa variable privada delimiters a delimitadoresPalabra
TokenizadorClase ();
// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@"
~TokenizadorClase ();
// Pone delimiters=""
TokenizadorClase& operator= (const TokenizadorClase&);
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
// Cambia “delimiters” por “nuevoDelimiters”
void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);

string DelimitadoresPalabra() const{return this->delimiters;};

private:

string delimiters;
// Delimitadores de términos. Aunque se modifique la forma de

};



#endif /* TOKENIZADORCLASE_H_ */
