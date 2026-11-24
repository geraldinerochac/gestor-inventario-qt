/**
 * @file basedatos.cpp
 * @brief Implementacion de la clase BaseDatos.
 */
#include "basedatos.h"
#include <QSqlError>
#include <QVariant>

BaseDatos::~BaseDatos()
{
    if (m_db.isOpen())
        m_db.close();
}

bool BaseDatos::abrir(const QString &ruta)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(ruta);
    if (!m_db.open()) {
        m_error = m_db.lastError().text();
        return false;
    }

    QSqlQuery q(m_db);
    const bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS articulos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre TEXT NOT NULL, "
        "tipo TEXT NOT NULL, "
        "cantidad INTEGER NOT NULL CHECK(cantidad >= 0), "
        "stock_minimo INTEGER NOT NULL DEFAULT 5, "
        "ubicacion TEXT, "
        "fecha_adquisicion TEXT)");
    if (!ok)
        m_error = q.lastError().text();
    return ok;
}

bool BaseDatos::insertar(const Articulo &a)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO articulos "
              "(nombre, tipo, cantidad, stock_minimo, ubicacion, fecha_adquisicion) "
              "VALUES (:n, :t, :c, :s, :u, :f)");
    q.bindValue(":n", a.nombre());
    q.bindValue(":t", a.tipo());
    q.bindValue(":c", a.cantidad());
    q.bindValue(":s", a.stockMinimo());
    q.bindValue(":u", a.ubicacion());
    q.bindValue(":f", a.fecha());
    if (!q.exec()) {
        m_error = q.lastError().text();
        return false;
    }
    return true;
}

bool BaseDatos::actualizar(const Articulo &a)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE articulos SET nombre=:n, tipo=:t, cantidad=:c, "
              "stock_minimo=:s, ubicacion=:u, fecha_adquisicion=:f WHERE id=:id");
    q.bindValue(":n", a.nombre());
    q.bindValue(":t", a.tipo());
    q.bindValue(":c", a.cantidad());
    q.bindValue(":s", a.stockMinimo());
    q.bindValue(":u", a.ubicacion());
    q.bindValue(":f", a.fecha());
    q.bindValue(":id", a.id());
    if (!q.exec()) {
        m_error = q.lastError().text();
        return false;
    }
    return true;
}

bool BaseDatos::eliminar(int id)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM articulos WHERE id=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        m_error = q.lastError().text();
        return false;
    }
    return true;
}

Articulo BaseDatos::desdeConsulta(const QSqlQuery &q) const
{
    return Articulo(q.value(0).toInt(), q.value(1).toString(),
                    q.value(2).toString(), q.value(3).toInt(),
                    q.value(4).toInt(), q.value(5).toString(),
                    q.value(6).toString());
}

Articulo BaseDatos::obtener(int id) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, nombre, tipo, cantidad, stock_minimo, ubicacion, "
              "fecha_adquisicion FROM articulos WHERE id=:id");
    q.bindValue(":id", id);
    if (q.exec() && q.next())
        return desdeConsulta(q);
    return Articulo();
}

QList<Articulo> BaseDatos::listar(const QString &texto, const QString &tipo) const
{
    QList<Articulo> resultado;
    QString sql = "SELECT id, nombre, tipo, cantidad, stock_minimo, ubicacion, "
                  "fecha_adquisicion FROM articulos "
                  "WHERE (nombre LIKE :t1 OR ubicacion LIKE :t2)";
    if (!tipo.isEmpty())
        sql += " AND tipo = :tipo";
    sql += " ORDER BY nombre COLLATE NOCASE";

    QSqlQuery q(m_db);
    q.prepare(sql);
    const QString patron = "%" + texto + "%";
    q.bindValue(":t1", patron);
    q.bindValue(":t2", patron);
    if (!tipo.isEmpty())
        q.bindValue(":tipo", tipo);

    if (!q.exec()) {
        m_error = q.lastError().text();
        return resultado;
    }
    while (q.next())
        resultado.append(desdeConsulta(q));
    return resultado;
}

QStringList BaseDatos::tipos() const
{
    QStringList lista;
    QSqlQuery q(m_db);
    if (q.exec("SELECT DISTINCT tipo FROM articulos ORDER BY tipo COLLATE NOCASE")) {
        while (q.next())
            lista << q.value(0).toString();
    }
    return lista;
}

int BaseDatos::contarStockBajo() const
{
    QSqlQuery q(m_db);
    if (q.exec("SELECT COUNT(*) FROM articulos WHERE cantidad <= stock_minimo") && q.next())
        return q.value(0).toInt();
    return 0;
}
