# required cmake version
cmake_minimum_required(VERSION 2.8)
project(EpicFlow)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++11")

# packages
find_package(Ceres REQUIRED)
#set(OpenCV_DIR "/usr/local/opencv2413/share/OpenCV") #修改了这一句，指定opencv头文件的路径
#set(OpenCV_DIR "/usr/local/opencv341/share/OpenCV")
set(OpenCV_DIR "/usr/local/opencv320/share/OpenCV")
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)	#这一句指定可执行文件的位置

include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS})

file(GLOB CURRENT_HEADERS  *.h DeepMatching122/*.h EpicFlow100/*.h SED/*.h)
file(GLOB CURRENT_SOURCES  *.cpp DeepMatching122/*.cpp EpicFlow100/*.c EpicFlow100/*.cpp SED/*.cpp)


add_executable(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        ${CERES_LIBRARIES}
        pthread
        jpeg
        png
        freetype
        harfbuzz)