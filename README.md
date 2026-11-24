# Gestor de Inventario (C++ / Qt / SQLite)

Aplicacion de escritorio para administrar el inventario de un laboratorio o tienda.
Proyecto final - Algoritmos en Sistemas Electronicos, Escuela Colombiana de Ingenieria Julio Garavito.

## Funciones
- Registro, edicion y eliminacion de articulos (nombre, tipo, cantidad, stock minimo, ubicacion, fecha de adquisicion)
- Busqueda y filtrado dinamico por texto y por tipo
- Alertas de stock bajo (filas en rojo y aviso superior)
- Exportacion de reportes a CSV
- Persistencia en SQLite (archivo `inventario.db` junto al ejecutable)

## Compilar
1. Abrir `CMakeLists.txt` con Qt Creator.
2. Seleccionar el kit *Desktop Qt 6.x MinGW 64-bit*.
3. Compilar y ejecutar (Ctrl+R).

## Documentacion
Ejecutar `doxygen Doxyfile`; el resultado queda en `docs/html/index.html`.

## Estructura
| Archivo | Responsabilidad |
|---|---|
| `articulo.h` | Clase de datos `Articulo` |
| `basedatos.*` | Acceso a SQLite (`BaseDatos`) |
| `dialogoarticulo.*` | Formulario de alta/edicion |
| `ventanaprincipal.*` | Ventana principal (GUI) |

## Autores
(Nombres del grupo)
