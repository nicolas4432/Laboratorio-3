#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>



/*------------------Estructuras----------------------*/
typedef struct adyacente {
	int peso;							//Peso de la arista
	int vertice;						//Numero del vertice adyacente
	struct adyacente* siguiente;		//Puntero al siguiente vertice adyacente
	struct vertice* verticeO;			//Puntero al vertice inicial
} Nodos;

typedef struct vertice {
	int vertice;				//Numero del vertice 
	struct adyacente* adyacente;	//Puntero a los nodos adyacentes
	struct vertice* sigVertice;	//Puntero al siguiente vertice
	struct vertice* cabecera;	//Puntero al primer vertice
} Grafo;

typedef struct lista {
	int vertice;				//Numero del vertice 
	struct lista* siguiente; //Siguiente vertice recorrido
} Pila;

typedef struct listaRecorridos {
	struct lista* recorrido;	//Se lista con recorrido
	struct listaRecorridos* siguiente; //Siguiente resultado
} Resultados;


/*------------------Declaracion de funciones----------------------*/
Pila* pop(Pila* lista);
Pila* push_pila(Pila* lista, int vertice);
Resultados* push_resultado(Resultados* lista, Pila* pila);
Nodos* agregarAdyacentes(Nodos* adyacentesInicial, int ultimaPos, int verticeNumero, int cantVertices, char linea[1000]);
Nodos* crearAdyacentes(Grafo* grafoInicial, char linea[1000], int cantVertices);
int obtenerNumero(int numeroPalabra, char linea[1000]);
Grafo* agregarVertice(Grafo* grafoInicial, int numeroGrafo, char linea[1000], int cantVertices);
Grafo* leerGrafo(char* nombreArchivo, int* nVertices, int* vSalida, int* vLlegada);
Resultados* recoridoProfundidad(Grafo* grafo, int verFinal, Pila* pilaActual, Pila* pilaRecorrido, int* verticesMarcados, Resultados* listaResultados);
int flujoTotal(Resultados* listaRecorridos, Grafo* grafo);
Pila* busquedaVerticeCritico(Grafo* grafo, int numeroVertices);
void escribirDatos(Pila* criticos, Resultados* caminos, Grafo* grafo);


/*------------------Funciones----------------------*/
Pila* pop(Pila* lista) {
	Pila* nodo = lista;
	int vertice = lista->vertice;
	lista = lista->siguiente;
	free(nodo);
	return lista;
}

Pila* push_pila(Pila* lista, int vertice) {
	Pila* nodo = (Pila*)malloc(sizeof(Pila));
	nodo->vertice = vertice;
	nodo->siguiente = lista;
	return nodo;
}

Resultados* push_resultado(Resultados* lista, Pila* pila) {
	Resultados* nodo = (Resultados*)malloc(sizeof(Resultados));
	nodo->recorrido = pila;
	nodo->siguiente = lista;
	return nodo;
}

Nodos* agregarAdyacentes(Nodos* adyacentesInicial, int ultimaPos, int verticeNumero, int cantVertices, char linea[1000])
{
	Nodos* final = adyacentesInicial;					//Asigno puntero para posicion final
	Nodos* adyacente = final;							//Guardo la lista inicial
	Nodos* nuevo = NULL;								//Creo el nuevo nodo
	int saltoE = verticeNumero;							//Iterador de espacios
	int aux = ultimaPos;								//Iterador auxiliar
	int numeroObtenido;

	while (saltoE != cantVertices)				//Se agregan adyacentes hasta que me encuentre en el ultimo vertice
	{
		while (final->siguiente != NULL)					//Posiciono puntero al final de la lista
		{
			final = final->siguiente;
		}

		while (1)											//Se busca el siguiente nodo adyacente
		{
			if (cantVertices * 2 - 1 == aux) break;
			else if (linea[aux] != '0' && linea[aux] != ' ') break;

			if (linea[aux] == ' ') saltoE++;
			aux++;
		}

		if (linea[aux] != '\n')								//Si el elemento no es 0 creo inicio la lista de vertices adyacentes
		{
			nuevo = (Nodos*)malloc(sizeof(Nodos));
			numeroObtenido = obtenerNumero(saltoE, linea);
			nuevo->peso = numeroObtenido;
			nuevo->vertice = saltoE;
			nuevo->verticeO = final->verticeO;
			nuevo->siguiente = NULL;
			final->siguiente = nuevo;
			aux++;
		}
	}

	return adyacente;
}

