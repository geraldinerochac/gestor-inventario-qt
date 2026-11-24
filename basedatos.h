/**
 * @file basedatos.h
 * @brief Definicion de la clase BaseDatos (acceso a SQLite).
 */
#ifndef BASEDATOS_H
#define BASEDATOS_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include "articulo.h"

/**
 * @class BaseDatos
 * @brief Gestiona la persistencia de los articulos en una base SQLite.
 *
 * Aisla todo el codigo SQL del resto de la aplicacion: la interfaz solo
 * trabaja con objetos Articulo.
 */
class BaseDatos
{
public:
    BaseDatos() = default;
    /** @brief Cierra la conexion si esta abierta. */
    ~BaseDatos();

    /**
     * @brief Abre (o crea) la base de datos y la tabla de articulos.
     * @param ruta Ruta del archivo .db
     * @return true si todo salio bien.
     */
    bool abrir(const QString &ruta);

    /** @brief Inserta un articulo nuevo. @return true si tuvo exito. */
    bool insertar(const Articulo &a);
    /** @brief Actualiza el articulo con el mismo id. @return true si tuvo exito. */
    bool actualizar(const Articulo &a);
    /** @brief Elimina el articulo con el id dado. @return true si tuvo exito. */
    bool eliminar(int id);
    /** @brief Obtiene un articulo por id (id = 0 si no existe). */
    Articulo obtener(int id) const;

    /**
     * @brief Lista articulos aplicando filtros.
     * @param texto Texto a buscar en nombre o ubicacion (vacio = sin filtro).
     * @param tipo Tipo exacto (vacio = todos).
     * @return Lista ordenada por nombre.
     */
    QList<Articulo> listar(const QString &texto = QString(),
                           const QString &tipo = QString()) const;

    /** @brief Devuelve los tipos distintos existentes en la base. */
    QStringList tipos() const;
    /** @brief Cuenta los articulos con stock bajo. */
    int contarStockBajo() const;
    /** @brief Devuelve el texto del ultimo error. */
    QString ultimoError() const { return m_error; }

private:
    /** @brief Construye un Articulo a partir de la fila actual de la consulta. */
    Articulo desdeConsulta(const QSqlQuery &q) const;

    QSqlDatabase m_db;          ///< Conexion SQLite
    mutable QString m_error;    ///< Ultimo mensaje de error
};

#endif // BASEDATOS_H
