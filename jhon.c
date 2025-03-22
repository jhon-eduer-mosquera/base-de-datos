#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// *Autores:* Jhon Eduer Mosquera Canga y [Nombre del compañero]

/* -----------------------------------------------
   Estructura para representar un producto en el inventario
   ----------------------------------------------- */
typedef struct Producto {
    int codigo;
    char nombre[50];
    int cantidad;
    int precio;
    struct Producto* siguiente;
} Producto;

// *Punteros para la cola FIFO*
Producto* frente = NULL;
Producto* final = NULL;

/* -----------------------------------------------
   Función para agregar un producto al inventario (FIFO)
   ----------------------------------------------- */
void agregarProducto(int codigo, char nombre[], int cantidad, int precio) {
    Producto* nuevo = (Producto*)malloc(sizeof(Producto));
    if (!nuevo) {
        printf("Error al asignar memoria.\n");
        return;
    }
    nuevo->codigo = codigo;
    strcpy(nuevo->nombre, nombre);
    nuevo->cantidad = cantidad;
    nuevo->precio = precio;
    nuevo->siguiente = NULL;

    if (final == NULL) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    printf("Producto agregado: %s\n", nombre);
}

/* -----------------------------------------------
   Función para mostrar todos los productos
   ----------------------------------------------- */
void mostrarProductos() {
    if (frente == NULL) {
        printf("Inventario vacío.\n");
        return;
    }
    Producto* temp = frente;
    printf("\nInventario Actual:\n");
    while (temp) {
        printf("Código: %d | Nombre: %s | Cantidad: %d | Precio: %d | Costo Total: %d\n",
               temp->codigo, temp->nombre, temp->cantidad, temp->precio, temp->cantidad * temp->precio);
        temp = temp->siguiente;
    }
}

/* -----------------------------------------------
   Función para actualizar la cantidad tras una compra (vender)
   ----------------------------------------------- */
void venderProducto(int codigo, int cantidadVendida) {
    Producto* temp = frente;
    while (temp) {
        if (temp->codigo == codigo) {
            if (temp->cantidad >= cantidadVendida) {
                temp->cantidad -= cantidadVendida;
                printf("Venta realizada: %d unidades de %s\n", cantidadVendida, temp->nombre);
                return;
            } else {
                printf("Stock insuficiente.\n");
                return;
            }
        }
        temp = temp->siguiente;
    }
    printf("Producto no encontrado.\n");
}

/* -----------------------------------------------
   Función para calcular el costo total del inventario
   ----------------------------------------------- */
void calcularCostoTotal() {
    int total = 0;
    Producto* temp = frente;
    while (temp) {
        total += temp->cantidad * temp->precio;
        temp = temp->siguiente;
    }
    printf("Costo total del inventario: %d\n", total);
}

/* -----------------------------------------------
   Función para eliminar un producto (solo si cantidad es 0)
   ----------------------------------------------- */
void eliminarProducto(int codigo) {
    if (frente == NULL) {
        printf("Inventario vacío.\n");
        return;
    }
    
    Producto* temp = frente;
    Producto* anterior = NULL;

    while (temp) {
        if (temp->codigo == codigo) {
            if (temp->cantidad == 0) {
                if (anterior == NULL) {
                    frente = temp->siguiente;
                } else {
                    anterior->siguiente = temp->siguiente;
                }
                if (temp == final) {
                    final = anterior;
                }
                free(temp);
                printf("Producto eliminado con éxito.\n");
                return;
            } else {
                printf("No se puede eliminar, aún hay stock.\n");
                return;
            }
        }
        anterior = temp;
        temp = temp->siguiente;
    }
    printf("Producto no encontrado.\n");
}

/* -----------------------------------------------
   Función para liberar toda la memoria al salir
   ----------------------------------------------- */
void liberarMemoria() {
    Producto* temp;
    while (frente) {
        temp = frente;
        frente = frente->siguiente;
        free(temp);
    }
    printf("Memoria liberada correctamente.\n");
}

/* -----------------------------------------------
   Función principal (menú de opciones)
   ----------------------------------------------- */
int main() {
    int opcion, codigo, cantidad, precio;
    char nombre[50];

    do {
        printf("\n--- Gestión de Inventario FIFO ---\n");
        printf("1. Agregar Producto\n2. Mostrar Productos\n3. Vender Producto\n4. Calcular Costo Total\n5. Eliminar Producto\n6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese código: ");
                scanf("%d", &codigo);
                getchar();  // Consumir el salto de línea para evitar problemas con fgets
                printf("Ingrese nombre: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;  // Eliminar el salto de línea final
                printf("Ingrese cantidad: ");
                scanf("%d", &cantidad);
                printf("Ingrese precio: ");
                scanf("%d", &precio);
                agregarProducto(codigo, nombre, cantidad, precio);
                break;
            case 2:
                mostrarProductos();
                break;
            case 3:
                printf("Ingrese código del producto vendido: ");
                scanf("%d", &codigo);
                printf("Ingrese cantidad vendida: ");
                scanf("%d", &cantidad);
                venderProducto(codigo, cantidad);
                break;
            case 4:
                calcularCostoTotal();
                break;
            case 5:
                printf("Ingrese código del producto a eliminar: ");
                scanf("%d", &codigo);
                eliminarProducto(codigo);
                break;
            case 6:
                liberarMemoria();
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 6);

    return 0;
}