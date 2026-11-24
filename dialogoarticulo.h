/**
 * @file dialogoarticulo.h
 * @brief Dialogo para crear o editar un articulo.
 */
#ifndef DIALOGOARTICULO_H
#define DIALOGOARTICULO_H

#include <QDialog>
#include "articulo.h"

class QLineEdit;
class QComboBox;
class QSpinBox;
class QDateEdit;

/**
 * @class DialogoArticulo
 * @brief Formulario modal con los campos de un articulo.
 *
 * Si se le pasa un articulo existente, queda en modo edicion.
 */
class DialogoArticulo : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param parent Ventana padre.
     * @param articulo Articulo a editar (por defecto, uno vacio para crear).
     */
    explicit DialogoArticulo(QWidget *parent = nullptr,
                             const Articulo &articulo = Articulo());

    /** @brief Devuelve el articulo con los datos escritos en el formulario. */
    Articulo articulo() const;

    /** @brief Valida el formulario antes de cerrar el dialogo. */
    void accept() override;

private:
    int m_id = 0;               ///< Id del articulo en edicion (0 = nuevo)
    QLineEdit *m_nombre;        ///< Campo nombre
    QComboBox *m_tipo;          ///< Campo tipo (editable)
    QSpinBox *m_cantidad;       ///< Campo cantidad
    QSpinBox *m_stockMinimo;    ///< Campo stock minimo
    QLineEdit *m_ubicacion;     ///< Campo ubicacion
    QDateEdit *m_fecha;         ///< Campo fecha de adquisicion
};

#endif // DIALOGOARTICULO_H
