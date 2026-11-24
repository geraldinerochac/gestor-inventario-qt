/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicacion Gestor de Inventario.
 */
#include <QApplication>
#include "ventanaprincipal.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VentanaPrincipal ventana;
    ventana.show();
    return app.exec();
}
