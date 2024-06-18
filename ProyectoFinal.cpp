#include <iostream>
#include <vector>
#include <fstream>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include <sstream>
#define ENTRADA "bajo.csv"
#define SEPARADOR cout << "\t+----------------------------------------------------+" << endl;
#define SEPARADORALT cout << "\t+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+" << endl;
#define CABEZERA cout << "\t|  Iteracion  |  Direccion  |  Bloque  |  Resultado  |" << endl \
					  << "\t+----------------------------------------------------+" << endl;
using namespace std;

void correspondenciaDirecta(int tamanoCache, char *direccionMemoria, struct stat sb);
void imprimirCache(int cache[], int tamanoCache);
void aumentarTiempoDesdeQueSeUso(int *tiempoDesdeQueSeUso, int tamanoCache);
void correspondenciaAsociativoPorConjuntos(int tamanoCache,int numeroDeConjuntos, char *direccionMemoria, struct stat sb);
void correspondenciaCompletamenteAsociativa(int tamanoCache, char *direccionMemoria, struct stat sb);
void cargarDirecciones(vector<int> &listaNumeros);
void ShowMenu(int *iniciar, int *numeroDeBloques, int *numeroDeConjuntos);
void printIteracion(int i, unsigned long int direccion, int numBloque, bool fallo);
void printMax(string data, int max);
unsigned long int DirHexToDec(string direccionHex);
void printResultados(unsigned long int fallos, unsigned long int aciertos, unsigned long int lon);

int main()
{
	int fd = open(ENTRADA, O_RDONLY, S_IRUSR | S_IWUSR);
	struct stat sb;

	if (fstat(fd, &sb) == -1)
	{
		perror("Error al obtener el tamaño del archivo");
		exit(EXIT_FAILURE);
	}
	char *direccionMemoria = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (direccionMemoria == MAP_FAILED)
	{
		perror("Error al mapear el archivo");
		exit(EXIT_FAILURE);
	}

	vector<int> entrada;
	cargarDirecciones(entrada);

	int iniciar = 0;
	int numeroDeBloques = 0;
	int numeroDeConjuntos = 0;

	ShowMenu(&iniciar, &numeroDeBloques, &numeroDeConjuntos);

	switch (iniciar)
	{
	case 1:
		correspondenciaDirecta(numeroDeBloques, direccionMemoria, sb);
		break;
	case 2:
		correspondenciaAsociativoPorConjuntos(numeroDeBloques, numeroDeConjuntos, direccionMemoria, sb);
		break;
	case 3:
		correspondenciaCompletamenteAsociativa(numeroDeBloques, direccionMemoria, sb);
		break;
	case 4:
		correspondenciaDirecta(numeroDeBloques, direccionMemoria, sb);
		correspondenciaAsociativoPorConjuntos(numeroDeBloques, numeroDeConjuntos, direccionMemoria, sb);
		correspondenciaCompletamenteAsociativa(numeroDeBloques, direccionMemoria, sb);
		break;
	}

	return 1;
}

void ShowMenu(int *inicar, int *numeroDeBloques, int *numeroDeConjuntos)
{
	cout << endl
		 << "\t+------------------ MENU PARTE II -------------------+" << endl;
	cout << "\t| Desarrolladores: Josue Carrion 30020470            |" << endl;
	cout << "\t|                  Gabriel Pinto 26364594            |" << endl;
	cout << "\t| Entrada: " << ENTRADA << "  |" << endl;
	cout << "\t|          4857379 registros, 310.085 KB             |" << endl;
	SEPARADOR
	cout << "\t| Seleccione el tipo de correspondencia              |" << endl;
	cout << "\t| [1] Correspondencia Directa                        |" << endl;
	cout << "\t| [2] Correspondencia Asociativa Con Conjuntos       |" << endl;
	cout << "\t| [3] Correspondencia Completamente Asociativa       |" << endl;
	cout << "\t| [4] Todas                                          |" << endl;
	SEPARADOR
	cout << "\t  Correspondencia tipo: ";
	do
	{
		cin >> *inicar;
		if (*inicar < 1 || *inicar > 4)
			cout << "\t  Opcion invalida, intente de nuevo: ";

	} while (*inicar < 0 || *inicar > 4);

	cout << "\t  Ingrese el numero de bloques: ";
	do
	{
		cin >> *numeroDeBloques;
		if (*numeroDeBloques < 1)
			cout << "\t  Opcion invalida, intente de nuevo: ";

	} while (*numeroDeBloques < 1);

	if (*inicar == 2 || *inicar == 4)
	{
		cout << "\t  Ingrese el numero de conjuntos: ";
		do
		{
			cin >> *numeroDeConjuntos;
			if (*numeroDeConjuntos < 1 || *numeroDeConjuntos > *numeroDeBloques)
				cout << "\t  Opcion invalida, intente de nuevo: ";

		} while (*numeroDeConjuntos < 1 || *numeroDeConjuntos > *numeroDeBloques);
	}
	SEPARADOR
	cout << endl;
}

