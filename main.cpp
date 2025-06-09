#include <iostream>
#include "GestorCSV.h"
#include <limits>
using namespace std;

void mostrarMenu() {
    cout << "\n=== MENU DE ANÁLISIS DE VENTAS ===" << endl;
    cout << "1. Mostrar todos los registros" << endl;
    cout << "2. Agregar una venta" << endl;
    cout << "3. Eliminar una venta por país o ciudad" << endl;
    cout << "4. Modificar una venta por ID" << endl;
    cout<<  "5. Entrar a Estadisticas"<<endl;
    cout<<  "6. Consultas Dinamica"<<endl;
    cout << "0. Salir" << endl;
}
void mostrarMenu2(){

  cout << "\n=== MENU DE Estadisticas DE VENTAS ===" << endl;
  cout << "1. Top 5 ciudades con mayor monto por país" << endl;
  cout << "2. Monto total vendido por producto y país" << endl;
  cout << "3. Promedio de ventas por categoría y país" << endl;
  cout << "4. Medio de envío más utilizado por país" << endl;
  cout << "5. Medio de envío más utilizado por categoría" << endl;
  cout << "6. Día con mayor monto de ventas" << endl;
  cout << "7. Estado de envío más frecuente por país" << endl;
  cout << "8. Producto más y menos vendido por cantidad" << endl;
  cout << "0. Salir" << endl;
}

void mostrarmenu3(){


  cout << "\n=== MENU DE Consultas Dinamicas ===" << endl;
  cout << "1. Listado de ventas realizadas en una ciudad específica" << endl;
  cout << "2. Listado de ventas realizadas en un rango de fechas por país." << endl;
  cout << "3. Comparación entre dos productos discriminado por todos los paises" << endl;
  cout << "4. Comparación entre dos países" << endl;
  cout << "5. Buscar productos vendidos en promedio por debajo de un monto total especificado por el usuario y por paìs." << endl;
  cout << "6. Buscar productos vendidos en promedio por encima de un monto total especificado por el usuario" << endl;
  cout << "0. Salir" << endl;


}

int leerOpcion() {
    string entrada;
    int opcion;

    while (true) {
        cout << "Ingrese una opción: ";
        getline(cin, entrada);  

        try {
            opcion = stoi(entrada);  
            return opcion;          
        } catch (const invalid_argument&) {
            cout << "Error: Ingrese solo números (sin letras ni símbolos).\n";
        } catch (const out_of_range&) {
            cout << "Error: Número demasiado grande.\n";
        }
    }
}


int main() {
    extern void medirTiempo(const std::string& nombre, std::function<void()> funcion);
    GestorCSV gestor;
    const string archivo = "ventas_sudamerica.csv";

    if (!gestor.leerArchivo(archivo)) {
        cerr << "No se pudo procesar el archivo '" << archivo << "'.\n";
        return 1;
    }
int opcion, opcion1;
do {
    mostrarMenu();
    opcion = leerOpcion();

    if (opcion < 0 || opcion > 6) {
        cout << "Opción fuera de rango. Intente de nuevo.\n";
        continue;  
    }

    cout << endl;

    switch (opcion) {
        case 1: medirTiempo("Imprimir Registros", [&]() { gestor.imprimirRegistros(); }); break;
        case 2: medirTiempo("Agregar Venta", [&]() { gestor.agregarVenta(); }); break;
        case 3: medirTiempo("Eliminar Venta", [&]() { gestor.eliminarVenta(); }); break;
        case 4: medirTiempo("Modificar Venta", [&]() { gestor.modificarVenta(); }); break;
        case 5:
            do {
                mostrarMenu2();
                opcion1 = leerOpcion();

                if (opcion1 < 0 || opcion1 > 8) {
                    cout << "Opción fuera de rango. Intente de nuevo.\n";
                    continue;
                }

                cout << endl;

                switch (opcion1) {
                    case 1: medirTiempo("Top 5 Ciudades", [&]() { gestor.mostrarTop5CiudadesPorPais(); }); break;
                    case 2: medirTiempo("Monto por Producto por Pais", [&]() { gestor.mostrarMontoPorProductoPorPais(); }); break;
                    case 3: medirTiempo("Promedio por Categoria por Pais", [&]() { gestor.mostrarPromedioPorCategoriaPorPais(); }); break;
                    case 4: medirTiempo("Medio de Envio por Pais", [&]() { gestor.mostrarMedioEnvioPorPais(); }); break;
                    case 5: medirTiempo("Medio de Envio por Categoria", [&]() { gestor.mostrarMedioEnvioPorCategoria(); }); break;
                    case 6: medirTiempo("Dia de Mayor Venta", [&]() { gestor.mostrarDiaMayorVenta(); }); break;
                    case 7: medirTiempo("Estado de Envio por Pais", [&]() { gestor.mostrarEstadoEnvioPorPais(); }); break;
                    case 8: medirTiempo("Producto Mas y Menos Vendido", [&]() { gestor.mostrarProductoMasYMenosVendido(); }); break;
                    case 0: cout << "Volviendo al menú principal...\n"; break;
                }
            } while (opcion1 != 0);
            break;
        case 6: 
            do {
                mostrarmenu3();
                opcion1 = leerOpcion();

                if (opcion1 < 0 || opcion1 > 7) {
                    cout << "Opción fuera de rango. Intente de nuevo.\n";
                    continue;
                }

                cout << endl;

                string productoA, productoB;
                string pais1, pais2;
                switch (opcion1) {
                    case 1: medirTiempo("Listado de ventas por ciudad", [&]() {gestor.mostrarVentasPorCiudad(); }); break;
                    case 2: medirTiempo("Listado de ventas por rango de fechas", [&]() { gestor.mostrarVentasPorRangoFechaYPais(); }); break;
                    case 3:
                        cout << "Ingrese el nombre del primer producto: ";
                        getline(cin, productoA);
                        cout << "Ingrese el nombre del segundo producto: ";
                        getline(cin, productoB);
                        medirTiempo("Comparación entre dos Productos", [&]() { gestor.compararProductosPorPaisAVL(productoA, productoB); }); break;
                    case 4:
                        cout << "Ingrese el nombre del primer país: ";
                        getline(cin, pais1);
                        cout << "Ingrese el nombre del segundo país: ";
                        getline(cin, pais2);
                        medirTiempo("Comparación entre dos Paises", [&]() { gestor.compararPaisesAVL(pais1, pais2); }); break;
                    case 5:
                        medirTiempo("Buscar productos vendidos por debajo de un monto", [&]() { gestor.buscarProductosPorDebajoDelUmbral(); });
                        break;
                    case 6: medirTiempo("Buscar productos vendidos por encima de un monto", [&]() { gestor.buscarProductosPorEncimaDelUmbralPorPais(); }); break;
                    case 0: cout << "Volviendo al menú principal...\n"; break;
                }
            } while (opcion1 != 0);
            break;

        case 0:
            cout << "Saliendo del programa...\n";
            break;
    }

} while (opcion != 0);
}