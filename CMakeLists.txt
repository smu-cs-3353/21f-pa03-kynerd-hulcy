cmake_minimum_required(VERSION 3.20)

project(21f_pa03_kynerd_hulcy)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.76.0 COMPONENTS graph)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(21f_pa03_kynerd_hulcy main.cpp)
    target_link_libraries(21f_pa03_kynerd_hulcy ${Boost_LIBRARIES})
endif()