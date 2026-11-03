cmake_minimum_required(VERSION 3.19)
project(console LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(console
    main.cpp
    REQUIREMENTS.md
    qtaskmanager.h qtaskmanager.cpp
    qconsolelistener.h qconsolelistener.cpp
)

target_compile_features(console PUBLIC cxx_std_20)

target_link_libraries(console
    PRIVATE
        Qt::Core
)

install(TARGETS console
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET console
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
)

install(SCRIPT ${deploy_script})

# file(CREATE_LINK
#   "${CMAKE_BINARY_DIR}/compile_commands.json"
#   "${CMAKE_SOURCE_DIR}/compile_commands.json"
#   SYMBOLIC
# )