void cargarDirecciones(vector<int> &listaNumeros)
{
	ifstream archivoEntrada("input.txt");
	if (archivoEntrada.is_open())
	{
		int numero;
		while (archivoEntrada >> numero)
		{
			listaNumeros.push_back(numero);
		}
		archivoEntrada.close();
	}
	else
	{
		cerr << "Error al abrir el archivo." << endl;
	}
}

void correspondenciaDirecta(int tamanoCache, char *direccionMemoria, struct stat sb)
{
	// int lon = direcciones.size();
	vector<unsigned long int> cache(tamanoCache, -1);
	unsigned long int aciertos = 0;
	unsigned long int fallos = 0;

	int i = 0;
	bool fallo = false;
	SEPARADORALT
	cout << "\t| Correspondencia Directa:                           |" << endl;
	SEPARADORALT
	CABEZERA
	unsigned long int total = 0;

	string s = "";
	for (i = 0; i < sb.st_size; i++)
	{
		if (*(direccionMemoria + i) == ',')
		{
			int numBloque = DirHexToDec(s) % cache.size();

			if (cache[numBloque] == DirHexToDec(s))
			{
				fallo = false;
				aciertos++;
			}
			else
			{
				fallos++;
				fallo = true;
				cache[numBloque] = DirHexToDec(s);
			}
			total++;
			printIteracion(total, DirHexToDec(s), numBloque, fallo);
			s = "";
		}
		else if (*(direccionMemoria + i) == '\n')
		{
			s = "";
		}
		else
		{
			s += *(direccionMemoria + i);
		}
	}

	printResultados(fallos, aciertos, total);
	// 	if (*(direccionMemoria + i) == ',')
	// 	{
	// 		cout << s << ", ";
	// 		total++;
	// 		s = "";
	// 	}
	// 	else if (*(direccionMemoria + i) == '\n')
	// 	{
	// 		cout << endl;
	// 		s = "";
	// 	}
	// 	else
	// 	{
	// 		s += *(direccionMemoria + i);
	// 	}
	// }

	// for (i = 0; i < lon; i++)
	// {
	// 	int numBloque = direcciones[i] % cache.size();
	// 	if (cache[numBloque] == direcciones[i])
	// 	{
	// 		fallo = false;
	// 		aciertos++;
	// 	}
	// 	else
	// 	{
	// 		fallos++;
	// 		fallo = true;
	// 		cache[numBloque] = direcciones[i];
	// 	}

	// 	printIteracion(i, direcciones[i], numBloque, fallo);
	// }
	// printResultados(fallos, aciertos, total);
}

void printResultados(unsigned long int fallos, unsigned long int aciertos, unsigned long int lon)
{
	SEPARADORALT
	cout << "\t| fallos: " << fallos << endl;
	cout << "\t| aciertos: " << aciertos << endl;
	cout << "\t| porcentaje de aciertos: " << (float)((float)aciertos / (float)lon) * 100 << "%" << endl;
	SEPARADORALT
	cout << endl;
}

void printMax(string data, int max)
{
	int x = 0;

	int len = data.length();
	for (x = 0; x < (max - len) / 2; x++)
	{
		cout << " ";
	}
	cout << data;
	for (x = 0; x < (max - len) / 2 + (max - len) % 2; x++)
	{
		cout << " ";
	}
}

void printIteracion(int i, unsigned long int direccion, int numBloque, bool fallo)
{
	int x = 0;
	cout << "\t|";
	printMax(to_string(i), 13);
	cout << "|";
	printMax(to_string(direccion), 13);
	cout << "|";
	printMax(to_string(numBloque), 10);
	cout << "|";
	if (fallo)
	{
		printMax("Fallo", 13);
	}
	else
	{
		printMax("Acierto", 13);
	}

	cout << "|" << endl;
}

