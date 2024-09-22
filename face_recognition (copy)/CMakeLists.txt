cmake_minimum_required(VERSION 3.20)

project(face_recognition VERSION 0.1)

find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})