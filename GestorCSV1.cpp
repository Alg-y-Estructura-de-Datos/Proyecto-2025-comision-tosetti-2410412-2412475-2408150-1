#include "GestorCSV.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include "HashMapList.h"
#include "ArbolBinarioAVL.h"
#include <climits>
using namespace std;
using namespace std::chrono;

int contadorIfs = 0;


string obtenerFechaActual() {
    time_t ahora = time(nullptr);
    tm* local = localtime(&ahora);
    stringstream ss;
    ss << put_time(local, "%Y-%m-%d");  
    return ss.str();
}

void medirTiempo(const std::string& nombre, std::function<void()> funcion) {
    auto inicio = high_resolution_clock::now();
    funcion();
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    std::cout << "Tiempo de ejecución de '" << nombre << "': " << duracion.count() << " ms\n";
}

void Registro::imprimir() const {
    cout << "ID: " << id_venta
         << ", Fecha: " << fecha
         << ", País: " << pais
         << ", Ciudad: " << ciudad
         << ", Cliente: " << cliente
         << ", Producto: " << producto
         << ", Categoría: " << categoria
         << ", Cantidad: " << cantidad
         << ", Precio Unitario: " << precio_unitario
         << ", Monto Total: " << monto_total
         << ", Medio Envío: " << medio_envio
         << ", Estado Envío: " << estado_envio << '\n';
}

bool GestorCSV::leerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << '\n';
        return false;
    }

    string linea;
    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        istringstream stream(linea);
        string idVentaStr, cantidadStr, precioUnitarioStr, montoTotalStr;
        Registro reg;

        if (getline(stream, idVentaStr, ',') &&
            getline(stream, reg.fecha, ',') &&
            getline(stream, reg.pais, ',') &&
            getline(stream, reg.ciudad, ',') &&
            getline(stream, reg.cliente, ',') &&
            getline(stream, reg.producto, ',') &&
            getline(stream, reg.categoria, ',') &&
            getline(stream, cantidadStr, ',') &&
            getline(stream, precioUnitarioStr, ',') &&
            getline(stream, montoTotalStr, ',') &&
            getline(stream, reg.medio_envio, ',') &&
            getline(stream, reg.estado_envio, ',')) {

          


            reg.id_venta = stoi(idVentaStr);
            reg.cantidad = stoi(cantidadStr);
            reg.precio_unitario = stod(precioUnitarioStr);
            reg.monto_total = stod(montoTotalStr);

            if (reg.id_venta >= siguienteIdVenta) {
      siguienteIdVenta = reg.id_venta + 1;
}

            registros.push_back(reg);
        }
    }
    archivo.close();
    return true;
}

void GestorCSV::imprimirRegistros() const {
    for (const auto& reg : registros) {
        reg.imprimir();
    }
}

