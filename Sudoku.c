
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


/*
#define MAGENTA "\x1b[35m"
#define AZUL "\x1b[34m"
#define MAGENTA_BRILLANTE "\x1b[95m"        Estos colores se van a usar en proximas etapas
#define AZUL_BRILLANTE "\x1b[94m"
#define CYAN "\x1b[36m"
#define CYAN_BRILLANTE "\x1b[396m"

 */

#define N 9 //Cantidad de filas y columnas

/* Declaracion de tipos de dato personalizados */

typedef struct nodo {
    int numero;
    struct nodo * siguiente;
}tNumero;

typedef int tArray[N];
typedef struct {
    tArray posicion[N];
} tTablero;

typedef struct {
	int cantFacil;
	int cantIntermedio;
	int cantDificil;
}tDif;

typedef struct {
	int dificultad; // 1=FACIL - 2=INTERMEDIO - 3=DIFICIL
}tStats;


/*PROTOTIPADO*/
void inicializarDatos();
void actualizarStats();
void abrirOCrearArchivo();
void mostrarImagenInicio();
void finalizarProceso();
void mostrarEstadisticas();
void ingresarDificultad();
void ingresarDatosSudoku();
void grabarRegistroSudoku();
char ingresarRespuesta();
void ingresarFilaSudoku();
void mostrarFila();
void mostrarSudoku(int tablero[N][N],int);
void mostrarInstrucciones();
void jugarGuardados();
void leerOpcion(int );
void mostrarMenu();
void borrarSudoku();
void comprobarSiExisteArchivo1(const char *);
void comprobarSiExisteArchivo2(const char *);

void inicializarLista();
bool listaVacia(tNumero *);
void ingresarAdelante(int);
void agregarNumero(int);
void elegirNumeroEliminar(int pContador);
void grabarNumerosASudoku();
void modificarNumFila();
void grabarStats();

/*PROTOTIPADO VALIDADOR*/
int esCorrecto(int tablero[N][N], int, int, int);
int resuelveSudoku(int tablero[N][N], int, int);


/*Declaracion de variables*/
tTablero vSudoku; /*Declaracion del registro vSudoku tipo tTablero*/
tStats regStats; /*Declaracion del registro regStats tipo tStats*/
FILE * fSudoku; /* Declaracion del archivo, puntero a la zona de memoria donde se va a iniciar la transferencia*/
FILE * fStats; /* Declaracion del archivo, donde se va almacenar el registro de juegos y su dificultad*/

tNumero * primerFila;

char respuesta;
int dificultad;
tDif cantDificultad;

/*Funcion principal*/
int main() {
    //mostrarImagenInicio();
    inicializarDatos();
    mostrarMenu();
    return 0;
}


/*Declaracion de funciones*/

void inicializarDatos(){
	abrirOCrearArchivo();
	actualizarStats();
}


void actualizarStats(){
	fStats = fopen("Stats.dat", "rb");
	fread(&regStats, sizeof(tStats), 1, fStats);
	while(!feof(fStats)){
		switch(regStats.dificultad){
			case 1:{
				cantDificultad.cantFacil++;
				break;
			}
			case 2:{
				cantDificultad.cantIntermedio++;
				break;
			}
			case 3:{
				cantDificultad.cantDificil++;
				break;
			}
		}
		fread(&regStats, sizeof(tStats), 1, fStats);
	}
	fclose(fStats);
}

void abrirOCrearArchivo() {  /* Se declara un puntero archivo con el nombre de nuestro archivo binario principal */
    char *fileName = "Sudoku.dat";
    char *failName = "Stats.dat";

    comprobarSiExisteArchivo1(fileName);
    sleep(1);
    comprobarSiExisteArchivo2(failName);
    sleep(1);
}

void comprobarSiExisteArchivo1(const char *nombreArchivo) { /* Validamos si existe el archivo mediante la funcion access() */

    if ( !access(nombreArchivo, F_OK )) {
        printf("\n\tEl archivo %s ha sido encontrado\n\n\t", nombreArchivo);
    }else {
        printf("\n\tEl archivo %s no existe\n\n\t", nombreArchivo);
        fSudoku = fopen("Sudoku.dat", "wb");
        printf("\n\tSe ha creado con exito\n\n\n");
    }
}

