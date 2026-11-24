/**
 * @file ventanaprincipal.cpp
 * @brief Implementacion de VentanaPrincipal.
 */
#include "ventanaprincipal.h"
#include "dialogoarticulo.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QStringConverter>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>

namespace {
/** @brief Encierra un texto entre comillas para CSV, escapando comillas internas. */
QString campoCsv(const QString &texto)
{
    QString s = texto;
    s.replace("\"", "\"\"");
    return QString("\"") + s + QString("\"");
}
}

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Gestor de Inventario");
    resize(950, 580);

    // --- Barra de busqueda y filtro ---
    m_busqueda = new QLineEdit(this);
    m_busqueda->setPlaceholderText("Buscar por nombre o ubicacion...");
    m_busqueda->setClearButtonEnabled(true);
    m_filtroTipo = new QComboBox(this);

    auto *barra = new QHBoxLayout;
    barra->addWidget(m_busqueda, 1);
    barra->addWidget(new QLabel("Tipo:", this));
    barra->addWidget(m_filtroTipo);

    // --- Alerta de stock ---
    m_alerta = new QLabel(this);
    m_alerta->setWordWrap(true);

    // --- Tabla ---
    m_tabla = new QTableWidget(this);
    m_tabla->setColumnCount(7);
    m_tabla->setHorizontalHeaderLabels({"ID", "Nombre", "Tipo", "Cantidad",
                                        "Stock min.", "Ubicacion", "Fecha adquisicion"});
    m_tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabla->verticalHeader()->setVisible(false);
    m_tabla->horizontalHeader()->setStretchLastSection(true);
    m_tabla->setColumnWidth(0, 50);
    m_tabla->setColumnWidth(1, 220);
    m_tabla->setSortingEnabled(true);

    // --- Botones ---
    auto *btnAgregar = new QPushButton("Agregar", this);
    auto *btnEditar = new QPushButton("Editar", this);
    auto *btnEliminar = new QPushButton("Eliminar", this);
    auto *btnCsv = new QPushButton("Exportar CSV", this);
    m_resumen = new QLabel(this);

    auto *botones = new QHBoxLayout;
    botones->addWidget(btnAgregar);
    botones->addWidget(btnEditar);
    botones->addWidget(btnEliminar);
    botones->addWidget(btnCsv);
    botones->addStretch(1);
    botones->addWidget(m_resumen);

    auto *central = new QWidget(this);
    auto *raiz = new QVBoxLayout(central);
    raiz->addLayout(barra);
    raiz->addWidget(m_alerta);
    raiz->addWidget(m_tabla, 1);
    raiz->addLayout(botones);
    setCentralWidget(central);

    // --- Conexiones ---
    connect(m_busqueda, &QLineEdit::textChanged, this, &VentanaPrincipal::recargar);
    connect(m_filtroTipo, &QComboBox::currentIndexChanged, this, &VentanaPrincipal::recargar);
    connect(btnAgregar, &QPushButton::clicked, this, &VentanaPrincipal::agregar);
    connect(btnEditar, &QPushButton::clicked, this, &VentanaPrincipal::editar);
    connect(btnEliminar, &QPushButton::clicked, this, &VentanaPrincipal::eliminar);
    connect(btnCsv, &QPushButton::clicked, this, &VentanaPrincipal::exportarCsv);
    connect(m_tabla, &QTableWidget::cellDoubleClicked, this, [this](int, int) { editar(); });

    // --- Base de datos ---
    const QString ruta = QCoreApplication::applicationDirPath() + "/inventario.db";
    if (!m_bd.abrir(ruta)) {
        QMessageBox::critical(this, "Error de base de datos",
                              "No se pudo abrir la base de datos:\n" + m_bd.ultimoError());
    }
    recargar();
}

void VentanaPrincipal::refrescarTipos()
{
    const QString actual = m_filtroTipo->currentText();
    const QSignalBlocker bloqueo(m_filtroTipo);
    m_filtroTipo->clear();
    m_filtroTipo->addItem("Todos");
    m_filtroTipo->addItems(m_bd.tipos());
    const int i = m_filtroTipo->findText(actual);
    m_filtroTipo->setCurrentIndex(i >= 0 ? i : 0);
}

