/*
 * tokenizador.cpp
 *
 *  Created on: 11 de feb. de 2016
 *      Author: jaquer
 */

#include "tokenizador.h"
static const string base = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
static const std::map<int, char> mymap = { { -1, 'y' }, { -3, 'y' },
		{ -4, 'u' }, { -5, 'u' }, { -6, 'u' }, { -7, 'u' }, { -8, 'o' }, { -10,
				'o' }, { -11, 'o' }, { -12, 'o' }, { -13, 'o' }, { -14, 'o' }, {
				-15, 'ñ' }, { -16, 'o' }, { -17, 'i' }, { -18, 'i' },
		{ -19, 'i' }, { -20, 'i' }, { -21, 'e' }, { -22, 'e' }, { -23, 'e' }, {
				-24, 'e' }, { -25, 'c' }, { -27, 'a' }, { -21, 'e' },
		{ -25, 'a' }, { -27, 'a' }, { -28, 'a' }, { -29, 'a' }, { -30, 'a' }, {
				-31, 'a' }, { -32, 'a' }, { -35, 'y' }, { -36, 'u' },
		{ -37, 'u' }, { -38, 'u' }, { -39, 'u' }, { -40, 'o' }, { -42, 'o' }, {
				-43, 'o' }, { -44, 'o' }, { -45, 'o' }, { -46, 'o' },
		{ -47, 'ñ' }, { -49, 'i' }, { -50, 'i' }, { -51, 'i' }, { -52, 'i' }, {
				-53, 'e' }, { -54, 'e' }, { -55, 'e' }, { -56, 'e' },
		{ -57, 'c' }, { -59, 'a' }, { -60, 'a' }, { -61, 'a' }, { -62, 'a' }, {
				-63, 'a' }, { -64, 'a' } };

Tokenizador::Tokenizador(const string& delimitadoresPalabra,
		const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
	this->delimiters = delimitadoresPalabra;
	for (unsigned int i = 0; i < delimitadoresPalabra.length(); i++) {
		this->tokens.insert(
				std::pair<int, char>((int) delimitadoresPalabra[i],
						delimitadoresPalabra[i]));
	}
	this->casosEspeciales = kcasosEspeciales;
	this->pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador() {

	this->delimiters = base;
	this->tokens = { {44,','}, {59,';'}, {58,':'}, {46,'.'}, {45,'-'}, {47,'/'}, {43,'+'}, {42,'*'}, {92,'\\'}, {32,' '},
		{	39,'\''}, {34,'"'}, {123,'{'}, {125,'}'}, {91,'['}, {93,']'}, {40,'('}, {41,')'}, {60,'<'}, {62,'>'},
		{	33,'!'}, {-95,'¡'}, {-65,'¿'}, {63,'?'}, {38,'&'}, {35,'#'}, {61,'='}, {9,'\t'}, {10,'\n'}, {13,'\r'}, {64,'@'}};
	this->casosEspeciales = true;
	this->pasarAminuscSinAcentos = false;

}
Tokenizador::Tokenizador(const Tokenizador& t) {
	this->casosEspeciales = t.casosEspeciales;
	this->delimiters = t.delimiters;
	this->pasarAminuscSinAcentos = t.casosEspeciales;
	this->tokens = t.tokens;

}

Tokenizador::~Tokenizador() {
	this->delimiters.clear();
	this->tokens.clear();

}

void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
	string base = str;
	if (this->pasarAminuscSinAcentos) {

		base = this->toMinusNoAccent(str);
	}

	if (this->casosEspeciales) {
		this->tokenizarCasosEspeciales(base, tokens);
	} else {

		string::size_type lastPos = base.find_first_not_of(delimiters, 0);
		string::size_type pos = base.find_first_of(delimiters, lastPos);
		while (string::npos != pos || string::npos != lastPos) {
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			lastPos = base.find_first_not_of(delimiters, pos);
			pos = base.find_first_of(delimiters, lastPos);
		}
	}
}

