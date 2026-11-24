/**
 * @file dialogoarticulo.cpp
 * @brief Implementacion de DialogoArticulo.
 */
#include "dialogoarticulo.h"
#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QVBoxLayout>

DialogoArticulo::DialogoArticulo(QWidget *parent, const Articulo &articulo)
    : QDialog(parent), m_id(articulo.id())
{
    setWindowTitle(m_id == 0 ? "Nuevo articulo" : "Editar articulo");
    setModal(true);

    m_nombre = new QLineEdit(this);

    m_tipo = new QComboBox(this);
    m_tipo->setEditable(true);
    m_tipo->addItems({"Componente", "Herramienta", "Instrumento", "Consumible", "Otro"});

    m_cantidad = new QSpinBox(this);
    m_cantidad->setRange(0, 1000000);

    m_stockMinimo = new QSpinBox(this);
    m_stockMinimo->setRange(0, 1000000);
    m_stockMinimo->setValue(5);

    m_ubicacion = new QLineEdit(this);

    m_fecha = new QDateEdit(QDate::currentDate(), this);
    m_fecha->setDisplayFormat("yyyy-MM-dd");
    m_fecha->setCalendarPopup(true);

    if (m_id != 0) {
        m_nombre->setText(articulo.nombre());
        m_tipo->setCurrentText(articulo.tipo());
        m_cantidad->setValue(articulo.cantidad());
        m_stockMinimo->setValue(articulo.stockMinimo());
        m_ubicacion->setText(articulo.ubicacion());
        const QDate f = QDate::fromString(articulo.fecha(), Qt::ISODate);
        if (f.isValid())
            m_fecha->setDate(f);
    }

    auto *formulario = new QFormLayout;
    formulario->addRow("Nombre:", m_nombre);
    formulario->addRow("Tipo:", m_tipo);
    formulario->addRow("Cantidad:", m_cantidad);
    formulario->addRow("Stock minimo:", m_stockMinimo);
    formulario->addRow("Ubicacion:", m_ubicacion);
    formulario->addRow("Fecha de adquisicion:", m_fecha);

    auto *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(botones, &QDialogButtonBox::accepted, this, &DialogoArticulo::accept);
    connect(botones, &QDialogButtonBox::rejected, this, &DialogoArticulo::reject);

    auto *raiz = new QVBoxLayout(this);
    raiz->addLayout(formulario);
    raiz->addWidget(botones);
}

Articulo DialogoArticulo::articulo() const
{
    QString tipo = m_tipo->currentText().trimmed();
    if (tipo.isEmpty())
        tipo = "Otro";
    return Articulo(m_id, m_nombre->text().trimmed(), tipo, m_cantidad->value(),
                    m_stockMinimo->value(), m_ubicacion->text().trimmed(),
                    m_fecha->date().toString(Qt::ISODate));
}

void DialogoArticulo::accept()
{
    if (m_nombre->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Dato requerido", "El nombre del articulo no puede estar vacio.");
        m_nombre->setFocus();
        return;
    }
    QDialog::accept();
}
