#include <iostream>
#include <vector>
#include <fstream>
#define SEPARADOR cout << "\t+----------------------------------------------------+" << endl;
#define SEPARADORALT cout << "\t+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+" << endl;
#define CABEZERA cout << "\t|  Iteracion  |  Direccion  |  Bloque  |  Resultado  |" << endl \
					  << "\t+----------------------------------------------------+" << endl;
using namespace std;

void correspondenciaDirecta(vector<int> direcciones, int tamanoCache);
void imprimirCache(int cache[], int tamanoCache);
void aumentarTiempoDesdeQueSeUso(int *tiempoDesdeQueSeUso, int tamanoCache);
void correspondenciaAsociativoPorConjuntos(vector<int> direcciones, int tamanoCache, int numConjuntos);
void correspondenciaCompletamenteAsociativa(vector<int> direcciones, int tamanoCache);
void cargarDirecciones(vector<int> &listaNumeros);
long int obtenerDireccionDecimal(void *direccionMemoria);
void ShowMenu(int *iniciar, int *numeroDeBloques, int *numeroDeConjuntos);
void printIteracion(int i, int direccion, int numBloque, bool fallo);

void printResultados(double fallos, double aciertos, int lon);

int main()
{
	vector<int> entrada;
	cargarDirecciones(entrada);

	int iniciar = 0;
	int numeroDeBloques = 0;
	int numeroDeConjuntos = 0;

	ShowMenu(&iniciar, &numeroDeBloques, &numeroDeConjuntos);

	switch (iniciar)
	{
	case 1:
		correspondenciaDirecta(entrada, numeroDeBloques);
		break;
	case 2:
		correspondenciaAsociativoPorConjuntos(entrada, numeroDeBloques, numeroDeConjuntos);
		break;
	case 3:
		correspondenciaCompletamenteAsociativa(entrada, numeroDeBloques);
		break;
	case 4:
		correspondenciaDirecta(entrada, numeroDeBloques);
		correspondenciaAsociativoPorConjuntos(entrada, numeroDeBloques, numeroDeConjuntos);
		correspondenciaCompletamenteAsociativa(entrada, numeroDeBloques);
		break;
	}

	return 1;
}

void ShowMenu(int *inicar, int *numeroDeBloques, int *numeroDeConjuntos)
{
	cout << endl
		 << "\t+------------------- MENU PARTE I -------------------+" << endl;
	cout << "\t| Desarrolladores: Josue Carrion 30020470            |" << endl;
	cout << "\t|                  Gabriel Pinto 26364594            |" << endl;
	cout << "\t| Entrada: input.txt         5 registros, 1 KB       |" << endl;
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

void correspondenciaDirecta(vector<int> direcciones, int tamanoCache)
{
	int lon = direcciones.size();
	vector<int> cache(tamanoCache, -1);
	double aciertos = 0;
	double fallos = 0;

	int i = 0;
	bool fallo = false;
	SEPARADORALT
	cout << "\t| Correspondencia Directa:                           |" << endl;
	SEPARADORALT
	CABEZERA

	for (i = 0; i < lon; i++)
	{
		int numBloque = direcciones[i] % cache.size();
		if (cache[numBloque] == direcciones[i])
		{
			fallo = false;
			aciertos++;
		}
		else
		{
			fallos++;
			fallo = true;
			cache[numBloque] = direcciones[i];
		}

		printIteracion(i, direcciones[i], numBloque, fallo);
	}
	printResultados(fallos, aciertos, lon);
}

void printResultados(double fallos, double aciertos, int lon)
{
	SEPARADORALT
	cout << "\t| fallos: " << fallos << endl;
	cout << "\t| aciertos: " << aciertos << endl;
	cout << "\t| porcentaje de aciertos: " << (aciertos / lon) * 100 << "%" << endl;
	SEPARADORALT
	cout << endl;
}

void printIteracion(int i, int direccion, int numBloque, bool fallo)
{
	int x = 0;

	cout << "\t|      " << i << "      ";

	for (x = 0; x < (i / 10); x++)
	{
		cout << " ";
	}
	cout << "|";
	cout << "      " << direccion << "      ";

	for (x = 0; x < (i / 10); x++)
	{
		cout << " ";
	}

	cout << "|";
	cout << "     " << numBloque << "    ";

	for (x = 0; x < (i / 10); x++)
	{
		cout << " ";
	}

	cout << "|";

	cout << "   " << (fallo ? " Fallo  " : "Acierto ") << "  ";

	for (x = 0; x < (i / 10); x++)
	{
		cout << " ";
	}
	cout << "|" << endl;
}

void correspondenciaCompletamenteAsociativa(vector<int> direcciones, int tamanoCache)
{
	int lon = direcciones.size();
	vector<int> cache(tamanoCache, -1);
	double aciertos = 0;
	double fallos = 0;

	int i = 0;
	int numBloque = -1;
	bool fallo = false;
	SEPARADORALT
	cout << "\t|  Correspondencia Completamente Asociativa:         |" << endl;
	SEPARADORALT
	CABEZERA

	for (i = 0; i < lon; i++)
	{
		int direccionBloque = direcciones[i];
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

		printIteracion(i, direcciones[i], numBloque, fallo);
	}
	printResultados(fallos, aciertos, lon);
}

void correspondenciaAsociativoPorConjuntos(vector<int> direcciones, int tamanoCache, int numConjuntos)
{
	int lon = direcciones.size();
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

	double aciertos = 0;
	double fallos = 0;

	bool fallo = false;
	SEPARADORALT
	cout << "\t|  Correspondencia Asociativa Por Conjuntos:         | " << endl;
	SEPARADORALT
	CABEZERA
	for (j = 0; j < lon; j++)
	{

		int numConjunto = direcciones[j] % numConjuntos;
		int numBloque = -1;
		int direccionBloque = direcciones[j];
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
		printIteracion(j, direcciones[j], numBloque, fallo);
	}
	printResultados(fallos, aciertos, lon);
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

long int obtenerDireccionDecimal(void *direccionMemoria)
{
	long int p = (long int)direccionMemoria;
	return p;
}