bool Tokenizador::Tokenizar(const string& NomFichEntr,
		const string& NomFichSal) const {
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

bool Tokenizador::Tokenizar(const string& i) const {
	return this->Tokenizar(i, i + ".tk");

}

bool Tokenizador::TokenizarListaFicheros(const string& nomDir) const {

	string cadena; //TODO  posible optimizacion
	ifstream fe(nomDir.c_str());
	if (fe) {
		if (nomDir == ".lista_fich") {
			getline(fe, cadena);
		}
		while (!fe.eof()) {
			cadena.clear();
			getline(fe, cadena);
			//cout<<"lectura:"<<cadena<<endl;
			if (cadena.length() != 0) {

				struct stat dir;
				//string dirAIndexar = cadena;
				// Compruebo la existencia del directorio
				int err = stat(cadena.c_str(), &dir);
				//cout<<cadena<<endl;
				//cout<<err<<endl;
				if (err == -1 || !S_ISDIR(dir.st_mode)) {
					if (!this->Tokenizar(cadena)) {
						cerr << "Error de fichero" << endl;

					}
				} else {
					// Hago una lista en un fichero con find>fich
					string cmd =
							"find " + cadena
									+ " -follow |grep -v -f .lista_fich |sort > .lista_fich";
					system(cmd.c_str());
				}
			}

		}
	} else {
		cerr << "error de apertura de fichero" << endl;
		return false;
	}

	fe.close();

	return true;
}

bool Tokenizador::TokenizarDirectorio(const string& dirAIndexar) const {
	struct stat dir;
	// Compruebo la existencia del directorio
	int err = stat(dirAIndexar.c_str(), &dir);
	//cout<<err<<endl;
	if (err == -1 || !S_ISDIR(dir.st_mode)) {
		cerr << "Error en el directorio" << endl;
		return false;
	} else {
		// Hago una lista en un fichero con find>fich
		string cmd = "find " + dirAIndexar + " -follow |sort > .lista_fich";
		system(cmd.c_str());
		return TokenizarListaFicheros(".lista_fich"); //TODO mirrar error
	}
}

void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters) {
	this->delimiters = nuevoDelimiters;
	this->tokens.clear();
	for (unsigned int i = 0; i < this->delimiters.length(); i++) {
		this->tokens.insert(
				std::pair<int, char>((int) this->delimiters[i],
						this->delimiters[i]));
	}
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters) {

	this->delimiters += nuevoDelimiters;
	std::sort(this->delimiters.begin(), this->delimiters.end());
	this->delimiters.erase(
			std::unique(this->delimiters.begin(), this->delimiters.end()),
			this->delimiters.end());

	for (unsigned int i = 0; i < this->delimiters.length(); i++) {
		this->tokens.insert(
				std::pair<int, char>((int) this->delimiters[i],
						this->delimiters[i]));
	}
}

ostream& operator<<(ostream& salida, const Tokenizador& base) {
//TODO arreglar
	salida << "DELIMITADORES: " << base.DelimitadoresPalabra()
			<< " TRATA CASOS ESPECIALES:" << base.CasosEspeciales()
			<< " PASAR A MINUSCULAS Y SIN ACENTOS: "
			<< base.PasarAminuscSinAcentos();
	return salida;
}

void Tokenizador::tokenizarCasosEspeciales(const string& str,
		list<string>& tokens) const {
//TODO terminar
	int inicio = 0;
	this->estadoNada(str, tokens, inicio);

}

string Tokenizador::toMinusNoAccent(const string& str) const {

	std::locale loc;
	string noaccent = "";
	for (std::string::size_type i = 0; i < str.length(); ++i) {
		if (mymap.find((int) str[i])->second != NULL) { //TODO Optimizar
			//cout<<mymap.find((int) str[i])->second<<endl;
			noaccent.push_back(mymap.find((int) str[i])->second);

		} else {
			noaccent.push_back(std::tolower(str[i], loc));

		}

	}
	return noaccent;
}