void GestorCSV::mostrarTop5CiudadesPorPais() const {
    map<string, map<string, double>> datos;
    for (const auto& v : registros) {
        datos[v.pais][v.ciudad] += v.monto_total;
    }

    cout << "Top 5 de ciudades con mayor monto de ventas por pais:\n";
    for (const auto& pais : datos) {
        cout << "Pais: " << pais.first << endl;
        vector<pair<string, double>> ciudades(pais.second.begin(), pais.second.end());
        sort(ciudades.begin(), ciudades.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });
        for (int i = 0; i < min(5, (int)ciudades.size()); ++i) {
            cout << "  " << ciudades[i].first << ": $" << ciudades[i].second << endl;
        }
    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarMontoPorProductoPorPais() const {
    map<string, map<string, double>> datos;
    for (const auto& v : registros) {
        datos[v.pais][v.producto] += v.monto_total;
    }
    cout << "Monto total vendido por producto y pais:\n";
    for (const auto& pais : datos) {
        cout << "Pais: " << pais.first << endl;
        for (const auto& prod : pais.second) {
            cout << "  " << prod.first << ": $" << prod.second << endl;
        }
    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarPromedioPorCategoriaPorPais() const {
    map<string, map<string, pair<double, int>>> datos;
    for (const auto& v : registros) {
        auto& item = datos[v.pais][v.categoria];
        item.first += v.monto_total;
        item.second++;
    }
    cout << "Promedio de ventas por categoria en cada pais:\n";
    for (const auto& pais : datos) {
        cout << "Pais: " << pais.first << endl;
        for (const auto& cat : pais.second) {
            double promedio = cat.second.first / cat.second.second;
            cout << "  " << cat.first << ": $" << promedio << endl;
        }
    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarMedioEnvioPorPais() const {
    contadorIfs = 0;
    map<string, map<string, int>> datos;
    for (const auto& v : registros) {
        datos[v.pais][v.medio_envio]++;
    }
    cout << "Medio de envio mas utilizado por pais:\n";
    for (const auto& pais : datos) {
        string medio;
        int maxUso = 0;
        for (const auto& medioUso : pais.second) {
            if ((++contadorIfs,medioUso.second > maxUso)) {
                maxUso = medioUso.second;
                medio = medioUso.first;
            }
        }
        cout << "  " << pais.first << ": " << medio << " (" << maxUso << " veces)" << endl;
        cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarMedioEnvioPorCategoria() const {
    contadorIfs = 0;
    map<string, map<string, int>> datos;
    for (const auto& v : registros) {
        datos[v.categoria][v.medio_envio]++;
    }
    cout << "Medio de envio mas utilizado por categoria:\n";
    for (const auto& cat : datos) {
        string medio;
        int maxUso = 0;
        for (const auto& medioUso : cat.second) {
            if ((++contadorIfs, medioUso.second > maxUso)) {
                maxUso = medioUso.second;
                medio = medioUso.first;
            }
        }
        cout << "  " << cat.first << ": " << medio << " (" << maxUso << " veces)" << endl;
         cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";

    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarDiaMayorVenta() const {
    contadorIfs = 0;
    map<string, double> datos;
    for (const auto& v : registros) {
        datos[v.fecha] += v.monto_total;
    }
    string mejorDia;
    double maxVenta = 0;
    for (const auto& d : datos) {
        if ((++contadorIfs, d.second > maxVenta)) {
            maxVenta = d.second;
            mejorDia = d.first;
        }
    }
    cout << "Dia con mayor monto total de ventas: " << mejorDia << " ($" << maxVenta << ")\n\n";
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Map"<<endl;

}

void GestorCSV::mostrarEstadoEnvioPorPais() const {
    contadorIfs = 0;
    map<string, map<string, int>> datos;
    for (const auto& v : registros) {
        datos[v.pais][v.estado_envio]++;
    }
    cout << "Estado de envio mas frecuente por pais:\n";
    for (const auto& pais : datos) {
        string estado;
        int maxVeces = 0;
        for (const auto& e : pais.second) {
            if ((++contadorIfs, e.second > maxVeces)) {
                maxVeces = e.second;
                estado = e.first;
            }
        }
        cout << "  " << pais.first << ": " << estado << " (" << maxVeces << " veces)" << endl;
        cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";

    }
    cout<<"Se utilizo: Map";
    cout << endl;
}

void GestorCSV::mostrarProductoMasYMenosVendido() const {
    contadorIfs = 0;
    map<string, int> datos;
    for (const auto& v : registros) {
        datos[v.producto] += v.cantidad;
    }
    string masVendido, menosVendido;
    int maxCant = 0, minCant = INT_MAX;
    for (const auto& p : datos) {
        if ((++contadorIfs, p.second > maxCant)) {
            maxCant = p.second;
            masVendido = p.first;
            cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";

        }
        if ((++contadorIfs,p.second < minCant)) {
            minCant = p.second;
            menosVendido = p.first;
        }
        cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";

    }
    cout << "Producto mas vendido en cantidad total: " << masVendido << " (" << maxCant << " unidades)\n";
    cout << "Producto menos vendido en cantidad total: " << menosVendido << " (" << minCant << " unidades)\n\n";
    cout<<"Se utilizo: Map";
}

void GestorCSV::procesarEstadisticas() const {
    mostrarTop5CiudadesPorPais();
    mostrarMontoPorProductoPorPais();
    mostrarPromedioPorCategoriaPorPais();
    mostrarMedioEnvioPorPais();
    mostrarMedioEnvioPorCategoria();
    mostrarDiaMayorVenta();
    mostrarEstadoEnvioPorPais();
    mostrarProductoMasYMenosVendido();
}

void GestorCSV::agregarVenta() {
    contadorIfs = 0;
    Registro reg;
    int opcionPais;

    reg.id_venta = siguienteIdVenta++;
    reg.fecha = obtenerFechaActual();

    const string paises[] = {
        "Argentina", "Uruguay", "Paraguay", "Chile", "Bolivia",
        "Brasil", "Peru", "Ecuador", "Colombia", "Venezuela",
        "Guyana", "Surinam"
    };

    cout << "Seleccione el país:\n";
    for (int i = 0; i < 12; ++i) {
        cout << " " << (i + 1) << ". " << paises[i] << '\n';
    }

    do {
        cout << "Ingrese el número correspondiente: ";
        cin >> opcionPais;
        if ((++contadorIfs, opcionPais < 1 || opcionPais > 12)) {
            cout << "Opción inválida. Intente nuevamente.\n";
             cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";

        }
    } while (opcionPais < 1 || opcionPais > 12);

    reg.pais = paises[opcionPais - 1];
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  

    cout << "Ingrese ciudad: "; getline(cin, reg.ciudad);
    cout << "Ingrese cliente: "; getline(cin, reg.cliente);
    cout << "Ingrese producto: "; getline(cin, reg.producto);
    cout << "Ingrese categoría: "; getline(cin, reg.categoria);

    cout << "Ingrese cantidad: "; cin >> reg.cantidad;
    cout << "Ingrese precio unitario: "; cin >> reg.precio_unitario;
    reg.monto_total = reg.cantidad * reg.precio_unitario;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Ingrese medio de envío: "; getline(cin, reg.medio_envio);
    cout << "Ingrese estado de envío: "; getline(cin, reg.estado_envio);

    registros.push_back(reg);
    cout << "Venta agregada correctamente.\n";

    ofstream archivo("ventas_sudamerica.csv", ios::app);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo CSV para agregar la venta.\n";
        return;
    }

    archivo << reg.id_venta << ','
            << reg.fecha << ','
            << reg.pais << ','
            << reg.ciudad << ','
            << reg.cliente << ','
            << reg.producto << ','
            << reg.categoria << ','
            << reg.cantidad << ','
            << reg.precio_unitario << ','
            << reg.monto_total << ','
            << reg.medio_envio << ','
            << reg.estado_envio << '\n';

    archivo.close();

    cout << "Venta agregada correctamente y guardada en el archivo.\n";
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Vectores"<<endl;
}


string trim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
    return s;
}

string aMinuscula(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void GestorCSV::eliminarVenta() {
    contadorIfs = 0; 
    string criterio, texto;
    int opcion;
    vector<Registro> filtrados;

    cout << "¿Buscar por país o ciudad?\n";
    cout << "1. País\n";
    cout << "2. Ciudad\n";
    cin >> opcion;
    cin.ignore();  


    if (opcion == 1) {
        texto = "país";
    } else if (opcion == 2) {
        texto = "ciudad";
    } else {
        cout << "Opción inválida.\n";
        return;
    }

    cout << "Ingrese el nombre del " << texto << ": ";
    getline(cin, criterio);
    criterio = aMinuscula(trim(criterio));

    for (const auto& r : registros) {
        string campo = (opcion == 1) ? r.pais : r.ciudad;
        if ((aMinuscula(trim(campo)) == criterio)) {
            filtrados.push_back(r);
        }
    }

    if ((++contadorIfs, filtrados.empty())) {
        cout << "No se encontraron ventas con ese criterio.\n";
        return;
    }

    cout << "Ventas encontradas:\n";
    for (size_t i = 0; i < filtrados.size(); ++i) {
        cout << i + 1 << ". ";
        filtrados[i].imprimir();
    }

    int indice;
    cout << "Seleccione el número de venta a eliminar: ";
    cin >> indice;

    if ((++contadorIfs, indice < 1 || indice > filtrados.size())) {
        cout << "Índice inválido.\n";
        return;
    }

    Registro aEliminar = filtrados[indice - 1];

    registros.erase(remove_if(registros.begin(), registros.end(), [&](const Registro& r) {
        ++contadorIfs;
        return r.id_venta == aEliminar.id_venta;
    }), registros.end());

    ofstream archivo("ventas_sudamerica.csv");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para escribir.\n";
        return;
    }

    archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";
    for (const auto& r : registros) {
        archivo << r.id_venta << ',' << r.fecha << ',' << r.pais << ',' << r.ciudad << ','
                << r.cliente << ',' << r.producto << ',' << r.categoria << ',' << r.cantidad << ','
                << r.precio_unitario << ',' << r.monto_total << ',' << r.medio_envio << ',' << r.estado_envio << '\n';
    }

    archivo.close();
    cout << "Venta eliminada correctamente del sistema y del archivo.\n";
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Vectores"<<endl;
}



void GestorCSV::modificarVenta() {
    int id;
    cout << "Ingrese ID de venta a modificar: ";
    cin >> id;
    cin.ignore();

    // Buscar la venta por ID
    auto it = find_if(registros.begin(), registros.end(), [=](const Registro& r) {
        return r.id_venta == id;
    });

    if ((++contadorIfs, it == registros.end())) {
        cout << "Venta no encontrada.\n";
        return;
    }

    Registro& reg = *it;

    // Mostrar y modificar los datos
    cout << "Ingrese nuevo país (actual: " << reg.pais << "): "; getline(cin, reg.pais);
    cout << "Ingrese nueva ciudad (actual: " << reg.ciudad << "): "; getline(cin, reg.ciudad);
    cout << "Ingrese nuevo cliente (actual: " << reg.cliente << "): "; getline(cin, reg.cliente);
    cout << "Ingrese nuevo producto (actual: " << reg.producto << "): "; getline(cin, reg.producto);
    cout << "Ingrese nueva categoría (actual: " << reg.categoria << "): "; getline(cin, reg.categoria);
    cout << "Ingrese nueva cantidad (actual: " << reg.cantidad << "): "; cin >> reg.cantidad;
    cout << "Ingrese nuevo precio unitario (actual: " << reg.precio_unitario << "): "; cin >> reg.precio_unitario;
    cin.ignore();
    reg.monto_total = reg.cantidad * reg.precio_unitario;
    cout << "Ingrese nuevo medio de envío (actual: " << reg.medio_envio << "): "; getline(cin, reg.medio_envio);
    cout << "Ingrese nuevo estado de envío (actual: " << reg.estado_envio << "): "; getline(cin, reg.estado_envio);

    cout << "Venta modificada correctamente.\n";

  
    ofstream archivo("ventas_sudamerica.csv");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para escribir.\n";
        return;
    }

    // Escribir encabezado
    archivo << "ID_Venta,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio_Unitario,Monto_Total,Medio_Envio,Estado_Envio\n";

    // Escribir todos los registros (incluyendo el modificado)
    for (const auto& r : registros) {
        archivo << r.id_venta << ','
                << r.fecha << ','
                << r.pais << ','
                << r.ciudad << ','
                << r.cliente << ','
                << r.producto << ','
                << r.categoria << ','
                << r.cantidad << ','
                << r.precio_unitario << ','
                << r.monto_total << ','
                << r.medio_envio << ','
                << r.estado_envio << '\n';
    }

    archivo.close();
cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
cout<<"Se utilizo: Vectores"<<endl;
}

unsigned int hashCiudad(const string& ciudad) {
    unsigned int hash = 0;
    for (char c : ciudad) {
        hash = hash * 31 + c;
    }
    return hash;
}

unsigned int hashCiudad(string ciudad) {
    unsigned int hash = 0;
    for (char c : ciudad) {
        hash = hash * 31 + c;
    }
    return hash;
}

   // Consultas Dinámicas
void GestorCSV::mostrarVentasPorCiudad() const {
    string ciudadBuscada;
    cout << "Ingrese la ciudad a consultar: ";
    getline(cin, ciudadBuscada);

    // Crear el hashmap y cargar registros
    HashMapList<string, Registro> mapa(100, hashCiudad);
    for (const auto& venta : registros) {
        mapa.put(venta.ciudad, venta);
    }

    try {
        cout << "\nVentas en la ciudad de " << ciudadBuscada << ":\n";
        mapa.getList(ciudadBuscada);
    } catch (...) {
        cout << "No se encontraron ventas en esa ciudad.\n";
    }
    cout<<"Se utilizo HashMap";
}


void GestorCSV::mostrarVentasPorRangoFechaYPais() const {
    string pais, fechaInicio, fechaFin;

    cout << "Ingrese país: ";
    getline(cin, pais);
    cout << "Ingrese fecha de inicio (YYYY-MM-DD): ";
    getline(cin, fechaInicio);
    cout << "Ingrese fecha de fin (YYYY-MM-DD): ";
    getline(cin, fechaFin);

    Lista<Registro> listaVentas;

    // Cargar los registros filtrados en una lista desde vector
    for (const auto& reg : registros) {
        if ((++contadorIfs, reg.pais == pais && reg.fecha >= fechaInicio && reg.fecha <= fechaFin)) {
            listaVentas.insertarUltimo(reg);
        }
    }

    if ((++contadorIfs, listaVentas.esVacia())) {
        cout << "No se encontraron ventas para ese criterio.\n";
        return;
    }

    cout << "\nVentas en " << pais << " entre " << fechaInicio << " y " << fechaFin << ":\n";
    Nodo<Registro>* nodo = listaVentas.getInicio();
    while (nodo != nullptr) {
        nodo->getDato().imprimir();
        nodo = nodo->getSiguiente();
    }

    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Listas";
}
   
// Comparación entre dos productos discriminado por país usando Árbol AVL
struct DatosProducto {
    string pais;
    int cantidadA = 0;
    double montoA = 0;
    int cantidadB = 0;
    double montoB = 0;

    // Agrega la cantidad y monto al producto correspondiente
    void agregar(int cantidad, double monto, const string& producto, const string& productoA = "", const string& productoB = "") {
        if ((++contadorIfs, !productoA.empty() && producto == productoA)) {
            cantidadA += cantidad;
            montoA += monto;
        } else if ((++contadorIfs, !productoB.empty() && producto == productoB)) {
            cantidadB += cantidad;
            montoB += monto;
        }
    }

      

    bool operator<(const DatosProducto& otro) const {
        return pais < otro.pais;
    }
    bool operator>(const DatosProducto& otro) const {
        return pais > otro.pais;
    }
    bool operator==(const DatosProducto& otro) const {
        return pais == otro.pais;
    }
  
    
};
void GestorCSV::compararProductosPorPaisAVL(const string& productoA, const string& productoB) const {
    contadorIfs = 0;
    ArbolBinarioAVL<DatosProducto> arbol;

    for (int i = 0; i < registros.size(); ++i) {
        Registro reg = registros[i];
        DatosProducto nodo;
        nodo.pais = reg.pais;

       try {
        contadorIfs++;
    DatosProducto actual = arbol.search(nodo);
          arbol.remove(nodo);
         actual.agregar(reg.cantidad, reg.monto_total, reg.producto, productoA, productoB);

         arbol.put(actual);
} catch (...) {
    nodo.agregar(reg.cantidad, reg.monto_total, reg.producto, productoA, productoB);

    arbol.put(nodo);
}

        
    }
    cout<<"Se utilizo: ArbolesAVL"<<endl;
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout << "\nComparación de productos por país (AVL):\n";
    cout << left << setw(15) << "País" << setw(20) << productoA + " (Cant)" << setw(20) << productoA + " (Monto)"
         << setw(20) << productoB + " (Cant)" << setw(20) << productoB + " (Monto)" << endl;
    
   arbol.recorrer(arbol.getRaiz()); // Llama a la función recorrer con la raíz del árbol AVL
}

struct DatosPais {
    string pais;
    double montoTotal = 0;
    map<string, int> productosVendidos;
    map<string, int> envios;

    bool operator<(const DatosPais& otro) const { return pais < otro.pais; }
    bool operator>(const DatosPais& otro) const { return pais > otro.pais; }
    bool operator==(const DatosPais& otro) const { return pais == otro.pais; }
};
void GestorCSV::compararPaisesAVL(const string& pais1, const string& pais2) const {
    ArbolBinarioAVL<DatosPais> arbol;

    for (int i = 0; i < registros.size(); ++i) {
        Registro reg = registros[i];
        DatosPais nodo;
        nodo.pais = reg.pais;

        try {
            DatosPais actual = arbol.search(nodo);
            arbol.remove(nodo);

            actual.montoTotal += reg.monto_total;
            actual.productosVendidos[reg.producto] += reg.cantidad;
            actual.envios[reg.medio_envio]++;

            arbol.put(actual);
        } catch (...) {
            nodo.montoTotal = reg.monto_total;
            nodo.productosVendidos[reg.producto] = reg.cantidad;
            nodo.envios[reg.medio_envio] = 1;
            arbol.put(nodo);
        }
    }

    auto mostrarResumen = [](const DatosPais& pais) {
        contadorIfs = 0;
        string productoMasVendido = "";
        int maxCantidad = 0;
        for (const auto& p : pais.productosVendidos) {
            if ((++contadorIfs, p.second > maxCantidad)) {
                maxCantidad = p.second;
                productoMasVendido = p.first;
            }
        }

        string envioMasUsado = "";
        int maxEnvios = 0;
        for (const auto& e : pais.envios) {
            if ((++contadorIfs, e.second > maxEnvios)) {
                maxEnvios = e.second;
                envioMasUsado = e.first;
            }
        }

        cout << "\nPaís: " << pais.pais << endl;
        cout << "Monto total de ventas: " << pais.montoTotal << endl;
        cout << "Producto más vendido: " << productoMasVendido << " (" << maxCantidad << " unidades)" << endl;
        cout << "Medio de envío más usado: " << envioMasUsado << " (" << maxEnvios << " veces)" << endl;
        
        
    };

    try {
        cout << "\n Comparación de Países:\n";
        mostrarResumen(arbol.search({pais1}));
        mostrarResumen(arbol.search({pais2}));
    } catch (...) {
        cout << "Uno de los países no tiene datos registrados.\n";
    }
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: ArbolesAVL"<<endl;
}

struct Datos {
    int cantidad = 0;
    double montoTotal = 0;

    void agregarVenta(int cant, double monto) {
        cantidad += cant;
        montoTotal += monto;
    }

    double promedio() const {
        return cantidad == 0 ? 0 : montoTotal / cantidad;
    }
};

void GestorCSV::buscarProductosPorDebajoDelUmbral() const {
    contadorIfs = 0;
    string paisBuscado;
    double umbral;

    cout << "Ingrese el país a analizar: ";
    getline(cin, paisBuscado);
    cout << "Ingrese el umbral de precio promedio: ";
    cin >> umbral;
    cin.ignore();

    // HashMap: producto → (monto total, cantidad total)
    HashMapList<string, pair<double, int>> productos(100, [](string clave) {
        unsigned int hash = 0;
        for (char c : clave) {
            hash = hash * 31 + c;
        }
        return hash;
    });

    // Acumular montos y cantidades por producto
    for (int i = 0; i < registros.size(); ++i) {
        Registro reg = registros[i];
        if ((++contadorIfs, reg.pais == paisBuscado)) {
            try {
                pair<double, int> actual = productos.get(reg.producto);
                productos.remove(reg.producto);
                actual.first += reg.monto_total;
                actual.second += reg.cantidad;
                productos.put(reg.producto, actual);
            } catch (...) {
                productos.put(reg.producto, make_pair(reg.monto_total, reg.cantidad));
            }
        }
        
    }

    cout << "\nProductos en " << paisBuscado << " con precio promedio menor a " << umbral << ":\n";
    bool hayResultado = false;

    // Mostrar resultados
    int capacidad = productos.getCapacity();
    for (int i = 0; i < capacidad; ++i) {
        Lista<HashEntry<string, pair<double, int>>>** tabla = productos.getTabla();
        if (tabla[i] != nullptr) {
            Nodo<HashEntry<string, pair<double, int>>>* nodo = tabla[i]->getInicio();
            while (nodo != nullptr) {
                string producto = nodo->getDato().getClave();
                pair<double, int> datos = nodo->getDato().getValor();
                double promedio = datos.first / datos.second;

                if ((++contadorIfs,promedio < umbral)) {
                    cout << "Producto: " << producto
                         << " | Precio promedio: " << promedio << endl;
                    hayResultado = true;
                }

                nodo = nodo->getSiguiente();
            }
        }
    }

    if ((++contadorIfs,!hayResultado)) {
        cout << "No se encontraron productos bajo el umbral especificado.\n";
    }
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Hashmap"<<endl;
}

void GestorCSV::buscarProductosPorEncimaDelUmbralPorPais() const {
    string pais;
    double umbral;

    cout << "Ingrese el país: ";
    getline(cin, pais);
    cout << "Ingrese el umbral de precio promedio: ";
    cin >> umbral;
    cin.ignore();

   
    HashMapList<string, pair<double, int>> productos(100, [](string clave) {
        unsigned int hash = 0;
        for (char c : clave) {
            hash = hash * 31 + c;
        }
        return hash;
    });

    // Acumular montos y cantidades SOLO para productos vendidos en el país dado
    for (int i = 0; i < registros.size(); ++i) {
        contadorIfs = 0;
        Registro reg = registros[i];
        ++contadorIfs;
        if ((reg.pais != pais)) continue;

        try {
            pair<double, int> actual = productos.get(reg.producto);
            productos.remove(reg.producto);
            actual.first += reg.monto_total;
            actual.second += reg.cantidad;
            productos.put(reg.producto, actual);
        } catch (...) {
            productos.put(reg.producto, make_pair(reg.monto_total, reg.cantidad));
        }
    }

    cout << "\nProductos en " << pais << " con precio promedio mayor a " << umbral << ":\n";
    bool hayResultado = false;

    int capacidad = productos.getCapacity();
    Lista<HashEntry<string, pair<double, int>>>** tabla = productos.getTabla();
    for (int i = 0; i < capacidad; ++i) {
        if (tabla[i] != nullptr) {
            Nodo<HashEntry<string, pair<double, int>>>* nodo = tabla[i]->getInicio();
            while (nodo != nullptr) {
                string producto = nodo->getDato().getClave();
                pair<double, int> datos = nodo->getDato().getValor();
                double promedio = datos.first / datos.second;

                if ((++contadorIfs ,promedio > umbral)) {
                    cout << "Producto: " << producto
                         << " | Precio promedio: " << promedio << endl;
                    hayResultado = true;
                }

                nodo = nodo->getSiguiente();
            }
        }
    }
    ++contadorIfs;
    if ((!hayResultado)) {
        cout << "No se encontraron productos por encima del umbral en ese país.\n";
    }
    cout << "\n[Total de sentencias if utilizadas en todo el programa: " << contadorIfs << "]\n";
    cout<<"Se utilizo: Hashmap"<<endl;
}
