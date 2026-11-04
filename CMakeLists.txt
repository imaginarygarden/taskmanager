cmake_minimum_required(VERSION 3.19)
project(console LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

qt_standard_project_setup()

qt_add_executable(console ${sources})

target_compile_features(console PUBLIC cxx_std_20)

target_link_libraries(console PRIVATE Qt::Core)

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
