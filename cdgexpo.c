#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    char nombre[100];
    int anio;
    char genero[50];
    float recaudacion;
    int altura;
    struct Nodo *izq, *der;
} Nodo;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int altura(Nodo* nodo) {
    return (nodo == NULL) ? 0 : nodo->altura;
}

Nodo* crearNodo(char nombre[], int anio, char genero[], float recaudacion) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nuevo->nombre, nombre);
    nuevo->anio = anio;
    strcpy(nuevo->genero, genero);
    nuevo->recaudacion = recaudacion;
    nuevo->altura = 1;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;
}

int obtenerBalance(Nodo* nodo) {
    return (nodo == NULL) ? 0 : altura(nodo->izq) - altura(nodo->der);
}

Nodo* rotarDerecha(Nodo* y) {
    Nodo* x = y->izq;
    Nodo* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

Nodo* rotarIzquierda(Nodo* x) {
    Nodo* y = x->der;
    Nodo* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

Nodo* insertar(Nodo* nodo, char nombre[], int anio, char genero[], float recaudacion) {
    if (nodo == NULL) return crearNodo(nombre, anio, genero, recaudacion);

    if (anio < nodo->anio)
        nodo->izq = insertar(nodo->izq, nombre, anio, genero, recaudacion);
    else if (anio > nodo->anio)
        nodo->der = insertar(nodo->der, nombre, anio, genero, recaudacion);
    else
        return nodo;

    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && anio < nodo->izq->anio)
        return rotarDerecha(nodo);
    if (balance < -1 && anio > nodo->der->anio)
        return rotarIzquierda(nodo);
    if (balance > 1 && anio > nodo->izq->anio) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && anio < nodo->der->anio) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void inorden(Nodo* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izq);
        printf("%s (%d) [%s] - $%.2fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        inorden(raiz->der);
    }
}

void preorden(Nodo* raiz) {
    if (raiz != NULL) {
        printf("%s (%d) [%s] - $%.2fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        preorden(raiz->izq);
        preorden(raiz->der);
    }
}

void posorden(Nodo* raiz) {
    if (raiz != NULL) {
        posorden(raiz->izq);
        posorden(raiz->der);
        printf("%s (%d) [%s] - $%.2fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    }
}

Nodo* buscarPelicula(Nodo* raiz, char nombre[]) {
    if (raiz == NULL) return NULL;
    if (strcmp(raiz->nombre, nombre) == 0) return raiz;
    Nodo* izq = buscarPelicula(raiz->izq, nombre);
    if (izq != NULL) return izq;
    return buscarPelicula(raiz->der, nombre);
}

void mostrarGenero(Nodo* raiz, char genero[]) {
    if (raiz != NULL) {
        mostrarGenero(raiz->izq, genero);
        if (strcmp(raiz->genero, genero) == 0) {
            printf("%s (%d) [%s] - $%.2fM\n", raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        }
        mostrarGenero(raiz->der, genero);
    }
}

typedef struct {
    char nombre[100];
    float recaudacion;
} Fracaso;

void llenarFracasos(Nodo* raiz, Fracaso fracasos[], int* index) {
    if (raiz != NULL) {
        llenarFracasos(raiz->izq, fracasos, index);
        strcpy(fracasos[*index].nombre, raiz->nombre);
        fracasos[*index].recaudacion = raiz->recaudacion;
        (*index)++;
        llenarFracasos(raiz->der, fracasos, index);
    }
}

int comparar(const void* a, const void* b) {
    Fracaso* fa = (Fracaso*)a;
    Fracaso* fb = (Fracaso*)b;
    return (fa->recaudacion > fb->recaudacion) - (fa->recaudacion < fb->recaudacion);
}

void mostrarFracasos(Nodo* raiz) {
    Fracaso fracasos[100];
    int n = 0;
    llenarFracasos(raiz, fracasos, &n);
    qsort(fracasos, n, sizeof(Fracaso), comparar);
    printf("\n3 Fracasos Taquilleros:\n");
    for (int i = 0; i < n && i < 3; i++) {
        printf("%s - $%.2fM\n", fracasos[i].nombre, fracasos[i].recaudacion);
    }
}

int main() {
    Nodo* raiz = NULL;
    int opcion;
    char nombre[100], genero[50];
    int anio;
    float recaudacion;

    do {
        printf("\n--- MEN\u00da ---\n");
        printf("1. Insertar pel\u00edcula\n");
        printf("2. Recorrido Inorden\n");
        printf("3. Recorrido Preorden\n");
        printf("4. Recorrido Posorden\n");
        printf("5. Buscar pel\u00edcula por nombre\n");
        printf("6. Mostrar pel\u00edculas por g\u00e9nero\n");
        printf("7. Mostrar 3 fracasos taquilleros\n");
        printf("0. Salir\n");
        printf("Opci\u00f3n: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("Nombre de la pel\u00edcula: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                printf("A\u00f1o de realizaci\u00f3n: ");
                scanf("%d", &anio);
                getchar();
                printf("G\u00e9nero: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = 0;
                printf("Dinero recaudado (en millones): ");
                scanf("%f", &recaudacion);
                getchar();
                raiz = insertar(raiz, nombre, anio, genero, recaudacion);
                break;
            case 2:
                printf("\nRecorrido Inorden:\n");
                inorden(raiz);
                break;
            case 3:
                printf("\nRecorrido Preorden:\n");
                preorden(raiz);
                break;
            case 4:
                printf("\nRecorrido Posorden:\n");
                posorden(raiz);
                break;
            case 5:
                printf("Nombre de la pel\u00edcula a buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                {
                    Nodo* buscado = buscarPelicula(raiz, nombre);
                    if (buscado) {
                        printf("Encontrado: %s (%d) [%s] - $%.2fM\n", buscado->nombre, buscado->anio, buscado->genero, buscado->recaudacion);
                    } else {
                        printf("Pel\u00edcula no encontrada.\n");
                    }
                }
                break;
            case 6:
                printf("G\u00e9nero a buscar: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = 0;
                printf("\nPel\u00edculas del g\u00e9nero %s:\n", genero);
                mostrarGenero(raiz, genero);
                break;
            case 7:
                mostrarFracasos(raiz);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opci\u00f3n inv\u00e1lida.\n");
        }

    } while (opcion != 0);

    return 0;
}