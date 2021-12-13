add_library(backend src/sudoku.cc)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})