void Tokenizador::estadoNada(const string& str, list<string>& tokens,
		int& index) const {

	string palabra = "";
//cout<<"tamano de str:"<<str.size();

	while (index <= str.size()) {
//cout<<"index:"<<index<<endl;
		int base = index;
		if (((int) str[index] >= 65 && (int) str[index] <= 90)
				|| ((int) str[index] >= 97 && (int) str[index] <= 122)) {

			this->estadoPosPalabra(str, tokens, index, base, palabra);
		} else if (((int) str[index] >= 48 && (int) str[index] <= 57)) {
			this->estadoPosNumero(str, tokens, index, base, palabra);
		} else if ((int) str[index] == 44 || (int) str[index] == 46) {
			int prueba = index;
			++prueba;
			if (((int) str[prueba] >= 48 && (int) str[prueba] <= 57)) {
				palabra.push_back('0');
				this->estadoPosNumero(str, tokens, index, base, palabra);
			} else {
				if (this->tokens.find((int) str[index])->second == NULL){
					palabra.push_back(str[index]);
				}
				++index;
			}

		}
		/*else if ((int) str[index] == 64) {
		 this->estadoPosEmail(str, tokens, index, base, palabra);
		 }*/else if (str[index] == ' ') {
			//encontrado delimitador
			++index;
			//this->estadoNada(str,tokens,++index);
		}

		else if (this->tokens.find((int) str[index])->second != NULL) {
			//encontrado delimitador
			//this->estadoNada(str,tokens,++index);
			++index;
		} else if (str[index] == '\0') {
			if (palabra != "") {
				this->estadoCortar(str, tokens, index, index, palabra);
				break;
			} else {
				break;
			}
		} else {
			//cerr << "error de carcter:" << str[index] << " no reconocido"
			//cout<<str[index]<<endl;
			palabra.push_back(str[index]);
			++index;
		}

	}

}

void Tokenizador::estadoPosPalabra(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
//TODO ver si en vez de usar &palabra puedo usar la lisa de tokens
	bool cortar = true;
	while (cortar) {

		//comporbar primero si palabra es ya http,https o ftp
		if (palabra == "http" || palabra == "https" || palabra == "ftp") {
			cortar = false;
			this->estadoPosUrl(str, tokens, index, pos, palabra);
		} else if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
				|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)) {
			// hay una letra
			palabra.push_back(str[pos]);
			++pos;
		} else if ((int) str[pos] == 64) {
			palabra.push_back(str[pos]);
			cortar = false;
			this->estadoPosEmail(str, tokens, index, ++pos, palabra);
		} else if ((int) str[pos] == 46) {
			palabra.push_back(str[pos]);
			cortar = false;
			this->estadoPosAcronimo1(str, tokens, index, ++pos, palabra);

		} else if ((int) str[pos] == 45) {
			palabra.push_back(str[pos]);
			cortar = false;
			this->estadoPosGuion(str, tokens, index, ++pos, palabra);
		} else if (str[pos] == ' ') {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		} else if (str[pos] == '\0') {
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		}

		else if (this->tokens.find((int) str[pos])->second != NULL) {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);

		} else {
			palabra.push_back(str[pos]);
			++pos;
		}

	}
}

void Tokenizador::estadoPosUrl(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
// 95 58 47 46 63 38 45 61 35 64
	bool cortar = true;
	while (cortar) {
		if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
				|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)
				|| ((int) str[pos] >= 48 && (int) str[pos] <= 57)) {
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosUrl(str,tokens,index,pos,palabra);
		} else {

			switch ((int) str[pos]) {
			case 95:
				break;
			case 58:
				break;
			case 47:
				break;
			case 46:
				break;
			case 63:
				break;
			case 38:
				break;
			case 45:
				break;
			case 61:
				break;
			case 35:
				break;
			case 64:
				break;
			default:
				if (str[pos] == ' ') {
					//encontrado delimitador
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);
				} else if (str[pos] == '\0') {
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);
				} else if (this->tokens.find((int) str[pos])->second != NULL) {
					//encontrado delimitador
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);

				} else {
					//cerr<<"error de carcter"<<endl;
					palabra.push_back(str[pos]);
					++pos;
					//this->estadoPosUrl(str,tokens,index,pos,palabra);
				}

			}

			//llamr a estado posibleURL
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosUrl(str,tokens,index,pos,palabra);
		}

	}
}

