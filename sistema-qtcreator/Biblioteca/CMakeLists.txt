cmake_minimum_required(VERSION 3.16)

project(Biblioteca VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appBiblioteca
    main.cpp
)

qt_add_qml_module(appBiblioteca
    URI Biblioteca
    QML_FILES
        Main.qml
        SOURCES Book.h
        SOURCES User.h
        SOURCES Librarian.h
        SOURCES Library.h
        SOURCES Library.cpp
        SOURCES User.cpp
        SOURCES Book.cpp
        QML_FILES MainScreen.qml
        QML_FILES LoginScreen.qml
        RESOURCES resources.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBiblioteca PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBiblioteca
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBiblioteca
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appBiblioteca
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
