cmake_minimum_required(VERSION 3.19)
project(console LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(console
    main.cpp
    REQUIREMENTS.md
    qtaskmanager.h qtaskmanager.cpp
)

target_link_libraries(console
    PRIVATE
        Qt::Core
)

install(TARGETS console
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