Nodos* crearAdyacentes(Grafo* grafoInicial, char linea[1000], int cantVertices)
{
	Nodos* adyacentes = NULL;							//Estructura donde se guardara los vertices adyacentes

	int saltoE = 1;										//Iterador de espacios
	int aux = 0;										//Iterador auxiliar
	int numeroObtenido;									

	while (1)											//Se busca el primer nodo adyacente
	{
		if (cantVertices * 2 - 1 == aux) break;
		else if (linea[aux] != '0' && linea[aux] != ' ') break;

		if (linea[aux] == ' ') saltoE++;
		aux++;
	}

	if (linea[aux] != '0')								//Si el elemento no es 0 creo inicio la lista de vertices adyacentes
	{
		adyacentes = (Nodos*)malloc(sizeof(Nodos));
		numeroObtenido = obtenerNumero(saltoE, linea);
		adyacentes->peso = numeroObtenido;
		adyacentes->vertice = saltoE;
		adyacentes->verticeO = grafoInicial;
		adyacentes->siguiente = NULL;
		adyacentes = agregarAdyacentes(adyacentes, aux + 1, saltoE, cantVertices, linea);
	}

	return adyacentes;
}

int obtenerNumero(int numeroPalabra, char linea[1000])
{
	int saltoE = 1;								//Iterador de espacios
	int iterador = 0;							//Iterador de posicion
	int aux = 0;								//Iterador auxiliar
	int largo = strlen(linea) - 1;					//Largo de la linea leida
	char numeroString[100];						//Arreglo que contendra el numero buscado
	int numeroObtenido;

	while (saltoE != numeroPalabra)							//Posiciono iterador en el numero que quiero leer
	{
		if (iterador == largo) break;
		else if (linea[iterador] == ' ')
		{
			saltoE++;
			iterador++;
		}
		else {
			iterador++;
		}
	}

	while (linea[iterador] != ' ')							//Copio el numero que quiero transformar
	{
		if (iterador == largo) break;
		numeroString[aux] = linea[iterador];
		aux++;
		iterador++;
	}

	numeroObtenido = atoi(numeroString);

	return numeroObtenido;								//Devuelvo el numero como entero
}

Grafo* agregarVertice(Grafo* grafoInicial, int numeroGrafo, char linea[1000], int cantVertices)
{
	Grafo* final = grafoInicial;					//Asigno puntero para posicion final
	Grafo* grafo = final;							//Guardo la lista inicial
	Grafo* nuevo = NULL;							//Creo el nuevo vertice

	while (final->sigVertice != NULL)				//Posiciono puntero al final de la lista
	{
		final = final->sigVertice;
	}

	nuevo = (Grafo*)malloc(sizeof(Grafo));			//Agrego nuevo Vertice
	nuevo->vertice = numeroGrafo;
	nuevo->adyacente = crearAdyacentes(nuevo, linea, cantVertices);
	nuevo->cabecera = final->cabecera;
	nuevo->sigVertice = NULL;
	final->sigVertice = nuevo;

	return grafo;
}

Grafo* leerGrafo(char* nombreArchivo, int* nVertices, int* vSalida, int* vLlegada)
{
	char buffer_linea[1000];						//Buffer de lectura de linea
	int	posCaracter = 0;							//Iterador para la posición del caracter
	int aux = 0;									//Iterador auxiliar
	Grafo* grafo = NULL;							//Estructura donde se guardara el grafo
	FILE* datosGrafo = fopen("entrada.in", "r");	//Abrimos el archivo con un Puntero FILE*

	grafo = (Grafo*)malloc(sizeof(Grafo));
	grafo->cabecera = grafo;						//El nodo cabecera es el primero que se crea

	if (datosGrafo == NULL) {						//El archivo se pudo leer?
		grafo->adyacente = NULL;
		grafo->sigVertice = NULL;
		grafo->vertice = -1;						//No pueden hacer vertices negativos
		return grafo;
	}

	fgets(buffer_linea, 1000, datosGrafo);			//Se le el numero de vertices, salida, llegada

	*nVertices = obtenerNumero(1, buffer_linea);	//Se guarda el numero de vertices
	*vSalida = obtenerNumero(2, buffer_linea);		//Se guarda el vertice de salida	
	*vLlegada = obtenerNumero(3, buffer_linea);		//Se guarda el vertice de llegada

	fgets(buffer_linea, 1000, datosGrafo);			//Se crea la lista con el primer vertice
	grafo->vertice = 1;
	grafo->sigVertice = NULL;
	grafo->adyacente = crearAdyacentes(grafo, buffer_linea, *nVertices);

	while (1) {
		if (fgets(buffer_linea, 1000, datosGrafo) == NULL) break;									//Hasta que se pueda leer
		else if (strlen(buffer_linea) == 1) break;  //En ubunto es 2
		else
		{
			for (int i = 1; i < *nVertices; i++)
			{
				grafo = agregarVertice(grafo, i + 1, buffer_linea, *nVertices);
				fgets(buffer_linea, 1000, datosGrafo);
			}
		}
	}

	fclose(datosGrafo);
	return grafo;
}