void VentanaPrincipal::recargar()
{
    refrescarTipos();

    const QString tipo = m_filtroTipo->currentIndex() > 0 ? m_filtroTipo->currentText() : QString();
    const QList<Articulo> lista = m_bd.listar(m_busqueda->text().trimmed(), tipo);

    m_tabla->setSortingEnabled(false);
    m_tabla->setRowCount(static_cast<int>(lista.size()));

    int fila = 0;
    int unidades = 0;
    for (const Articulo &a : lista) {
        auto *id = new QTableWidgetItem;
        id->setData(Qt::DisplayRole, a.id());
        auto *nombre = new QTableWidgetItem(a.nombre());
        auto *tipoItem = new QTableWidgetItem(a.tipo());
        auto *cantidad = new QTableWidgetItem;
        cantidad->setData(Qt::DisplayRole, a.cantidad());
        auto *minimo = new QTableWidgetItem;
        minimo->setData(Qt::DisplayRole, a.stockMinimo());
        auto *ubicacion = new QTableWidgetItem(a.ubicacion());
        auto *fecha = new QTableWidgetItem(a.fecha());

        const QList<QTableWidgetItem *> items = {id, nombre, tipoItem, cantidad, minimo, ubicacion, fecha};
        int columna = 0;
        for (QTableWidgetItem *item : items) {
            if (a.stockBajo()) {
                item->setBackground(QColor(255, 205, 205));
                item->setForeground(QColor(120, 0, 0));
            }
            m_tabla->setItem(fila, columna++, item);
        }
        unidades += a.cantidad();
        ++fila;
    }
    m_tabla->setSortingEnabled(true);

    m_resumen->setText(QString("Articulos mostrados: %1  |  Unidades: %2")
                           .arg(lista.size()).arg(unidades));

    const int bajos = m_bd.contarStockBajo();
    if (bajos > 0) {
        m_alerta->setText(QString("ATENCION: %1 articulo(s) con stock bajo (filas en rojo).").arg(bajos));
        m_alerta->setStyleSheet("background:#ffd6d6; color:#7a0000; padding:6px; border-radius:4px;");
    } else {
        m_alerta->setText("Sin alertas de stock.");
        m_alerta->setStyleSheet("background:#d9f2d9; color:#134d13; padding:6px; border-radius:4px;");
    }
}

int VentanaPrincipal::idSeleccionado() const
{
    const int fila = m_tabla->currentRow();
    if (fila < 0)
        return -1;
    const QTableWidgetItem *item = m_tabla->item(fila, 0);
    return item ? item->data(Qt::DisplayRole).toInt() : -1;
}

void VentanaPrincipal::agregar()
{
    DialogoArticulo dialogo(this);
    if (dialogo.exec() == QDialog::Accepted) {
        if (!m_bd.insertar(dialogo.articulo()))
            QMessageBox::warning(this, "Error", "No se pudo guardar:\n" + m_bd.ultimoError());
        recargar();
    }
}

void VentanaPrincipal::editar()
{
    const int id = idSeleccionado();
    if (id < 0) {
        QMessageBox::information(this, "Editar", "Selecciona primero un articulo de la tabla.");
        return;
    }
    DialogoArticulo dialogo(this, m_bd.obtener(id));
    if (dialogo.exec() == QDialog::Accepted) {
        if (!m_bd.actualizar(dialogo.articulo()))
            QMessageBox::warning(this, "Error", "No se pudo actualizar:\n" + m_bd.ultimoError());
        recargar();
    }
}

void VentanaPrincipal::eliminar()
{
    const int id = idSeleccionado();
    if (id < 0) {
        QMessageBox::information(this, "Eliminar", "Selecciona primero un articulo de la tabla.");
        return;
    }
    const Articulo a = m_bd.obtener(id);
    const auto respuesta = QMessageBox::question(
        this, "Confirmar eliminacion",
        QString("Eliminar el articulo \"%1\"?").arg(a.nombre()));
    if (respuesta == QMessageBox::Yes) {
        if (!m_bd.eliminar(id))
            QMessageBox::warning(this, "Error", "No se pudo eliminar:\n" + m_bd.ultimoError());
        recargar();
    }
}

void VentanaPrincipal::exportarCsv()
{
    const QString ruta = QFileDialog::getSaveFileName(this, "Exportar inventario",
                                                      "inventario.csv", "Archivos CSV (*.csv)");
    if (ruta.isEmpty())
        return;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo crear el archivo.");
        return;
    }

    QTextStream salida(&archivo);
    salida.setEncoding(QStringConverter::Utf8);
    salida.setGenerateByteOrderMark(true);  // para que Excel lea bien las tildes
    salida << "sep=;\n";  // indica a Excel que el separador es punto y coma
    salida << "ID;Nombre;Tipo;Cantidad;Stock minimo;Ubicacion;Fecha adquisicion;Estado\n";

    const QString tipo = m_filtroTipo->currentIndex() > 0 ? m_filtroTipo->currentText() : QString();
    const QList<Articulo> lista = m_bd.listar(m_busqueda->text().trimmed(), tipo);
    for (const Articulo &a : lista) {
        salida << a.id() << ';' << campoCsv(a.nombre()) << ';' << campoCsv(a.tipo()) << ';'
               << a.cantidad() << ';' << a.stockMinimo() << ';' << campoCsv(a.ubicacion()) << ';'
               << a.fecha() << ';' << (a.stockBajo() ? "STOCK BAJO" : "OK") << '\n';
    }
    QMessageBox::information(this, "Exportar", "Reporte guardado correctamente.");
}
