cmake_minimum_required(VERSION 3.20)

project(face_recognition VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)

find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)
find_package(CUDAToolkit REQUIRED)

add_compile_definitions(DLIB_USE_CUDA=1)
add_subdirectory(src)
target_link_libraries(${CMAKE_PROJECT_NAME} dlib::dlib ${OpenCV_LIBS} )