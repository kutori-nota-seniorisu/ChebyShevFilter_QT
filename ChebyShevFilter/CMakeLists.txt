cmake_minimum_required(VERSION 3.5)

project(ChebyShevFilter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

SET(FILT_SRCS
    chebyshev_iir.cpp
    find_roots.cpp
    iir_coeff.cpp
    design_iir.cpp
    )
add_library(spuce ${FILT_SRCS})
set_property(TARGET spuce PROPERTY CXX_STANDARD 11)
set_property(TARGET spuce PROPERTY POSITION_INDEPENDENT_CODE TRUE)

set_target_properties(spuce PROPERTIES DEFINE_SYMBOL "SPUCE_DLL_EXPORTS")
install(TARGETS spuce
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
    )
add_library(fft FFT.cpp)
add_executable(ChebyShevFilter
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    )

target_link_libraries(ChebyShevFilter PRIVATE Qt5::Widgets Qt5::Charts spuce fft)
