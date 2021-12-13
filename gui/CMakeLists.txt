set(CMAKE_CXX_STANDARD 20)

set(wxWidgets_ROOT_DIR C:/wxWidgets-3.1.3)
find_package(wxWidgets 3.1 COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

add_definitions(-D
        _USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D UNICODE -D wxUSE_UNICODE -D _WINDOWS -D __WXMSW__ -D _CRT_SECURE_NO_DEPRECATE -D _CRT_NONSTDC_NO_DEPRECATE -D NDEBUG)
add_executable(gui WIN32
        src/sudokugame.cc
        src/mainwindow.cc
        src/sudokugrid.cc
        )

set_property(TARGET gui PROPERTY VS_DPI_AWARE PerMonitor)
target_include_directories(gui PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(gui LINK_PUBLIC ${wxWidgets_LIBRARIES} backend)