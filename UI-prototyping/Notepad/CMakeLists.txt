cmake_minimum_required(VERSION 3.19)
project(Notepad LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

include_directories("C:/Qt/6.9.1/mingw_64/include")

qt_add_executable(Notepad
    WIN32 MACOSX_BUNDLE
    main.cpp
    notepad.cpp
    notepad.h
    notepad.ui
    toast.h
    toast.cpp
)

set(QSCINTILLA_LIB "C:/Qt/6.9.1/mingw_64/lib/libqscintilla2_qt6.a")

target_link_libraries(Notepad
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${QSCINTILLA_LIB}
)
target_link_libraries(Notepad PRIVATE Qt6::Widgets)

include(GNUInstallDirs)

install(TARGETS Notepad
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Notepad
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
