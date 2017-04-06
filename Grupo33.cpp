//Jose Luis Moreno y Desiree Ventencourt Rosas (1B)
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <Windows.h>
#include <cctype>
#include <fstream>
#include <string>
#include <conio.h>
using namespace std;

typedef enum { Arriba, Abajo, Derecha, Izquierda, Salir, Nada }tAccion;

const int DIM = 4;

typedef int tTablero[DIM][DIM];
//------ Version 1 ------//
void inicializarTablero(tTablero tablero);
void visualiza(const tTablero tablero, int puntos, int puntosTotales); // contiene las funciones tableroSup, tableroMed y tableroInf
void tableroSup(); //Muestra la primeras lineas del tablero
void tableroMed(const tTablero tablero); // Muestra la parte media del tablero (fichas incluidas)
void tableroInf(); //muestra la ultima linea del tablero
void colorFondo(int color); //cambia el color de la consola
int log2(int num); 
void carga(tTablero tablero, int &puntos);
//------ Version 2 ------//
tAccion leeAccion(); // la accion introducida aqui, pasa a la funcion mueveFichas
void mueveFichas(tTablero tablero, tAccion accion); // dependiendo de la accion introducida, se elige entre arriba, abajo, derecha o izquierda
void moverArriba(tTablero tablero);
void moverAbajo(tTablero tablero);
void moverDerecha(tTablero tablero);
void moverIzquierda(tTablero tablero);
//------ Version 3 ------//
int mayor(const tTablero tablero);//Devuelve la mayor ficha en el tablero (la mayor posible es 2048)
bool lleno(const tTablero tablero);//Devuelve true si no hay ninguna casilla libre
void combinaFichas(tTablero tablero, tAccion accion, int &puntos);//Dada una direccion, y una vez movidas las fichas en dicha dir, las junta si son iguales
void nuevaFicha(tTablero tablero); //añade una nueva ficha aleatoriamente (2->95%//4->5%)
void juntarArriba(tTablero tablero, int &puntos);
void juntarAbajo(tTablero tablero, int &puntos);
void juntarDerecha(tTablero tablero, int &puntos);
void juntarIzquierda(tTablero tablero, int &puntos);
//------ Version 4 ------//
void guarda(const tTablero tablero, int puntos);
//------ Programa ------//
int main(){
	tTablero tablero;
	tAccion accion = Nada;
	int puntos = 0, puntosTotales = 0;
	bool tableroLleno = false, finalizado = false;
	char getOut; //variable para salir del programa y guardar la partida
	srand((unsigned int)time(NULL));
	carga(tablero, puntosTotales);
	do{
		visualiza(tablero, puntos, puntosTotales);
		puntos = 0;
		cout << "Usa las teclas de direccion (Esc para salir)..." << endl;
		accion = leeAccion();
		if (accion == Salir){
			cout << "Estas seguro de querer salir? [S/N]: ";
			cin >> getOut;
			if (toupper(getOut) == 'S') {
				cout << "Quieres guardar la partida? [S/N]: ";
				cin >> getOut;
				if (toupper(getOut) == 'S'){
					guarda(tablero, puntosTotales);
				}
				cout << "Has abandonado la partida...Hasta otra!" << endl;
				finalizado = true;
			}
			else visualiza(tablero, puntos, puntosTotales); // esto quita las lineas donde sale el texto de salir y demas
		}
		else{
			mueveFichas(tablero, accion);
			combinaFichas(tablero, accion, puntos);
			mueveFichas(tablero, accion);
			puntosTotales = puntosTotales + puntos;
			tableroLleno = lleno(tablero);
			visualiza(tablero, puntos, puntosTotales);
			if (mayor(tablero) >= 2048){
				cout << "Enhorabuena! Has ganado!" << endl;
				finalizado = true;
			}
			else if (tableroLleno){
					cout << "Lo siento, no quedan mas movimientos" << endl;
			}
			else{
				nuevaFicha(tablero);
			}
		}
	} while ((!tableroLleno) && (!finalizado));
	system("pause");
	return 0;
}