void Tokenizador::estadoPosEmail(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
	bool cortar = true;
	while (cortar) {				//cout << "Posible email: " << pos << endl;
		if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
				|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)
				|| ((int) str[pos] >= 48 && (int) str[pos] <= 57)) {

			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosEmail(str,tokens,index,pos,palabra);
		} else {

			switch ((int) str[pos]) {
			case 45:
				palabra.push_back(str[pos]);
				++pos;
				break;
			case 95:
				palabra.push_back(str[pos]);
				++pos;
				break;
			case 46:
				palabra.push_back(str[pos]);
				++pos;
				break;
			default:
				//si encuentro @ otra vez replantear todo
				if ((int) str[pos] == 64) {
					//cortar = false;
					//cout<<palabra<<endl;
					string primera;
					if (this->tokens.find(64)->second != NULL) {
						//esta puesta como delimitador
						primera = palabra.substr(0, palabra.find_first_of('@'));
						this->estadoCortar(str, tokens, index, pos, primera);
						palabra = palabra.substr(palabra.find_first_of('@') + 1,
								palabra.npos);
						//cout<<palabra<<endl;
						palabra.push_back(char(64));
						++pos;
					} else {
						//primera=palabra.substr(0,palabra.find_first_of('@')+1);
						//no esta como delimitador se añade;
						palabra.push_back(char(64));
						++pos;
					}
					//string primera=palabra.substr(0,palabra.find_first_of('@'));
					//cout<<primera<<endl;

				} else if (str[pos] == ' ') {
					//encontrado delimitador
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);
				} else if (str[pos] == '\0') {
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);
				}

				else if (this->tokens.find((int) str[pos])->second != NULL) {
					//encontrado delimitador
					cortar = false;
					this->estadoCortar(str, tokens, index, pos, palabra);

				} else {
					//cerr<<"error de carcter"<<endl;
					palabra.push_back(str[pos]);
					++pos;

					//this->estadoPosEmail(str,tokens,index,pos,palabra);
				}

			}

		}
	}
}

void Tokenizador::estadoPosNumero(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
	bool cortar = true;
	while (cortar) {

		if (((int) str[pos] >= 48 && (int) str[pos] <= 57)) {
			//posible numero
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosNumero(str,tokens,index,pos,palabra);
		} else if ((int) str[pos] == 36 || (int) str[pos] == 37
				|| (int) str[pos] == -86 || (int) str[pos] == -70
				|| (int) str[pos] == -92) {
			palabra.push_back('	');
			palabra.push_back(str[pos]);
			++pos;
		} /*else if ((int) str[pos] == 64) {
			palabra.push_back(str[pos]);
			++pos;
			cortar = false;
			this->estadoPosEmail(str, tokens, index, pos, palabra);

		}*/ else if ((int) str[pos] == 44 || (int) str[pos] == 46) {
			palabra.push_back(str[pos]);
			++pos;
			if (str[pos]
					== ' '|| str[pos] == '\0' || this->tokens.find((int) str[pos])->second != NULL) {
				//palabra.push_back('0');
				cortar = false;
				this->estadoCortar(str, tokens, index, pos,
						palabra.substr(0, palabra.length() - 1));
				palabra.clear();

			}
			else if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
							|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)) {
						palabra.push_back(str[pos]);
						++pos;
						cortar = false;
						this->estadoPosAcronimo1(str, tokens, index, pos, palabra);

					}
		} else if (str[pos]=='E') {
			int auxpos = pos;
			++auxpos;
			if(str[auxpos]=='+'){
				palabra.push_back('E');
				palabra.push_back('+');
				++pos;
				++pos;
			}
		}
		 else if (str[pos] == ' ') {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		} else if (str[pos] == '\0') {
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		}

		else if (this->tokens.find((int) str[pos])->second != NULL) {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);

		} else {
			//cerr<<"error de carcter"<<endl;
			//sustituye a estado numero especia

			palabra.push_back(str[pos]);
			++pos;
			//cortar = false;
			//this->estadoCortar(str, tokens, index, pos, palabra);
		}
	}

}

