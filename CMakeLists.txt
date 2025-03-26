cmake_minimum_required(VERSION 3.16)

project(LexiBar-v2 VERSION 1.0 LANGUAGES CXX)

# Configuration C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration automatique pour Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Recherche des composants Qt
find_package(Qt6 REQUIRED COMPONENTS Core Quick Gui Qml)

# Sources du projet
set(SOURCES
    src/main.cpp
    src/gestionlexibar.cpp
)

set(HEADERS
    src/gestionlexibar.h
)

# Fichiers QML
set(QML_FILES
    qml/main.qml
)

# Ressources
qt_add_resources(RESOURCES
    ressources/qml.qrc
)

# Création de l'exécutable
add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
    ${HEADERS}
    ${QML_FILES}
    ${RESOURCES}
)

# Définitions de compilation
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    QT_DEPRECATED_WARNINGS
    $<$<CONFIG:Debug>:QT_QML_DEBUG>
)

# Liaison avec les bibliothèques Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::Qml
)

# Pour le développement QML, définir le chemin d'importation QML
set_property(TARGET ${PROJECT_NAME} PROPERTY QT_QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)
set_property(TARGET ${PROJECT_NAME} PROPERTY QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)