void correspondenciaCompletamenteAsociativa(int tamanoCache, char *direccionMemoria, struct stat sb)
{
	vector<unsigned long int> cache(tamanoCache, -1);
	unsigned long int aciertos = 0;
	unsigned long int fallos = 0;
	unsigned long int total = 0;

	int i = 0;
	int numBloque = -1;
	bool fallo = false;
	SEPARADORALT
	cout << "\t|  Correspondencia Completamente Asociativa:         |" << endl;
	SEPARADORALT
	CABEZERA

	string s = "";
	for (i = 0; i < sb.st_size; i++)
	{
		if (*(direccionMemoria + i) == ',')
		{
			int direccionBloque = *(direccionMemoria + i);
		numBloque = -1;
		fallo = true;
		int x = 0;
		while (x < cache.size() && fallo)
		{
			if (cache[x] == direccionBloque)
			{
				aciertos++;
				fallo = false;
				numBloque = x;
			}
			x++;
		}

		bool espacioDisponible = false;
		if (fallo)
		{
			fallos++;

			x = 0;
			while (x < cache.size() && !espacioDisponible)
			{
				if (cache[x] == -1)
				{
					numBloque = x;
					cache[x] = direccionBloque;
					espacioDisponible = true;
				}
				x++;
			}

			if (!espacioDisponible)
			{
				numBloque = rand() % cache.size();
				cache[numBloque] = direccionBloque;
			}
		}
			
			total++;
			printIteracion(total, DirHexToDec(s), numBloque, fallo);
			s = "";
		}
		else if (*(direccionMemoria + i) == '\n')
		{
			s = "";
		}
		else
		{
			s += *(direccionMemoria + i);
		}
		

		printIteracion(i, *(direccionMemoria + i), numBloque, fallo);
	}
	printResultados(fallos, aciertos, total);
}

void correspondenciaAsociativoPorConjuntos( int tamanoCache, int numConjuntos, char *direccionMemoria, struct stat sb)
{
	int cache[numConjuntos][tamanoCache];
	int tiempoDeUso[numConjuntos][tamanoCache];

	int i, j = 0;
	for (i = 0; i < numConjuntos; i++)
	{
		for (j = 0; j < tamanoCache; j++)
		{
			cache[i][j] = -1;
			tiempoDeUso[i][j] = -1;
		}
	}

	unsigned long int aciertos = 0;
	unsigned long int fallos = 0;

	bool fallo = false;
	SEPARADORALT
	cout << "\t|  Correspondencia Asociativa Por Conjuntos:         | " << endl;
	SEPARADORALT
	CABEZERA
	unsigned long int total = 0;
	string s = "";
		for (i = 0; i < sb.st_size; i++)
	{
		if (*(direccionMemoria + i) == ',')
		{
			total++;
		int numConjunto = *(direccionMemoria + i) % numConjuntos;
		int numBloque = -1;
		int direccionBloque = *(direccionMemoria + i);
		fallo = true;
		int x = 0;
		aumentarTiempoDesdeQueSeUso(tiempoDeUso[numConjunto], tamanoCache);
		while (x < tamanoCache && fallo)
		{
			if (cache[numConjunto][x] == direccionBloque)
			{
				aciertos++;
				fallo = false;
				numBloque = x;
			}
			x++;
		}

		bool espacioDisponible = false;
		if (fallo)
		{
			fallos++;

			x = 0;
			while (x < numConjuntos && !espacioDisponible)
			{
				if (cache[numConjunto][x] == -1)
				{
					numBloque = x;
					cache[numConjunto][x] = direccionBloque;
					espacioDisponible = true;
				}
				x++;
			}

			if (!espacioDisponible)
			{
				x = 0;
				int max = -1;
				for (x = 0; x < tamanoCache; x++)
				{
					if (tiempoDeUso[numConjunto][x] > max)
					{
						max = tiempoDeUso[numConjunto][x];
						numBloque = x;
					}
				}
				cache[numConjunto][numBloque] = direccionBloque;
			}
		}
		tiempoDeUso[numConjunto][numBloque] = 0;
		printIteracion(i, *(direccionMemoria + i), numBloque, fallo);
		}
		else if (*(direccionMemoria + i) == '\n')
		{
			s = "";
		}
		else
		{
			s += *(direccionMemoria + i);
		}

		
	}
	printResultados(fallos, aciertos, total);
}

void aumentarTiempoDesdeQueSeUso(int *tiempoDesdeQueSeUso, int tamanoCache)
{
	for (int i = 0; i < tamanoCache; i++)
	{
		if (tiempoDesdeQueSeUso[i] != -1)
			tiempoDesdeQueSeUso[i]++;
	}
}

void imprimirCache(int cache[], int tamanoCache)
{
	for (int i = 0; i < tamanoCache; i++)
	{
		cout << cache[i] << " ";
	}
	cout << endl;
}

unsigned long int DirHexToDec(string direccionHex)
{

	if (direccionHex.substr(0, 2) == "0x")
	{
		direccionHex = direccionHex.substr(2);
	}

	unsigned long int decimal;
	istringstream(direccionHex) >> hex >> decimal;

	return decimal;
}