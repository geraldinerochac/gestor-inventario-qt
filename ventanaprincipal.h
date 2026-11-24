/**
 * @file ventanaprincipal.h
 * @brief Ventana principal del Gestor de Inventario.
 */
#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "basedatos.h"

class QLineEdit;
class QComboBox;
class QTableWidget;
class QLabel;

/**
 * @class VentanaPrincipal
 * @brief Interfaz grafica principal: tabla, busqueda, filtros, alertas y CSV.
 */
class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT
public:
    /** @brief Construye la interfaz y abre la base de datos. */
    explicit VentanaPrincipal(QWidget *parent = nullptr);

private slots:
    /** @brief Recarga la tabla segun la busqueda y el filtro actuales. */
    void recargar();
    /** @brief Abre el formulario para agregar un articulo. */
    void agregar();
    /** @brief Abre el formulario para editar el articulo seleccionado. */
    void editar();
    /** @brief Elimina el articulo seleccionado (con confirmacion). */
    void eliminar();
    /** @brief Exporta la lista mostrada a un archivo CSV. */
    void exportarCsv();

private:
    /** @brief Devuelve el id de la fila seleccionada, o -1 si no hay. */
    int idSeleccionado() const;
    /** @brief Actualiza las opciones del filtro de tipo. */
    void refrescarTipos();

    BaseDatos m_bd;             ///< Acceso a la base de datos
    QLineEdit *m_busqueda;      ///< Caja de busqueda
    QComboBox *m_filtroTipo;    ///< Filtro por tipo
    QTableWidget *m_tabla;      ///< Tabla de articulos
    QLabel *m_alerta;           ///< Aviso de stock bajo
    QLabel *m_resumen;          ///< Resumen de totales
};

#endif // VENTANAPRINCIPAL_H