void inicializarTablero(tTablero tablero){
	int pos, posAux = -1, columna, fila, num, aux = -1, dim = DIM*DIM - 1; // dim es el numero de casillas individuales que hay en el array
	for (int i = 0; i < DIM; i++){
		for (int j = 0; j < DIM; j++){
			tablero[i][j] = 1;
		}
	}
	for (int i = 0; i < 2; i++){
		do{
			pos = rand() % (dim + 1); //Valor entre 0--(dim-1)
		} while (pos == posAux); //si vuelve a salir la misma posicion de antes, se repite hasta que salga una que no es
		fila = pos / DIM; // nos da la posicion de la fila a que le corresponde la posicion de pos
		columna = pos % DIM; // nos da la posicion de la columna a que le corresponde la posicion de pos
		num = (rand() % 100) + 1; // valor aleatorio entre 1--100
		if ((num > 95) && (aux != 4)){
			tablero[fila][columna] = 4;
			aux = 4;
			posAux = pos;
		}
		else tablero[fila][columna] = 2;
		posAux = pos;
	}
}

void visualiza(const tTablero tablero, int puntos, int puntosTotales){
	system("cls");
	cout << setw(6) << puntos << setw(16) << "Total: " << puntosTotales << endl;
	tableroSup();
	tableroMed(tablero);
	tableroInf();
}

void tableroSup(){
	colorFondo(0);
	cout << char(218);
	for (int i = 0; i < DIM; i++){
		for (int j = 0; j < 6; j++){
			cout << char(196);
		}
		if (i >= DIM - 1){
			cout << char(191) << endl;
		}
		else cout << char(194);
	}
}

void tableroMed(const tTablero tablero){
	int columna;
	for (int fila = 0; fila < DIM; fila++){
		for (columna = 0; columna < DIM; columna++){
			cout << char(179);
			colorFondo(log2(tablero[fila][columna]));
			cout << right << setw(6) << "";
			colorFondo(0);
		}
		cout << char(179) << endl;
		for (columna = 0; columna < DIM; columna++) {
			cout << char(179);
			colorFondo(log2(tablero[fila][columna]));
			if (tablero[fila][columna] != 1){
				cout << setw(6) << tablero[fila][columna];
			}
			else cout << setw(7);
			colorFondo(0);
		}
		cout << char(179) << endl;
		for (columna = 0; columna < DIM; columna++){
			cout << char(179);
			colorFondo(log2(tablero[fila][columna]));
			cout << right << setw(6) << "";
			colorFondo(0);
		}
		cout << char(179) << endl;
		if (fila < DIM - 1){
			cout << char(195);
			for (int j = 0; j < DIM; j++){
				for (int i = 0; i < 6; i++) cout << char(196);
				if (j >= DIM - 1) cout << char(180) << endl;
				else cout << char(197);
			}
		}
	}
}

void tableroInf(){
	cout << char(192);
	for (int i = 0; i < DIM; i++){
		for (int j = 0; j < 6; j++){
			cout << char(196);
		}
		if (i >= DIM - 1){
			cout << char(217) << endl;
		}
		else cout << char(193);
	}
}

