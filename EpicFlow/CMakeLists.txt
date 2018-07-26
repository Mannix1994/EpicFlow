include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS})

file(GLOB HEADERS  *.h DeepMatching122/*.h EpicFlow100/*.h SED/*.h)
file(GLOB SOURCES  *.cpp DeepMatching122/*.cpp EpicFlow100/*.c EpicFlow100/*.cpp SED/*.cpp)

message(STATUS "Path: ${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(epicflow SHARED ${HEADERS} ${SOURCES})

target_link_libraries(epicflow
        ${OpenCV_LIBS}
        ${CERES_LIBRARIES}
        pthread
        jpeg
        png
        freetype
        harfbuzz)