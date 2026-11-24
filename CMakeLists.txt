cmake_minimum_required(VERSION 3.16)
project(GestorInventario VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(GestorInventario WIN32
    main.cpp
    articulo.h
    basedatos.h
    basedatos.cpp
    dialogoarticulo.h
    dialogoarticulo.cpp
    ventanaprincipal.h
    ventanaprincipal.cpp
)

target_link_libraries(GestorInventario PRIVATE Qt6::Widgets Qt6::Sql)
