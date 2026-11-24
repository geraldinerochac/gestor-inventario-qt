/**
 * @file articulo.h
 * @brief Definicion de la clase Articulo.
 */
#ifndef ARTICULO_H
#define ARTICULO_H

#include <QString>

/**
 * @class Articulo
 * @brief Representa un articulo del inventario.
 *
 * Encapsula los datos de un articulo; los atributos son privados y se
 * acceden mediante metodos publicos.
 */
class Articulo
{
public:
    /** @brief Constructor por defecto (articulo vacio, id = 0). */
    Articulo() = default;

    /**
     * @brief Constructor completo.
     * @param id Identificador (0 si aun no esta en la base de datos).
     * @param nombre Nombre del articulo.
     * @param tipo Categoria (componente, herramienta, etc.).
     * @param cantidad Unidades disponibles.
     * @param stockMinimo Umbral para la alerta de stock bajo.
     * @param ubicacion Lugar fisico donde se guarda.
     * @param fecha Fecha de adquisicion en formato ISO (yyyy-MM-dd).
     */
    Articulo(int id, const QString &nombre, const QString &tipo, int cantidad,
             int stockMinimo, const QString &ubicacion, const QString &fecha)
        : m_id(id), m_nombre(nombre), m_tipo(tipo), m_cantidad(cantidad),
          m_stockMinimo(stockMinimo), m_ubicacion(ubicacion), m_fecha(fecha) {}

    /** @brief Devuelve el identificador. */
    int id() const { return m_id; }
    /** @brief Devuelve el nombre. */
    QString nombre() const { return m_nombre; }
    /** @brief Devuelve el tipo. */
    QString tipo() const { return m_tipo; }
    /** @brief Devuelve la cantidad en inventario. */
    int cantidad() const { return m_cantidad; }
    /** @brief Devuelve el stock minimo. */
    int stockMinimo() const { return m_stockMinimo; }
    /** @brief Devuelve la ubicacion. */
    QString ubicacion() const { return m_ubicacion; }
    /** @brief Devuelve la fecha de adquisicion (ISO). */
    QString fecha() const { return m_fecha; }

    /** @brief Asigna el identificador. */
    void setId(int id) { m_id = id; }

    /** @brief Indica si la cantidad esta en o por debajo del stock minimo. */
    bool stockBajo() const { return m_cantidad <= m_stockMinimo; }

private:
    int m_id = 0;           ///< Identificador unico
    QString m_nombre;       ///< Nombre del articulo
    QString m_tipo;         ///< Categoria
    int m_cantidad = 0;     ///< Unidades disponibles
    int m_stockMinimo = 0;  ///< Umbral de alerta
    QString m_ubicacion;    ///< Ubicacion fisica
    QString m_fecha;        ///< Fecha de adquisicion (yyyy-MM-dd)
};

#endif // ARTICULO_H
