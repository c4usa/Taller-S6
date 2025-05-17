#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_RECURSOS 5
#define MAX_LONGITUD_NOMBRE 30

char nombreProductos[MAX_PRODUCTOS][MAX_LONGITUD_NOMBRE] = {
    "Laptop",
    "Kindle",
    "EMSI Claw",
    "Teclado",
    "Mouse"};

int recursosRequeridos[MAX_PRODUCTOS][MAX_RECURSOS] = {
    {3, 1, 1, 1, 1},  // Laptop: 3 sensores, 1 pantalla, 1 procesador, 1 micrófono, 1 PCB
    {4, 1, 1, 0, 1},  // Kindle: 4 sensores, 1 pantalla, 1 procesador, 0 micrófono, 1 PCB
    {10, 1, 1, 1, 1}, // EMSI Claw: 10 sensores, 1 pantalla, 1 procesador, 1 micrófono, 1 PCB
    {2, 1, 1, 0, 1},  // Teclado: 2 sensores, 1 pantalla, 1 procesador, 0 micrófono, 1 PCB
    {2, 0, 1, 0, 1}   // Mouse: 2 sensores, 0 pantallas, 1 procesador, 0 micrófono, 1 PCB
};

float horasFabricacion[MAX_PRODUCTOS] = {4, 2, 6, 3, 4};

char nombreRecursos[MAX_RECURSOS][MAX_LONGITUD_NOMBRE] = {
    "Sensores",
    "Pantallas",
    "Procesadores",
    "Microfono",
    "PCB"};

int stockRecursos[MAX_RECURSOS];
int unidadesDemandadas[MAX_PRODUCTOS] = {0};
int productoRegistrado[MAX_PRODUCTOS] = {0};
float horasDisponibles;

int leerEntero(char *mensaje)
{
    int valor;
    printf("%s", mensaje);
    while (scanf("%d", &valor) != 1 || valor < 0)
    {
        printf("Entrada invalida. Ingrese un valor correcto: ");
        while (getchar() != '\n')
            ;
    }
    while (getchar() != '\n')
        ;
    return valor;
}

float leerFloat(char *mensaje)
{
    float valor;
    printf("%s", mensaje);
    while (scanf("%f", &valor) != 1 || valor < 0)
    {
        printf("Entrada invalida. Ingrese un valor correcto: ");
        while (getchar() != '\n')
            ;
    }
    while (getchar() != '\n')
        ;
    return valor;
}

char leerCaracter(char *mensaje)
{
    char valor;
    printf("%s", mensaje);
    scanf(" %c", &valor);
    while (getchar() != '\n')
        ;
    return valor;
}

int buscarProducto(char *nombre)
{
    for (int i = 0; i < MAX_PRODUCTOS; i++)
    {
        if (strcmp(nombreProductos[i], nombre) == 0)
            return i;
    }
    return -1;
}

void ingresarInventario()
{
    printf("\n=== INGRESAR INVENTARIO DE RECURSOS ===\n");
    for (int i = 0; i < MAX_RECURSOS; i++)
    {
        printf("Ingrese cantidad de %s disponibles: ", nombreRecursos[i]);
        stockRecursos[i] = leerEntero("");
    }
    horasDisponibles = leerFloat("Ingrese tiempo total disponible de la empresa en horas: ");
    printf("Inventario y tiempo ingresados correctamente.\n");
}

void mostrarInventario()
{
    printf("\n=== INVENTARIO ACTUAL ===\n");
    for (int i = 0; i < MAX_RECURSOS; i++)
    {
        printf("%-15s: %d\n", nombreRecursos[i], stockRecursos[i]);
    }
    printf("Tiempo disponible: %.2f horas\n", horasDisponibles);
}

void producirProducto()
{
    char nombre[MAX_LONGITUD_NOMBRE];
    int indice, cantidad;
    printf("\n=== PRODUCIR PRODUCTO ===\n");
    do
    {
        printf("Productos disponibles: Laptop, Kindle, EMSI Claw, Teclado, Mouse\n");
        printf("Ingrese nombre del producto: ");
        fgets(nombre, MAX_LONGITUD_NOMBRE, stdin);
        nombre[strcspn(nombre, "\n")] = 0;
        indice = buscarProducto(nombre);
        if (indice < 0)
            printf("Producto no encontrado.\n");
    } while (indice < 0);

    cantidad = leerEntero("Ingrese cantidad a producir: ");
    int recursosDisponibles = 1;
    float tiempoNecesario = horasFabricacion[indice] * cantidad;
    printf("\nRecurso\tDisp\tReq\tSuficiente\n");
    for (int i = 0; i < MAX_RECURSOS; i++)
    {
        int requerido = recursosRequeridos[indice][i] * cantidad;
        printf("%s\t%d\t%d\t%s\n", nombreRecursos[i], stockRecursos[i], requerido,
               stockRecursos[i] >= requerido ? "Si" : "No");
        if (stockRecursos[i] < requerido)
            recursosDisponibles = 0;
    }
    printf("Tiempo requerido: %.2f horas (Disp: %.2f)\n", tiempoNecesario, horasDisponibles);
    if (tiempoNecesario > horasDisponibles)
        recursosDisponibles = 0;
    if (!recursosDisponibles)
    {
        printf("Recursos/tiempo insuficiente.\n");
        return;
    }
    if (leerCaracter("Confirmar produccion? (S/N): ") == 'S')
    {
        for (int i = 0; i < MAX_RECURSOS; i++)
            stockRecursos[i] -= recursosRequeridos[indice][i] * cantidad;
        horasDisponibles -= tiempoNecesario;
        unidadesDemandadas[indice] += cantidad;
        productoRegistrado[indice] = 1;
        printf("Produccion exitosa: %d de %s.\n", cantidad, nombreProductos[indice]);
    }
}

