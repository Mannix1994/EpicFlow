# EpicFlow

This is a library which is used to compute optic flow.it is 
based on [EpicFlow](http://lear.inrialpes.fr/src/epicflow/)
. I add a function to compute SED.

# How to use
Note that this is a cmake project based on OpenCV, so you should
install OpenCV.
* Download this Project
* Copy EpicFlow to your project
* Edit your CMakeLists.txt
```cmake
# add thres lines
add_subdirectory(EpicFlow)
include_directories(EpicFlow)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/EpicFlow)

add_executable(${PROJECT_NAME} ....)
# add library
target_link_libraries(${PROJECT_NAME} epicflow)
```  
* Here is a demo:
```cplusplus
#include "EpicFlowWithDeepMatching.h"

void func(){
    Mat pre = ...;
    Mat next = ...;
    if(pre.empty()){
        cerr<<__LINE__<<": pre is empty";
        return;
    }if(next.empty()){
        cerr<<__LINE__<<": next is empty";
        return;
    }
    assert(pre.type() == CV_8UC3 && pre.type() == next.type() && !"Not a color image");
    Mat optic_flow =  get_flow(pre,next);
}
```
