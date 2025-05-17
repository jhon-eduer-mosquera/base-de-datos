#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    char nombre[100];
    int anio;
    char genero[50];
    float recaudacion;
    struct Nodo *izq, *der;
} Nodo;

Nodo* crearNodo(char nombre[], int anio, char genero[], float recaudacion) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nuevo->nombre, nombre);
    nuevo->anio = anio;
    strcpy(nuevo->genero, genero);
    nuevo->recaudacion = recaudacion;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;
}

Nodo* insertar(Nodo* raiz, char nombre[], int anio, char genero[], float recaudacion) {
    if (raiz == NULL) {
        return crearNodo(nombre, anio, genero, recaudacion);
    }
    if (anio <= raiz->anio) {
        raiz->izq = insertar(raiz->izq, nombre, anio, genero, recaudacion);
    } else {
        raiz->der = insertar(raiz->der, nombre, anio, genero, recaudacion);
    }
    return raiz;
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

Nodo* encontrarMinimo(Nodo* raiz) {
    while (raiz && raiz->izq != NULL) {
        raiz = raiz->izq;
    }
    return raiz;
}

Nodo* eliminar(Nodo* raiz, int anio) {
    if (raiz == NULL) return NULL;

    if (anio < raiz->anio) {
        raiz->izq = eliminar(raiz->izq, anio);
    } else if (anio > raiz->anio) {
        raiz->der = eliminar(raiz->der, anio);
    } else {
        if (raiz->izq == NULL && raiz->der == NULL) {
            free(raiz);
            return NULL;
        } else if (raiz->izq == NULL) {
            Nodo* temp = raiz->der;
            free(raiz);
            return temp;
        } else if (raiz->der == NULL) {
            Nodo* temp = raiz->izq;
            free(raiz);
            return temp;
        } else {
            Nodo* sucesor = encontrarMinimo(raiz->der);
            strcpy(raiz->nombre, sucesor->nombre);
            raiz->anio = sucesor->anio;
            strcpy(raiz->genero, sucesor->genero);
            raiz->recaudacion = sucesor->recaudacion;
            raiz->der = eliminar(raiz->der, sucesor->anio);
        }
    }
    return raiz;
}

int main() {
    Nodo* raiz = NULL;
    int opcion;
    char nombre[100], genero[50];
    int anio;
    float recaudacion;

    do {
        printf("\n--- MENÚ ---\n");
        printf("1. Insertar película\n");
        printf("2. Recorrido Inorden\n");
        printf("3. Recorrido Preorden\n");
        printf("4. Recorrido Posorden\n");
        printf("5. Buscar película por nombre\n");
        printf("6. Mostrar películas por género\n");
        printf("7. Mostrar 3 fracasos taquilleros\n");
        printf("8. Eliminar película por año\n");
        printf("0. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("Nombre de la película: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                printf("Año de realización: ");
                scanf("%d", &anio);
                getchar();
                printf("Género: ");
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
                printf("Nombre de la película a buscar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                {
                    Nodo* buscado = buscarPelicula(raiz, nombre);
                    if (buscado) {
                        printf("Encontrado: %s (%d) [%s] - $%.2fM\n", buscado->nombre, buscado->anio, buscado->genero, buscado->recaudacion);
                    } else {
                        printf("Película no encontrada.\n");
                    }
                }
                break;
            case 6:
                printf("Género a buscar: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = 0;
                printf("\nPelículas del género %s:\n", genero);
                mostrarGenero(raiz, genero);
                break;
            case 7:
                mostrarFracasos(raiz);
                break;
            case 8:
                printf("Ingrese el año de la película a eliminar: ");
                scanf("%d", &anio);
                getchar();
                raiz = eliminar(raiz, anio);
                printf("Película eliminada si existía.\n");
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida.\n");
        }

    } while (opcion != 0);

    return 0;
}
