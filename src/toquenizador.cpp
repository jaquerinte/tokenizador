//============================================================================
// Name        : toquenizador.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <list>
#include <sys/resource.h>
#include "tokenizadorClase.h"
#include "tokenizador.h"

using namespace std;
double getcputime(void) {
struct timeval tim;
struct rusage ru;
getrusage(RUSAGE_SELF, &ru);
tim=ru.ru_utime;
double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
tim=ru.ru_stime;
t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
return t;
}

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;

}

int
main(void)
{


long double aa=getcputime();
bool kCasosEspeciales = true, kpasarAminusculas = false;

	list<string> lt1, lt2;

Tokenizador a("-#", true, false);
list<string> tokens;

//a.DelimitadoresPalabra("@.&");
//a.Tokenizar("U.S.A p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "U.S.A, p1, e.g, p2, La"
//	imprimirListaSTL(tokens);
//	tokens.clear();

a.DelimitadoresPalabra("");
a.Tokenizar(".U.S.A e.g. p2. La", tokens);
//a.Tokenizar("U.S.A .U.S.A .p1 p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "U.S.A, U.S.A, .p1, p1, e.g, p2., La"
	imprimirListaSTL(tokens);
	tokens.clear();

a.Tokenizar("a&U.S.A p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "a&U.S.A, p1, e.g, p2., La"
	imprimirListaSTL(tokens);
	tokens.clear();

a.DelimitadoresPalabra("&");
a.Tokenizar("a&U.S.A p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "a, U.S.A, p1, e.g, p2., La"
	imprimirListaSTL(tokens);
	tokens.clear();


a.PasarAminuscSinAcentos(true);
a.Tokenizar("a&U.S.A p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "a, u.s.a, p1, e.g, p2., la"
	imprimirListaSTL(tokens);
	tokens.clear();

a.DelimitadoresPalabra(".&");
a.CasosEspeciales (false);
a.Tokenizar("a&U.S.A p1 e.g. p2. La", tokens);
// La lista de tokens a devolver debería contener: "a, u, s, a p1 e, g,  p2,  la"
	imprimirListaSTL(tokens);

	tokens.clear();
cout << "Ha tardado " << getcputime() - aa << " segundos" << endl;


}