Resultados* recoridoProfundidad(Grafo* grafo, int verFinal, Pila* pilaActual, Pila* pilaRecorrido, int* verticesMarcados, Resultados* listaResultados) 
{
	int verAct;													//Vertice actual
	Nodos* puntAdyacentes = (Nodos*)malloc(sizeof(Nodos));								//Puntero de la lista de vertices adyacentes
	Grafo* puntVertices = grafo;									//Puntero de la lista de vertices
	Pila* pilaAux = (Pila*)malloc(sizeof(Pila));

	while (pilaActual != NULL)										//Hasta que la pila este vacia
	{
		verAct = pilaActual->vertice;							//Vertice actual

		if (verAct == verFinal) {						//Si el vertice actual es el final
			pilaRecorrido = push_pila(pilaRecorrido, verAct);	//Se agrega vertice y termina de crecorrer nodos
			listaResultados = push_resultado(listaResultados, pilaRecorrido);
			free(pilaAux);
			return listaResultados;
		}
		
		pilaActual = pop(pilaActual);								//Se saca el ultimo verte

		if (verticesMarcados[verAct - 1] == 0) {					//Si el vertice no a sido recorrido 
			verticesMarcados[verAct - 1] = 1;						//Se marca el vertice recorrido
			pilaRecorrido = push_pila(pilaRecorrido, verAct);		//Se agrega el vertice recorrido a la lista de recorridos

			for (int i = 1; i < verAct; i++)						//Posiciono el puntero en el vertice actual
			{
				puntVertices = puntVertices->sigVertice;
			}

			puntAdyacentes = puntVertices->adyacente;					//Posiciono puntero en la lista de adyacentes

			while (puntAdyacentes != NULL)								//Agrego todos los vertices adyacentes a la pila
			{
				pilaActual = push_pila(pilaActual, puntAdyacentes->vertice);	//Agrego el vertice actual recorido
				pilaAux->vertice = pilaActual->vertice;							//Se copia en un auxiliar
				pilaAux->siguiente = NULL;
				
				puntAdyacentes = puntAdyacentes->siguiente;						//Se pasa al siguiente adyacente
				listaResultados = recoridoProfundidad(grafo, verFinal, pilaAux, pilaRecorrido, verticesMarcados, listaResultados); //Se buscan los resultados con el recorrido actual
				pilaAux = (Pila*)malloc(sizeof(Pila));
				pilaActual = pop(pilaActual);
			}

			puntVertices = puntVertices->cabecera;					//Posiciono el puntero de vertices en el incio de la lista
		}
	}

	free(pilaAux);
	return listaResultados;
}

int flujoTotal(Resultados* listaRecorridos, Grafo* grafo) {
	
	Grafo* puntVertices = grafo;									//Puntero de la lista de vertices
	Pila* camino = listaRecorridos->recorrido;
	Nodos* punteroN = (Nodos*)malloc(sizeof(Nodos));		//Puntero para la lista de adyacente
	int inicio;						//Vertice inicio
	int llegada;					//Vertice llegada
	int total = 0;		//Flujo total
	
	while (camino->siguiente != NULL)	//Hasta que mi camino siguiente sea nulo
	{
		llegada = camino->vertice;					//Vertice de llegada
		inicio = camino->siguiente->vertice;		//Vertice de salida

		for (int i = 1; i < inicio; i++)						//Posiciono el puntero en el vertice inicio para conocer el flujo al vertice de llegada
		{
			puntVertices = puntVertices->sigVertice;
		}

		punteroN = puntVertices->adyacente;						//Apunto la lista de adyacentes

		while (punteroN->vertice != llegada)					//Busco el vertice adyacente de llegada y su peso
		{
			punteroN = punteroN->siguiente;
		}

		if (total == 0) total = punteroN->peso;			//Si total no a sido iniciado
		if (punteroN->peso < total) total = punteroN->peso;	//Si aparece un peso menor se cambia y conserva el menor
		camino = camino->siguiente;
		puntVertices = puntVertices->cabecera;
	}

	return total;
}