void comprobarSiExisteArchivo2(const char *nombreArchivo) { /* Validamos si existe el archivo mediante la funcion access() */

    if ( !access(nombreArchivo, F_OK )) {
        printf("\n\tEl archivo %s ha sido encontrado\n\n\t", nombreArchivo);
    }else {
        printf("\n\tEl archivo %s no existe\n\n\t", nombreArchivo);
        fStats = fopen("Stats.dat", "wb");
        printf("\n\tSe ha creado con exito\n\n\n");
    }
}


void inicializarLista(){
    primerFila=NULL;
}

bool listaVacia(tNumero * primerFila){
    return(primerFila==NULL);
}

void mostrarImagenInicio() {    /*  Imagen con el nombre de nuestro grupo */

    printf("\n\n\n\t\t::::::::  ::::    ::::   :::::::: :::::::::::       ::::::::  :::   :::  ::::::::\n");
    sleep(1);
    printf("\t\t:+:    :+: +:+:+: :+:+:+ :+:    :+:    :+:          :+:    :+: :+:   :+: :+:    :+: \n");
    sleep(1);
    printf("\t\t+:+        +:+ +:+:+ +:+ +:+    +:+    +:+          +:+         +:+ +:+  +:+        \n");
    sleep(1);
    printf("\t\t:#:        +#+  +:+  +#+ +#+    +:+    +#+          +#++:++#++   +#++:   +#++:++#++ \n");
    sleep(1);
    printf("\t\t+#+   +#+# +#+       +#+ +#+    +#+    +#+                 +#+    +#+           +#+ \n");
    sleep(1);
    printf("\t\t#+#    #+# #+#       #+# #+#    #+#    #+#          #+#    #+#    #+#    #+#    #+# \n");
    sleep(1);
    printf("\t\t ########  ###       ###  ########     ###           ########     ###     ########  \n\n\n");
    sleep(3);
    system("cls");
}


void mostrarMenu() {  /* Menu principal de nuestro programa */
    int opc;
    system("cls");
    printf("\n\n\t\t\t||\t\t *** Menu principal ***\t\t\t\t||", 165);
    printf("\n\t\t\t||-------------------------------------------------------------\t||");
    printf("\n\t\t\t||\t\t\t\t\t\t\t\t||");
    printf("\n\t\t\t||\tSeleccione una opcion para continuar\t\t\t||");
    printf("\n\t\t\t||-------------------------------------------------------------\t||");
    printf("\n\t\t\t||\t[1] Instrucciones del juego  \t\t\t\t||");
    printf("\n\t\t\t||\t[2] Generar tablero \t\t\t\t\t||");
    printf("\n\t\t\t||\t[3] Juegos guardados\t\t\t\t\t||");
    printf("\n\t\t\t||\t[4] Borrar un Sudoku\t\t\t\t\t||");
    printf("\n\t\t\t||\t[5] Estadisticas de Juego\t\t\t\t||");
    printf("\n\t\t\t||\t[6] Salir\t\t\t\t\t\t||");
    printf("\n\t\t\t||-------------------------------------------------------------\t||");
    printf("\n\t\t\t  \t*OPCION: ");
    scanf("%i", &opc);
    leerOpcion(opc);
}

void leerOpcion(int pOpc) {
    switch (pOpc){                  /* switch para el manejo de nuestro menu principal  */
        case 1: {
            mostrarInstrucciones();
            break;
        }
        case 2: {
            system("cls");
            ingresarDatosSudoku();
            break;
        }
        case 3: {
            jugarGuardados();
            break;
        }
        case 4: {
            borrarSudoku();
            mostrarMenu();
            break;
        }
        case 5: {
            mostrarEstadisticas();
            mostrarMenu();
            break;
        }
        case 6: {
            printf("\n\n\n\t\tMuchas gracias por jugar, esperamos que te haya gustado!\n\n\n\t");
            system("pause");
            exit(0);
        }
        default: {
            printf("\n\t\t\t   ***ERROR***\n\tOPCION INEXISTENTE\n\t\t", 165);
            system("pause");
            mostrarMenu();
            break;
        }
    }
}

