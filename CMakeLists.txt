cmake_minimum_required(VERSION 3.0.0)
project(Computational_Depth_of_Field VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Computational_Depth_of_Field main.cpp)

# Find OpenMP and link the target with OpenMP::OpenMP_CXX
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Computational_Depth_of_Field OpenMP::OpenMP_CXX)
endif()

target_link_libraries(Computational_Depth_of_Field ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