Pila* busquedaVerticeCritico(Grafo* grafo, int numeroVertices)
{
	Grafo* puntVertices = grafo;								//Puntero al grafo
	Nodos* puntAdyacentes = (Nodos*)malloc(sizeof(Nodos));
	Pila* pilaAux = NULL;											//Creo una pila
	Pila* criticos = NULL;									//Lista para nodos criticos
	
	int* marcados = (int*)malloc(numeroVertices * sizeof(int));	//Creo un arreglo de igual tamaño que la cantidad de vertices
	int verAct;		//Vertice actual
	int verInicial;

	for (int i = 0; i < numeroVertices; i++)
	{
		for (int j = 0; j < numeroVertices; j++)					//Arreglo de "booleanos" para vertices recorridos
		{
			marcados[j] = 0;
		}
		
		marcados[i] = 1;									//Marco el vertice a verificar si es critico
		if (i == numeroVertices - 1) verInicial = 1;					//Si estoy en el ultimo vertice mi vertice actual es 1
		else verInicial = i + 2;					//Vertice actual es el siguiente al vertice marcado como critico

		pilaAux = push_pila(pilaAux, verInicial);

		while (pilaAux != NULL)										//Hasta que la pila este vacia
		{
			verAct = pilaAux->vertice;							//Vertice actual
			pilaAux = pop(pilaAux);								//Se saca el ultimo verte

			if (marcados[verAct - 1] == 0) {					//Si el vertice no a sido recorrido 
				marcados[verAct - 1] = 1;						//Se marca el vertice recorrido

				for (int i = 1; i < verAct; i++)						//Posiciono el puntero en el vertice actual
				{
					puntVertices = puntVertices->sigVertice;
				}

				puntAdyacentes = puntVertices->adyacente;					//Posiciono puntero en la lista de adyacentes

				while (puntAdyacentes != NULL)								//Agrego todos los vertices adyacentes a la pila
				{
					pilaAux = push_pila(pilaAux, puntAdyacentes->vertice);	//Agrego el vertice actual recorido
					puntAdyacentes = puntAdyacentes->siguiente;
				}

				puntVertices = puntVertices->cabecera;					//Posiciono el puntero de vertices en el incio de la lista
				puntAdyacentes = puntVertices->adyacente;				//Puntero a los adyacentes del primer nodo

				while (puntVertices != NULL)				//Mientras lista vertices distinto de nulo
				{
					while (puntAdyacentes != NULL)				//Hata recorrer todos los adyacentes
					{
						if(puntAdyacentes->vertice == verAct) pilaAux = push_pila(pilaAux, puntVertices->vertice);			//Si el vertice acctual es adyacente a algun nodo se agrega a la pila ese nodo
						puntAdyacentes = puntAdyacentes->siguiente;
					}
					puntVertices = puntVertices->sigVertice;		//Paso al siguiente vertice
					if (puntVertices != NULL) puntAdyacentes = puntVertices->adyacente;
				}

				puntVertices = grafo;					//Posiciono el puntero de vertices en el incio de la lista
			}
		}

		for (int k = 0; k < numeroVertices; k++)					//Reviso si algun vertice no fue recorrido
		{
			if (marcados[k] == 0)							//Si un vertice no fue recorrido es por que el nodo marcado al incio es nodo critico
			{
				criticos = push_pila(criticos, i + 1);
				break;
			}
		}

	}

	return criticos;
}