void leerRegistroSudoku() {    /* se lee de a un tablero sudoku a la vez */
    /* lee el registro */
    fread(&vSudoku, sizeof(tTablero), 1, fSudoku);
    printf("\tRegistro de solucion de sudoku leido! \n");
}

void mostrarEstadisticas(){
	fStats = fopen("Stats.dat", "rb");
	fread(&regStats, sizeof(tStats), 1, fStats);
	cantDificultad.cantFacil=0;
	cantDificultad.cantIntermedio=0;
	cantDificultad.cantDificil=0;
	while(!feof(fStats)){
		switch(regStats.dificultad){
			case 1:{ 				// 1 = FACIL
				cantDificultad.cantFacil++;
				break;
			}
			case 2:{				// 2 = INTERMEDIO
				cantDificultad.cantIntermedio++;
				break;
			}
			case 3:{				// 3 = DIFICIL
				cantDificultad.cantDificil++;
				break;
			}
		}
		fread(&regStats, sizeof(tStats), 1, fStats);
	}
	fclose(fStats);
	system("cls");
	printf("\n\n\t***ESTADISTICAS DEL JUEGO***");
	printf("\n\n\tCANTIDAD DE JUEGOS JUGADOS");
	printf("\n\n\n\tDificultad\tCantidad");
	printf("\n\tFACIL\t\t    %d",cantDificultad.cantFacil);
	printf("\n\tINTERMEDIO\t    %d",cantDificultad.cantIntermedio);
	printf("\n\tDIFICIL\t\t    %d",cantDificultad.cantDificil);
	printf("\n\n\n\t");
	system("pause");
}

void mostrarInstrucciones() {  /* muestra las reglas del juego */
    printf("\n\n\t\tInstrucciones del juego:\n\n");
    printf("\n\n\t1) Hay que completar las casillas vacias con un solo numero del 1 al 9.");
    printf("\n\t2) En una misma fila no puede haber numeros repetidos.");
    printf("\n\t3) En una misma columna no puede haber numeros repetidos.");
    printf("\n\t4) En una misma cuadricula 3x3 no puede haber numeros repetidos.");
    printf("\n\t5) la solución de un sudoku es unica.\t\n\n");
    system("pause");
    mostrarMenu();
}


void ingresarDificultad() {  /* se ingresa la dificulad del tablero */
    system("cls");
    printf("\n\n\t\tSeleccione Dificultad:\n\n");
    printf("\t[1] Facil\n");
    printf("\t[2] Intermedio\n");
    printf("\t[3] Dificil\n");
    printf("\tOpcion: ");
    scanf("%d", &dificultad);
    switch (dificultad) {
        case 1: {
            dificultad = 80;
            break;
        }
        case 2: {
            dificultad = 60;
            break;
        }
        case 3: {
            dificultad = 40;
            break;
        }
        default: {
            printf("\t***ERROR***");
            printf("\tOpcion ingresada incorrecta\n\n\t");
            system("pause");
            //ingresarDificultad();  /* Recursividad */
        }
    }
}

void grabarStats(){
	int i;
	fStats = fopen("Stats.dat", "wb");
	for (i=1;i<=cantDificultad.cantFacil;i++){
		regStats.dificultad=1;
		fwrite(&regStats, sizeof(tStats), 1, fStats);
	}
	for (i=1;i<=cantDificultad.cantIntermedio;i++){
		regStats.dificultad=2;
		fwrite(&regStats, sizeof(tStats), 1, fStats);
	}
	for (i=1;i<=cantDificultad.cantDificil;i++){
		regStats.dificultad=3;
		fwrite(&regStats, sizeof(tStats), 1, fStats);
	}
	fclose(fStats);
}