void Tokenizador::estadoCortar(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {

	//tokens.push_back(str.substr(index, pos - index));
	tokens.push_back(palabra);
	//cout<<palabra<<endl;
	//cout << str.substr(index, pos - index) << endl;
	//cout << pos << endl;
	palabra.clear();
	index = pos;
	/*if (str[pos] == '\0') {
	 this->estadoNada(str, tokens, actual);
	 } else {
	 this->estadoNada(str, tokens, ++actual);
	 }*/

}
void Tokenizador::estadoCortar(const string& str, list<string>& tokens,
		int& index, int& pos, const string& palabra) const {

	//tokens.push_back(str.substr(index, pos - index));
	tokens.push_back(palabra);
	//cout<<palabra<<endl;
	//cout << str.substr(index, pos - index) << endl;
	//cout << pos << endl;
	index = pos;
	/*if (str[pos] == '\0') {
	 this->estadoNada(str, tokens, actual);
	 } else {
	 this->estadoNada(str, tokens, ++actual);
	 }*/

}

void Tokenizador::estadoPosGuion(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {

	bool cortar = true;
	while (cortar) {
		if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
				|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)) {
			// hay una letra
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosGuion(str,tokens,index,++pos,palabra);
		} else if ((int) str[index] >= 48 && (int) str[index] <= 57) {
			//hay un numero
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosGuion(str,tokens,index,++pos,palabra);
		} else if (str[pos] == 45) {
			//encontrado delimitador
			palabra.push_back(str[pos]);
			++pos;
		}

		else if (str[pos] == ' ') {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		} else if (str[pos] == '\0') {
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		}

		else if (this->tokens.find((int) str[pos])->second != NULL) {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);

		} else {
			//cerr<<"error de carcter"<<endl;

			palabra.push_back(str[pos]);
			++pos;
		}
	}

}

void Tokenizador::estadoPosAcronimo1(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
//TODO ARREGLAR PARA CUANDO NO SEA UN ACRONIMO;
	if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
			|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)) {
		// hay una letra
		palabra.push_back(str[pos]);
		this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
	} else if ((int) str[index] >= 48 && (int) str[index] <= 57) {
		//hay un numero
		palabra.push_back(str[pos]);
		this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
	} else if (str[pos] == ' ') {
		//encontrado delimitador
		//palabra.pop_back();
		//cout<<palabra[palabra.length() - 1]<<endl;
		if (this->tokens.find(palabra[palabra.length() - 1])->second != NULL) {
			this->estadoCortar(str, tokens, index, pos,
					palabra.substr(0, palabra.length() - 1));
			palabra.clear();
		} else {
			this->estadoCortar(str, tokens, index, pos, palabra);
		}
		palabra.clear();
	} else if (str[pos] == '\0') {

		if (this->tokens.find(palabra[palabra.length() - 1])->second != NULL) {
			this->estadoCortar(str, tokens, index, pos,
					palabra.substr(0, palabra.length() - 1));
			palabra.clear();
		} else {
			this->estadoCortar(str, tokens, index, pos, palabra);
		}
		palabra.clear();
	}

	else if (this->tokens.find((int) str[pos])->second != NULL) {
		//encontrado delimitador
		if (this->tokens.find(palabra[palabra.length() - 1])->second != NULL) {
			this->estadoCortar(str, tokens, index, pos,
					palabra.substr(0, palabra.length() - 1));
			palabra.clear();
		} else {
			this->estadoCortar(str, tokens, index, pos, palabra);
		}
		palabra.clear();

	} else {
		//cerr<<"error de carcter"<<endl;

		palabra.push_back(str[pos]);
		this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
	}

}

void Tokenizador::estadoPosAcronimo2(const string& str, list<string>& tokens,
		int& index, int& pos, string& palabra) const {
	bool cortar = true;
	while (cortar) {

		if (((int) str[pos] >= 65 && (int) str[pos] <= 90)
				|| ((int) str[pos] >= 97 && (int) str[pos] <= 122)) {
			// hay una letra
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
		} else if ((int) str[pos] >= 48 && (int) str[pos] <= 57) {
			//hay un numero
			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
		} else if ((int) str[pos] == 46) {
			palabra.push_back(str[pos]);
			int auxpos = pos;
			++auxpos;
			if (str[auxpos]== ' '|| str[auxpos] == '\0' || this->tokens.find((int) str[auxpos])->second != NULL) {
				cortar = false;
				++pos;
				this->estadoCortar(str, tokens, index, pos,palabra.substr(0, palabra.length() - 1));
							palabra.clear();
			}else{
				cortar = false;
				this->estadoPosAcronimo1(str, tokens, index, ++pos, palabra);
			}


		}

		else if (str[pos] == ' ') {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		} else if (str[pos] == '\0') {
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);
		}

		else if (this->tokens.find((int) str[pos])->second != NULL) {
			//encontrado delimitador
			cortar = false;
			this->estadoCortar(str, tokens, index, pos, palabra);

		} else {
			//cerr<<"error de carcter"<<endl;

			palabra.push_back(str[pos]);
			++pos;
			//this->estadoPosAcronimo2(str, tokens, index, ++pos, palabra);
		}
	}
}
