#include <iostream>
#include "cliente.cpp"

int main() {
    try {
        Cliente cliente;
        cliente.Conectar();
        cliente.Comunicacion();
    } catch (const std::exception& e) {
        std::cerr << "Ocurrió una excepción: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Ocurrió un error desconocido." << std::endl;
        return 1;
    }
    return 0;
}