void colorFondo(int color){
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

int log2(int num){
	int numero;
	numero = (log(num) / log(2));
	return numero;
}

void carga(tTablero tablero, int &puntos){
	ifstream archivo;
	string nombArch;
	int dimension;
	char c;
	bool ok = false;
	do{
		cout << "Desea cargar una partida? [S/N]: ";
		cin >> c;
		if (toupper(c) == 'S'){
			cout << "Introduce el nombre de la partida a cargar (SIN EXTENSION!): ";
			cin >> nombArch;
			nombArch = nombArch + ".txt";
			archivo.open(nombArch.c_str());
			if (archivo.is_open()){
				archivo >> dimension;
				if (DIM == dimension){
					for (int i = 0; i < dimension; i++){
						for (int j = 0; j < dimension; j++){
							archivo >> tablero[i][j];
						}
					}
					archivo >> puntos;
					ok = true;
				}
				else{
					cout << "La dimension del archivo a cargar no es la correcta." << endl << endl;
				}
			}
			else{
				cout << "Error al intentar cargar el archivo solicitado. Vuelve a intentarlo." << endl << endl;
			}
		}
		else if (toupper(c) == 'N') {
			inicializarTablero(tablero);
			ok = true;
		}
	} while (!ok);
}

tAccion leeAccion(){
	tAccion accion = Nada;
	int key;
	do{
		cin.sync();
		key = _getch();
		if (key == 0xe0){
			key = _getch();
			if (key == 72) accion = Arriba;
			else if (key == 80) accion = Abajo;
			else if (key == 77) accion = Derecha;
			else if (key == 75) accion = Izquierda;
		}
		else if (key == 27) accion = Salir;
	} while (accion == Nada);
	return accion;
}

void mueveFichas(tTablero tablero, tAccion accion){
	switch (accion){
	case 0:
		moverArriba(tablero);
		break;
	case 1:
		moverAbajo(tablero);
		break;
	case 2:
		moverDerecha(tablero);
		break;
	case 3:
		moverIzquierda(tablero);
		break;
	}
}

void moverArriba(tTablero tablero){
	int aux, incF = 0; // aux guarda el valor de la casilla donde estamos, e incF es el incremento que le sumamos a la fila para que la ficha se mueva hasta donde pueda
	for (int cini = DIM - 1; cini >= 0; cini--){
		for (int fini = 1; fini < DIM; fini++){
			if (tablero[fini][cini] > 1){
				while ((tablero[fini + incF - 1][cini] < 2) && (fini + incF - 1 >= 0)){
					aux = tablero[fini + incF][cini];
					tablero[fini + incF][cini] = 1;
					tablero[fini + incF - 1][cini] = aux;
					incF--;
				}
				incF = 0;
			}
		}
	}
}

void moverAbajo(tTablero tablero){
	int aux, incF = 0;// aux guarda el valor de la casilla donde estamos, e incF es el incremento que le sumamos a la fila para que la ficha se mueva hasta donde pueda
	for (int cini = DIM - 1; cini >= 0; cini--){
		for (int fini = DIM - 2; fini >= 0; fini--){
			if ((tablero[fini][cini] > 1) && (fini < (DIM - 1))){
				while ((tablero[fini + incF + 1][cini] < 2) && (fini + incF + 1 < DIM)){
					aux = tablero[fini + incF][cini];
					tablero[fini + incF][cini] = 1;
					tablero[fini + incF + 1][cini] = aux;
					incF++;
				}
				incF = 0;
			}
		}
	}
}

void moverDerecha(tTablero tablero){
	int aux, incC = 0;// aux guarda el valor de la casilla donde estamos, e incF es el incremento que le sumamos a la columna para que la ficha se mueva hasta donde pueda
	for (int fini = 0; fini < DIM; fini++){
		for (int cini = DIM - 2; cini >= 0; cini--){
			if ((tablero[fini][cini] > 1) && (cini < (DIM - 1))){
				while ((tablero[fini][cini + incC + 1] < 2) && (incC + 1 + cini < DIM)){
					aux = tablero[fini][cini + incC];
					tablero[fini][cini + incC] = 1;
					tablero[fini][cini + incC + 1] = aux;
					incC++;
				}
				incC = 0;
			}
		}
	}
}

void moverIzquierda(tTablero tablero){
	int aux, incC = 0;// aux guarda el valor de la casilla donde estamos, e incF es el incremento que le sumamos a la columna para que la ficha se mueva hasta donde pueda
	for (int fini = 0; fini < DIM; fini++){
		for (int cini = 1; cini < DIM; cini++){
			if (tablero[fini][cini] > 1){
				while ((tablero[fini][cini + incC - 1] < 2) && (cini + incC - 1 >= 0)){
					aux = tablero[fini][cini + incC];
					tablero[fini][cini + incC] = 1;
					tablero[fini][cini + incC - 1] = aux;
					incC--;
				}
				incC = 0;
			}
		}
	}
}

int mayor(const tTablero tablero){
	int mayor = 1, actual; //actual es la ficha donde estamos
	for (int fila = 0; fila < DIM; fila++){
		for (int columna = 0; columna < DIM; columna++){
			actual = tablero[fila][columna];
			if (actual > mayor) mayor = actual;
		}
	}
	return mayor;
}

bool lleno(const tTablero tablero){
	bool lleno = true;
	int fila = 0, columna;
	do{
		columna = 0;
		do{
			if (tablero[fila][columna] <= 1) lleno = false;
			columna++;
		} while ((lleno) && (columna < DIM));
		fila++;
	} while ((lleno) && (fila < DIM));
	return lleno;
}


void combinaFichas(tTablero tablero, tAccion accion, int &puntos){
	switch (accion){
	case 0:
		juntarArriba(tablero, puntos);
		break;
	case 1:
		juntarAbajo(tablero, puntos);
		break;
	case 2:
		juntarDerecha(tablero, puntos);
		break;
	case 3:
		juntarIzquierda(tablero, puntos);
		break;
	}


}

void nuevaFicha(tTablero tablero){
	int dim = DIM*DIM - 1, pos, fila, columna, num; //dim es el numero de cada casilla del array
	bool rellena = true;
	do{
		pos = rand() % (dim + 1);
		fila = pos / DIM;
		columna = pos % DIM;
		if (tablero[fila][columna] <= 1){
			rellena = false;
			num = (rand() % 100) + 1; // valor aleatorio entre 0-99
			if (num > 95){
				tablero[fila][columna] = 4;
			}
			else tablero[fila][columna] = 2;
		}
	} while (rellena);

}

void juntarArriba(tTablero tablero, int &puntos){
	for (int columna = 0; columna < DIM; columna++){
		for (int fila = 0; fila < DIM - 1; fila++){
			if ((tablero[fila][columna] == tablero[fila + 1][columna])&&(tablero[fila][columna] != 1)){
				tablero[fila][columna] = tablero[fila][columna] + tablero[fila + 1][columna];
				tablero[fila+1][columna] = 1;
				puntos = puntos + tablero[fila][columna];
			}
		}
	}
}

void juntarAbajo(tTablero tablero, int &puntos){
	for (int columna = 0; columna < DIM; columna++){
		for (int fila = DIM - 1; fila > 0; fila--){
			if ((tablero[fila][columna] == tablero[fila - 1][columna]) && (tablero[fila][columna] != 1)){
				tablero[fila][columna] = tablero[fila][columna] + tablero[fila - 1][columna];
				tablero[fila - 1][columna] = 1;
				puntos = puntos + tablero[fila][columna];
			}
		}
	}
}

void juntarDerecha(tTablero tablero, int &puntos){
	for (int fila = 0; fila < DIM; fila++){
		for (int columna = DIM - 1; columna > 0; columna--){
			if ((tablero[fila][columna] == tablero[fila][columna - 1]) && (tablero[fila][columna] != 1)){
				tablero[fila][columna] = tablero[fila][columna] + tablero[fila][columna - 1];
				tablero[fila][columna - 1] = 1;
				puntos = puntos + tablero[fila][columna];
			}
		}
	}
}

void juntarIzquierda(tTablero tablero, int &puntos){
	for (int fila = 0; fila < DIM; fila++){
		for (int columna = 0; columna < DIM - 1; columna++) {
			if ((tablero[fila][columna] == tablero[fila][columna + 1]) && (tablero[fila][columna] != 1)){
				tablero[fila][columna] = tablero[fila][columna] + tablero[fila][columna + 1];
				tablero[fila][columna + 1] = 1;
				puntos = puntos + tablero[fila][columna];
			}
		}
	}
}

void guarda(const tTablero tablero, int puntos){
	ofstream archivo;
	string nombArch;
	cout << "Introduzca el nombre de la partida (SIN EXTENSION!): ";
	cin >> nombArch;
	nombArch = nombArch + ".txt";
	archivo.open(nombArch.c_str());
	archivo << DIM << endl;
	for (int fila = 0; fila < DIM; fila++){
		for (int columna = 0; columna < DIM; columna++){
			archivo << tablero[fila][columna] << endl;
		}
	}
	archivo << puntos << endl;
	archivo.close();
}