void ingresarDatosSudoku() { /*se ingresa la primer linea del tablero, muestra en pantalla y pregunta al usuario si son correctos los datos */
    fSudoku = fopen("Sudoku.dat", "ab");
    respuesta = 'a';
    printf("\nIngrese la primer fila para generar el sudoku: \n");
    ingresarFilaSudoku();

    if (resuelveSudoku(vSudoku.posicion, 0, 0) == 1) { /* si lo ingresado es correcto se resuelve el sudoku, se graba en el archivo, se cierra el mismo y vuelve al menu*/
        printf("\tSudoku creado exiosamente!!\n");
        grabarRegistroSudoku();
        finalizarProceso();
        system("pause");
        mostrarMenu();
    }else {
        printf("\tNo tiene solucion"); /* los datos ingresados eran incorrectos, se cierra el archivo */
        finalizarProceso();
        system("pause");
        mostrarMenu();
    }
}


void jugarGuardados() { /* carga archivo en modo lectura y muestra sub menu */
    fSudoku = fopen("Sudoku.dat", "rb");
    ingresarDificultad();
    leerRegistroSudoku();
    while (!feof (fSudoku)){
        mostrarSudoku(vSudoku.posicion, dificultad);
        printf("\n\n\t[A] Siguiente Sudoku");
        printf("\n\t[B] Cambiar dificultad de este Sudoku");
        printf("\n\t[C] Mostrar solucion del sudoku");
        printf("\n\t[D] Volver al menu principal\n");
        printf("\n\tOPCION: ");
        respuesta = ingresarRespuesta();
        switch (respuesta) {
            case 'A': {
            	leerRegistroSudoku();
                system("cls");
                break;
            }
            case 'B': {
                ingresarDificultad();
                break;
            }
            case 'C': {
                mostrarSudoku(vSudoku.posicion, 100);
                system("pause");
                finalizarProceso();
                mostrarMenu();
                break;
            }
            case 'D': {
                system("cls");
                mostrarMenu();
                break;
            }
            default: {
                printf("\n\n\t  ERROR");
                printf("\n\tOPCION INCORRECTA");
                break;
            }
        }
        if ( feof(fSudoku) ) {
            printf("\n\t\tNO HAY MAS SUDOKUS PARA MOSTRAR\n\n\t");
            system("pause");
            break;
        }
    }
    finalizarProceso();
    mostrarMenu();
}

void ingresarFilaSudoku(){
	inicializarLista();
    system("cls");
    int bandera;
    bandera=0;
    char respuesta2;
    int contador,numero,i;
    contador=0;
    printf("\n\n\tINGRESO DE NUMEROS DE LA PRIMERA FILA\n\n");
    //pedir al usuario que decida ingresar mas numeros o no
    respuesta2='S';
    respuesta='N';
    while(respuesta!='S'){ //una respuesta que responda a que si los datos son correctos, para así grabar en el sudoku
        while(respuesta2=='S'){
        	if(bandera==0){
        		printf("\tIngrese primer numero: ");
        		bandera=1;
        	}else{
        		printf("\tIngrese N%c: ",167);
        	}
            scanf("%d",&numero);
            contador=contador+1;
            if (listaVacia(primerFila)){
                ingresarAdelante(numero);
            }else{
                agregarNumero(numero);
            }
            mostrarFila();
            printf("\n\tDesea ingresar mas numeros? S/N");
            respuesta2 = ingresarRespuesta();
            system("cls");
        }
        while(contador!=9){
            if(contador<9){
                for(i=contador+1;i<=9;i++){
                    agregarNumero(0);
                    contador=contador+1;
                }
            }else{
                elegirNumeroEliminar(contador); //llama a la funcion para eliminar un numero ingresado
                contador = contador-1;
            }
        }
        while(respuesta!='S'){
        	system("cls");
        	printf("\n\tLos datos son correctos para grabar en el sudoku? [S/N]\n\n");
        	mostrarFila();
        	printf("\n\tRespuesta: ");
        	respuesta = ingresarRespuesta();
        	if (respuesta=='N'){
				modificarNumFila();
        	}
        }
    }
    grabarNumerosASudoku();
}


