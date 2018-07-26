# required cmake version
cmake_minimum_required(VERSION 2.8)
project(EpicFlow)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++11")

# packages
find_package(Ceres REQUIRED)

#指定opencv的路径, if you dont know what it is mean, google
#set(OpenCV_DIR "/usr/local/opencv2413/share/OpenCV")
#set(OpenCV_DIR "/usr/local/opencv341/share/OpenCV")
set(OpenCV_DIR "/usr/local/opencv320/share/OpenCV") # 指定opencv路径
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)	#这一句指定可执行文件的位置

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS})

file(GLOB CURRENT_HEADERS  *.h)
file(GLOB CURRENT_SOURCES  *.cpp)

add_subdirectory(EpicFlow)
include_directories(EpicFlow)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/EpicFlow)

add_executable(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME} epicflow
        ${OpenCV_LIBS}
        ${CERES_LIBRARIES}
        pthread
        jpeg
        png
        freetype
        harfbuzz)