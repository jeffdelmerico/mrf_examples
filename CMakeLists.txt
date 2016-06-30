project(mrf_examples)
cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_BUILD_TYPE Release)

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(denoise src/denoise.cpp)
target_link_libraries(denoise ${OpenCV_LIBRARIES})