void modificarNumFila(){
	int i,posNumEliminar,numNuevo;
	system("cls");
	printf("\n\tIngrese posicion numero a modificar\n\n\tPosicion: ");
	mostrarFila();
	scanf("%d",&posNumEliminar);
	printf("\n\tIngrese el nuevo valor: ");
	scanf("%d",&numNuevo);
	tNumero * aux;
	aux=primerFila;
	if(posNumEliminar==1){
		aux->numero=numNuevo;
	}else{
		for (i=1;i< (posNumEliminar);i++){
			aux=aux->siguiente;
		}
		aux->numero=numNuevo;
	}
}


void grabarNumerosASudoku(){
	int i;
	tNumero * nodoEliminar;
	//TENGO QUE HACER 2 COSAS:
	// 1.- Recorrer la lista para ir guardando en el ARRAY del sudoku
	// 2.- Recorrer la primer fila del ARRAY del sudoku para ir guardando lo que se lee de la lista
	for (i=0;i<=N-1;i++){
			//leer un numero de la lista, y copiarlo en la posición i del array
			vSudoku.posicion[0][i]=primerFila->numero;
			nodoEliminar=primerFila;
			primerFila=primerFila->siguiente;
			free(nodoEliminar);
			nodoEliminar=NULL;			
	}
	system("pause");
}



void elegirNumeroEliminar(int pContador){
	system("cls");
	printf("\n\tCHEEEE INGRESASTE MUCHOS NUMEROS, BORRA ALGUNOS!\n");
	int pos,i;
	printf("\n\n\tElija la posicion del numero que quiere quitar [1-%d]",pContador);
	mostrarFila();
	printf("\n\n\tPosicion: ");
	scanf("%d",&pos);
	tNumero * aux;
	tNumero * numAEliminar;
	numAEliminar=(tNumero *) malloc (sizeof(tNumero));
	aux=primerFila;
	if(pos==1){
		numAEliminar=primerFila;
		primerFila=primerFila->siguiente;
	}else{
		for (i=1;i< (pos-1);i++){
			aux=aux->siguiente;
		}
		numAEliminar=aux->siguiente;
		aux->siguiente=numAEliminar->siguiente;
	}
	printf("\n\tSe ha quitado el numero: ");
	printf("%d\n",numAEliminar->numero);
	system("pause");
	free(numAEliminar);
	numAEliminar=NULL;
}


void ingresarAdelante(int pNumero){
    tNumero * nuevoNodo;
    nuevoNodo = (tNumero*) malloc (sizeof(tNumero));
    nuevoNodo->numero=pNumero;
    nuevoNodo->siguiente=primerFila;
    primerFila=nuevoNodo;
    printf("\n\n\tNumero %d ingresado",nuevoNodo->numero);
}


void agregarNumero(int pNumero){
    tNumero * nuevoNodo;
    tNumero * aux;
    aux=primerFila;
    nuevoNodo = (tNumero*) malloc (sizeof(tNumero));
    nuevoNodo->numero=pNumero;
    while(aux->siguiente!=NULL){
        aux=aux->siguiente;
    }
    nuevoNodo->siguiente=aux->siguiente;
    aux->siguiente=nuevoNodo;
    printf("\n\n\tNumero %d ingresado",nuevoNodo->numero);
}


void mostrarFila() {  /* se muestra en pantalla los numeros ingresados y el usuario valida si son correctos */
    tNumero * aux;
    aux=primerFila;
    printf("\n\t");
    int contador;
    contador=1;
    while(aux!=NULL){
        printf("[%d]\t",contador);
        aux=aux->siguiente;
        contador++;
    }
    printf("\n\t");
    aux=primerFila;
    while(aux!=NULL){
        printf(" %d\t",aux->numero);
        aux=aux->siguiente;
    }
}

