message("Compiling target folder: ${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${PROJECT_NAME} face_recognition.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)