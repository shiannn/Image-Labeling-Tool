cmake_minimum_required(VERSION 3.5)
project( DisplayImage )

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/config_vars.txt)
    file(READ ${CMAKE_CURRENT_LIST_DIR}/config_vars.txt LOAD_VARS)
    set(OpenCV_DIR ${LOAD_VARS})
endif()

find_package(
    OpenCV REQUIRED
    PATHS ${OpenCV_DIR}
    NO_DEFAULT_PATH
)
add_subdirectory(BBox_Annotator)
add_subdirectory(Seg_Annotator)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage DisplayImage.cpp )

set_property(TARGET DisplayImage PROPERTY CXX_STANDARD 17)
target_link_libraries( DisplayImage ${OpenCV_LIBS} bbox seg )