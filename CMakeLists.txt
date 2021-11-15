cmake_minimum_required(VERSION 3.20)

project(21f_pa03_kynerd_hulcy)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.65.0 COMPONENTS graph)
find_package(PythonLibs 2.7 REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "-O3")

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} PRIVATE ${PYTHON_INCLUDE_DIRS})
    add_executable(21f_pa03_kynerd_hulcy src/main.cpp src/girvan_newman.h)
    target_link_libraries(21f_pa03_kynerd_hulcy ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
endif()