char ingresarRespuesta() {  /* lee opcion de usuario y la convierte a mayuscula */
    fflush(stdin);
    char pRespuesta;
    scanf("%c", &pRespuesta);
    pRespuesta = toupper(pRespuesta);
    return pRespuesta;
}


/* En base a un tablero parcialmente completado,
   intenta completar cumpliendo las reglas del juego */
int resuelveSudoku(int tablero[N][N], int fila, int columna) {
    int numero;
    // Si llega al final del tablero retorna 1
    if (fila == N - 1 && columna == N) {
        return 1;
    }

    /* Si el valor de la columna es 9, continua con la fila siguiente desde la posicion 0 */
    if (columna == N) {
        fila++;
        columna = 0;
    }

    /* Si la posicion actual del tablero ya contiene valores > 0 pasa a la columna siguiente  */
    if (tablero[fila][columna] > 0) {
        return resuelveSudoku(tablero, fila, columna + 1);
    }

    for (numero = 1; numero <= N; numero++) {

        /* Comprueba si es correcto asignar el numero en la fila y columna que se itera  */
        if (esCorrecto(tablero, fila, columna, numero) == 1) {
            /* asigna el numero en la fila y columna dada   */
            tablero[fila][columna] = numero;

            /*  Verifica la siguiente posibilidad con la columna siguiente  */
            if (resuelveSudoku(tablero, fila, columna + 1) == 1) {
                return 1;
            }
        }


        /* Eliminando el numero asignado, ya que nuestra comparacion era incorrecta, y probamos el siguiente valor  */
        tablero[fila][columna] = 0;
    }
    return 0;
}

/* Verifica si se puede asignar numero a la fila y a la columna  */
int esCorrecto(int tablero[N][N], int pFila, int pColumna, int pNumero) {
    int i, j, primerFila, primerColumna;
    /*  Si encuentra a numero en la fila retorna 0   */
    for (i = 0; i <= N - 1; i++) {
        if (tablero[pFila][i] == pNumero) {
            return 0;
        }
    }

    /* Si encuentra a pNumero en la columna retorno 0 */
    for (i = 0; i <= N - 1; i++) {
        if (tablero[i][pColumna] == pNumero) {
            return 0;
        }
    }

    /* Si encuentra a pNumero en la matriz 3x3, retorna 0  */
    primerFila = pFila - pFila % 3;
    primerColumna = pColumna - pColumna % 3;

    for ( i = 0; i <= 2; i++) {
        for ( j = 0; j <= 2; j++) {
            if (tablero[i + primerFila][j + primerColumna] == pNumero) {
                return 0;
            }
        }
    }
    return 1;
}

void borrarSudoku() {
    fSudoku = fopen("Sudoku.dat", "rb");
    int numSudoku, aux;
    tTablero vr_temporal;
    numSudoku = 0;
    while ( !feof (fSudoku) ) {
        leerRegistroSudoku();
        numSudoku++;
        dificultad = 100;
        mostrarSudoku(vSudoku.posicion, dificultad);
        printf("\n\tSudoku nro. %d", numSudoku);
        printf("\n\n\tDesea borrar este sudoku? S/N\n\n\t");
        printf("Opcion: ");
        respuesta = ingresarRespuesta();
        switch (respuesta) {
            case 'S': {
                break;
            }
            case 'N': {
                continue;
            }
            default: {
                printf("\n\n\t  ***ERROR***");
                printf("\n\tOPCION INCORRECTA");
                break;
            }
        }
        FILE * nuevo;
        nuevo = fopen("Sudoku.tmp","wb");   /*  archivo de backup para poder eliminar registros  */
        if (fSudoku == NULL) {
            printf("Ocurrio un error al crear el archivo\n");
        }
        fread(&vSudoku, sizeof(tTablero), 1, fSudoku);
        aux=1;
        while ( !feof(fSudoku) ){
            if ( numSudoku == aux ){
                printf("\n\n\tSudoku nro %d eliminado\n\n", aux);
            } else {
                fwrite(&vr_temporal, sizeof(tTablero), 1, nuevo);
                aux++;
            }
            if (fSudoku != NULL) {
                fread(&vSudoku, sizeof(tTablero), 1, fSudoku);
            }else {
                printf( "\tNo hay mas sudokus! \n" );
                break;
            }
        }
        fclose(fSudoku);
        fclose(nuevo);

        rename("Sudoku.dat","SudokuOld.dat");
        rename("Sudoku.tmp","Sudoku.dat");
        break;
    }
}