void editarDatos()
{
    printf("\n=== EDITAR DEMANDA DE UN PRODUCTO ===\n");
    int existe = 0;
    printf("Productos con demanda registrada:\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++)
        if (productoRegistrado[i])
        {
            existe = 1;
            printf("- %s: %d unidades\n", nombreProductos[i], unidadesDemandadas[i]);
        }
    if (!existe)
    {
        printf("No hay demanda registrada.\n");
        return;
    }
    char nombre[MAX_LONGITUD_NOMBRE];
    printf("\nIngrese nombre del producto a editar: ");
    fgets(nombre, MAX_LONGITUD_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;
    int posicion = buscarProducto(nombre);

    if (posicion < 0 || !productoRegistrado[posicion])
    {
        printf("Producto no encontrado o sin demanda registrada.\n");
        return;
    }
    printf("\nInformación actual de %s:\n", nombreProductos[posicion]);
    printf("- Unidades demandadas: %d\n", unidadesDemandadas[posicion]);
    char respuesta = leerCaracter("¿Desea modificar la cantidad demandada? (S/N): ");
    if (respuesta != 'S' && respuesta != 's')
    {
        printf("No se realizaron cambios.\n");
        return;
    }
    int nuevaCantidad = leerEntero("Nueva cantidad demandada: ");
    int diferencia = nuevaCantidad - unidadesDemandadas[posicion];
    if (diferencia == 0)
    {
        printf("No se realizaron cambios en la cantidad.\n");
        return;
    }
    float tiempoDiferencia = diferencia * horasFabricacion[posicion];
    int factible = 1;
    if (diferencia > 0)
    {
        for (int i = 0; i < MAX_RECURSOS; i++)
            if (stockRecursos[i] < recursosRequeridos[posicion][i] * diferencia)
            {
                printf("Recurso insuficiente: %s (Disponible: %d, Necesario: %d)\n",
                       nombreRecursos[i], stockRecursos[i], recursosRequeridos[posicion][i] * diferencia);
                factible = 0;
            }
        if (horasDisponibles < tiempoDiferencia)
        {
            printf("Tiempo insuficiente: Disponible %.2f horas, Necesario %.2f horas adicionales\n",
                   horasDisponibles, tiempoDiferencia);
            factible = 0;
        }
        if (!factible)
        {
            printf("No es posible aumentar la demanda por recursos/tiempo insuficiente.\n");
            return;
        }
        respuesta = leerCaracter("Confirmar cambio de demanda? (S/N): ");
        if (respuesta != 'S' && respuesta != 's')
        {
            printf("Cambios cancelados.\n");
            return;
        }
        for (int i = 0; i < MAX_RECURSOS; i++)
            stockRecursos[i] -= recursosRequeridos[posicion][i] * diferencia;
        horasDisponibles -= tiempoDiferencia;
    }
    else if (diferencia < 0)
    {
        respuesta = leerCaracter("Confirmar reducción de demanda? (S/N): ");
        if (respuesta != 'S' && respuesta != 's')
        {
            printf("Cambios cancelados.\n");
            return;
        }
        for (int i = 0; i < MAX_RECURSOS; i++)
            stockRecursos[i] += recursosRequeridos[posicion][i] * (-diferencia);
        horasDisponibles += (-diferencia) * horasFabricacion[posicion];
    }
    unidadesDemandadas[posicion] = nuevaCantidad;
    if (nuevaCantidad == 0)
        productoRegistrado[posicion] = 0;

    printf("Demanda actualizada exitosamente. Nueva cantidad: %d unidades.\n", nuevaCantidad);
}

void eliminarDatos()
{
    printf("\n=== ELIMINAR DEMANDA DE UN PRODUCTO ===\n");
    int existe = 0;
    for (int i = 0; i < MAX_PRODUCTOS; i++)
        if (productoRegistrado[i])
        {
            existe = 1;
            printf("- %s: %d\n", nombreProductos[i], unidadesDemandadas[i]);
        }
    if (!existe)
    {
        printf("No hay demanda registrada.\n");
        return;
    }
    char nombre[MAX_LONGITUD_NOMBRE];
    printf("Ingrese nombre del producto a eliminar: ");
    fgets(nombre, MAX_LONGITUD_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = 0;
    int posicion = buscarProducto(nombre);
    if (posicion < 0 || !productoRegistrado[posicion])
    {
        printf("Producto invalido.\n");
        return;
    }
    char confirmar = leerCaracter("Desea eliminar la demanda de este producto? (S/N): ");
    if (confirmar != 'S' && confirmar != 's')
    {
        printf("Eliminacion cancelada.\n");
        return;
    }
    for (int i = 0; i < MAX_RECURSOS; i++)
        stockRecursos[i] += recursosRequeridos[posicion][i] * unidadesDemandadas[posicion];
    horasDisponibles += unidadesDemandadas[posicion] * horasFabricacion[posicion];
    unidadesDemandadas[posicion] = 0;
    productoRegistrado[posicion] = 0;
    printf("Demanda eliminada y recursos devueltos.\n");
}

void mostrarResumenDemanda()
{
    printf("\n=== RESUMEN DE DEMANDA ===\n");
    int existe = 0;
    for (int i = 0; i < MAX_PRODUCTOS; i++)
        if (productoRegistrado[i])
        {
            existe = 1;
            printf("- %s: %d unidades\n", nombreProductos[i], unidadesDemandadas[i]);
        }
    if (!existe)
        printf("No hay demanda registrada.\n");
}

void mostrarEstadisticasProduccion()
{
    printf("\n=== ESTADISTICAS DE PRODUCCION ===\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++)
    {
        printf("%s (%.2f h): ", nombreProductos[i], horasFabricacion[i]);
        for (int j = 0; j < MAX_RECURSOS; j++)
            printf("%s:%d ", nombreRecursos[j], recursosRequeridos[i][j]);
        printf("\n");
    }
}

void mostrarFabricables()
{
    printf("\n=== PRODUCTOS FABRICABLES CON INVENTARIO ACTUAL ===\n");
    int hayProductos = 0;
    for (int i = 0; i < MAX_PRODUCTOS; i++)
    {
        int maxUnidades = 999999;
        for (int j = 0; j < MAX_RECURSOS; j++)
        {
            int requerido = recursosRequeridos[i][j];
            if (requerido > 0)
            {
                int posible = stockRecursos[j] / requerido;
                if (posible < maxUnidades)
                    maxUnidades = posible;
            }
        }
        int maxPorTiempo = (int)(horasDisponibles / horasFabricacion[i]);
        int cantidadPosible = (maxUnidades < maxPorTiempo) ? maxUnidades : maxPorTiempo;
        if (cantidadPosible > 0)
        {
            printf("- %s: hasta %d unidades\n", nombreProductos[i], cantidadPosible);
            hayProductos = 1;
        }
    }
    if (!hayProductos)
        printf("No hay productos que puedan fabricarse actualmente.\n");
}

void mostrarMenu()
{
    printf("\n============= EMSI SYSTEM ==============\n");
    printf("\n================ MENU ==================\n");
    printf("1. Ingresar inventario de recursos\n");
    printf("2. Mostrar inventario actual\n");
    printf("3. Producir producto\n");
    printf("4. Editar demanda de un producto\n");
    printf("5. Eliminar demanda de un producto\n");
    printf("6. Mostrar resumen de demanda\n");
    printf("7. Mostrar estadisticas de produccion\n");
    printf("8. Salir\n");
    printf("9. Mostrar productos fabricables con inventario\n");
    printf("===========================================\n");
}

int main()
{
    int opcion;
    int inicializado = 0;
    do
    {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion)
        {
        case 1:
            ingresarInventario();
            inicializado = 1;
            break;
        case 2:
            if (!inicializado)
                printf("Primero debe ingresar inventario.\n");
            else
                mostrarInventario();
            break;
        case 3:
            if (!inicializado)
                printf("Primero debe ingresar inventario.\n");
            else
                producirProducto();
            break;
        case 4:
            if (!inicializado)
                printf("Primero debe ingresar inventario.\n");
            else
                editarDatos();
            break;
        case 5:
            if (!inicializado)
                printf("Primero debe ingresar inventario.\n");
            else
                eliminarDatos();
            break;
        case 6:
            mostrarResumenDemanda();
            break;
        case 7:
            mostrarEstadisticasProduccion();
            break;
        case 8:
            printf("Saliendo de EMSI SYSTEM.....\n");
            break;
        case 9:
            if (!inicializado)
                printf("Primero debe ingresar el inventario.\n");
            else
                mostrarFabricables();
            break;
        default:
            printf("Opcion invalida. Intente de nuevo\n");
        }
    } while (opcion != 8);
    return 0;
}