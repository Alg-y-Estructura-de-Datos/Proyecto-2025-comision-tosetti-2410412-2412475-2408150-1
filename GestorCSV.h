#pragma once
#include <string>
#include <vector>
#include <chrono>
using namespace std;

// Estructura que representa una venta (una línea del archivo CSV)
struct Registro {
    int id_venta;
    string fecha;
    string pais;
    string ciudad;
    string cliente;
    string producto;
    string categoria;
    int cantidad;
    double precio_unitario;
    double monto_total;
    string medio_envio;
    string estado_envio;

    void imprimir() const;
};

#include <ostream>

inline ostream& operator<<(ostream& os, const Registro& r) {
    os << "ID: " << r.id_venta
       << ", Fecha: " << r.fecha
       << ", País: " << r.pais
       << ", Ciudad: " << r.ciudad
       << ", Cliente: " << r.cliente
       << ", Producto: " << r.producto
       << ", Categoría: " << r.categoria
       << ", Cantidad: " << r.cantidad
       << ", Precio Unitario: " << r.precio_unitario
       << ", Monto Total: " << r.monto_total
       << ", Medio Envío: " << r.medio_envio
       << ", Estado Envío: " << r.estado_envio;
    return os;
}

class GestorCSV {
private:
    vector<Registro> registros;
    int siguienteIdVenta = 1;

public:
    bool leerArchivo(const string& nombreArchivo);
    void imprimirRegistros() const;

    // Funciones de análisis
    void mostrarTop5CiudadesPorPais() const;
    void mostrarMontoPorProductoPorPais() const;
    void mostrarPromedioPorCategoriaPorPais() const;
    void mostrarMedioEnvioPorPais() const;
    void mostrarMedioEnvioPorCategoria() const;
    void mostrarDiaMayorVenta() const;
    void mostrarEstadoEnvioPorPais() const;
    void mostrarProductoMasYMenosVendido() const;
    void procesarEstadisticas() const;
   

    // Funciones de modificación
    void agregarVenta();
    void eliminarVenta();
    void modificarVenta();

    void mostrarVentasPorCiudad() const;
    void mostrarVentasPorRangoFechaYPais() const;   
    void compararProductosPorPaisAVL(const string& productoA, const string& productoB) const;
    void compararPaisesAVL(const string &pais1, const string &pais2) const;
    void buscarProductosPorDebajoDelUmbral() const;
    void buscarProductosPorEncimaDelUmbralPorPais() const;
};