/* Imprime el tablero */
void mostrarSudoku(int tablero[N][N], int dificultad) {
	
	switch (dificultad) {
        case 80: {
            cantDificultad.cantFacil=cantDificultad.cantFacil+1;
            //se guarda en el archivo que se jugó 1 juego mas en
            //dificultad FACIL
            break;
        }
        case 60: {
            cantDificultad.cantIntermedio=cantDificultad.cantIntermedio+1;
            //se guarda en el archivo que se jugó 1 juego mas en
            //dificultad INTERMEDIO
            break;
        }
        case 40: {
            cantDificultad.cantDificil=cantDificultad.cantDificil+1;
            //se guarda en el archivo que se jugó 1 juego mas en
            //dificultad DIFICIL
            break;
        }
    }
	
	grabarStats();
	
    system("cls");
    int i, j, cantTope, cantNum = 0;
    cantTope = 100 - dificultad;
    printf("\n\n\t\t  S U D O K U\n");
    printf("\n\t  1 2 3      4 5 6      7 8 9");
    printf("\n\t%c", 201);
    for (j = 0; j <= 7; j++) {
        printf("%c", 205);
    }
    printf("%c", 203);
    for (j = 0; j <= 9; j++) {
        printf("%c", 205);
    }
    printf("%c", 203);
    for (j = 0; j <= 9; j++) {
        printf("%c", 205);
    }
    printf("%c", 187);
    printf("\n   ");
    for (i = 0; i <= 8; i++) {
        printf("  %i  ", i+1);
        if (i < 9){
            printf("%c", 186);
        }
        for (j = 0; j <= 8; j++) {  /* Dependiendo el valor de dificultad oculta ciertos valores para poder jugar  */
            if ( (rand() % 100) > dificultad && cantNum < cantTope) {
                printf(" %c", 254);
                cantNum++;
            } else {
                printf(" %i", tablero[i][j]);
            }
            if (j == 2 || j == 5 || j == 8) printf("  %c  ", 186);
        }
        printf("%i", i+1);

        printf("\n   ");
        if (i == 2 || i == 5 || i == 8){
            if (i == 2 || i == 5){
                printf("   \t%c", 204);
            } else {
                printf("\t%c", 200);
            }

            for (j = 0; j <= 7; j++) {
                printf("%c", 205);
            }

            if (i < 8) {
                printf("%c", 206);
            } else {
                printf("%c", 202);
            }

            for (j = 0; j <= 9; j++) {
                printf("%c", 205);
            }

            if (i < 8) {
                printf("%c", 206);
            } else{
                printf("%c", 202);
            }

            for (j = 0; j <= 9; j++) {
                printf("%c", 205);
            }
            if (i < 8) {
                printf("%c\n   ", 185);
            } else {
                printf("%c", 188);
            }
        }
    }
    printf("\n\t  1 2 3      4 5 6      7 8 9");
    printf("\n\n\n\t");
    
    
    printf("FACIL %d\n",cantDificultad.cantFacil);
    printf("INTERMEDIO %d\n",cantDificultad.cantIntermedio);
    printf("DIFICIL %d\n",cantDificultad.cantDificil);
    printf("\n\n\n\t");
}

void grabarRegistroSudoku() {
    /* Se graba fisicamente 1 registro en el archivo de Alumnos */
    fwrite(&vSudoku, sizeof(tTablero), 1, fSudoku);
    dificultad = 100;
    mostrarSudoku(vSudoku.posicion, dificultad);
    printf("\tRegistro de la solucion del sudoku guardado! \n");
}

void finalizarProceso() {
    fclose(fSudoku); /* Se cierra el archivo */
}