void escribirDatos(Pila* criticos, Resultados* caminos, Grafo* grafo) 
{
	Pila* auxCriticos = criticos;
	Resultados* auxResultados = caminos;
	Pila* caminoRes = (Pila*)malloc(sizeof(Pila));
	Pila* axuliar = NULL;
	int total;
	int suma = 0;

	FILE* archivo = fopen("salida.out", "w");;

	while (auxResultados != NULL) {
		caminoRes = auxResultados->recorrido;
		axuliar = push_pila(axuliar, caminoRes->vertice);
		
		while (caminoRes->siguiente != NULL) {
			caminoRes = caminoRes->siguiente;
			axuliar = push_pila(axuliar, caminoRes->vertice);
		}

		fprintf(archivo, "%d", axuliar->vertice);		//Imprimo camino de vertices
		
		while (axuliar->siguiente != NULL) {
			axuliar = pop(axuliar);
			fprintf(archivo, " - %d", axuliar->vertice);
		}

		axuliar = pop(axuliar);
		total = flujoTotal(auxResultados, grafo);
		suma = total + suma;
		fprintf(archivo, ": %d\n", total);
		auxResultados = auxResultados->siguiente;
	}

	fprintf(archivo, "Total: %d\n\n", suma);
	fprintf(archivo, "Nodos Criticos\n");
	fprintf(archivo, "%d", auxCriticos->vertice);
	
	while (auxCriticos->siguiente != NULL) {
		auxCriticos = auxCriticos->siguiente;
		fprintf(archivo, " - %d", auxCriticos->vertice);
	}

	fclose(archivo);
}

void main()
{
	/*------------------Se ingresan los nombres de los archivos----------------------*/
	char nombreArchivoP[100];
	int numeroVertices = 0;
	int verticeSalida = 0;
	int verticeLlegada = 0;

	printf(" \n\nIngrese nombre del archivo:");
	scanf("%s", nombreArchivoP);
	

	/*------------------Se lee la matriz----------------------*/
	printf("Leyendo %s...\n\n", nombreArchivoP);
	Grafo* grafo = leerGrafo(nombreArchivoP, &numeroVertices, &verticeSalida, &verticeLlegada);					//Se el archivo de grafos
	
	if (grafo->vertice == -1) {
		printf("Error al leer archivo %s o no se encontro.", nombreArchivoP);
		return;
	}
	else
		printf("Archivo leido con exito.\n");
	
	Pila* recorrido = NULL;										//Creo la lista que tendra el recorrido
	Pila* pila = NULL;											//Creo una pila
	Resultados* resultado = NULL;								//Creo la lista que guardara los recorridos
	int* marcados = (int*)malloc(numeroVertices * sizeof(int));	//Creo un arreglo de igual tamaño que la cantidad de vertices

	for (int i = 0; i < numeroVertices; i++)					//Arreglo de "booleanos" para vertices recorridos
	{
		marcados[i] = 0;
	}

	pila = push_pila(pila, verticeSalida);								//Agrego el vertice inicial
	resultado = recoridoProfundidad(grafo, verticeLlegada, pila, recorrido, marcados, resultado); //Obtengo caminos posibles

	Pila* verticesCriticos = (Pila*)malloc(sizeof(Pila));		//Pila para guardar los nodos criticos
	
	verticesCriticos = busquedaVerticeCritico(grafo, numeroVertices);		//Busco los vertices criticos
	escribirDatos(verticesCriticos, resultado, grafo);		//Escribir datos
}



//IMPRIMIR GRAFO
//Grafo* puntero = grafo;					//Asigno puntero para posicion final
//Nodos* punteroN = grafo->adyacente;
//while (puntero != NULL)				//Posiciono puntero al final de la lista
//{
//	printf("El vertice es: %d\n", puntero->vertice);
//	printf("Los vertices adyacentes son: ");
//	while (punteroN != NULL)				//Posiciono puntero al final de la lista
//	{
//		printf("%d con peso %d. ", punteroN->vertice, punteroN->peso);
//		punteroN = punteroN->siguiente;
//	}
//	printf("\n");
//	puntero = puntero->sigVertice;
//	if (puntero != NULL) punteroN = puntero->adyacente;
//}


//IMPRIMIR RESULTADOS
//Resultados* puntero = resultado;					//Asigno puntero para posicion final
//Pila* erecorrido = puntero->recorrido;
//while (puntero != NULL)				//Posiciono puntero al final de la lista
//{
//	printf("El recorrido es: ");
//	printf("%d ", erecorrido->vertice);
//	erecorrido = erecorrido->siguiente;
//	while (erecorrido != NULL)				//Posiciono puntero al final de la lista
//	{
//		printf("- %d ", erecorrido->vertice);
//		erecorrido = erecorrido->siguiente;
//	}
//	printf("\n");
//	puntero = puntero->siguiente;
//	if (puntero != NULL) erecorrido = puntero->recorrido;
//}


//IMPRIMIR TODOS LOS VALORES TOTALES
//while (resultado != NULL)
//{
	//int total = flujoTotal(resultado, grafo);
	//printf("%d\n", total);
	//resultado = resultado->siguiente;
//
//}